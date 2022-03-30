//
// Copyright 2022 Anysphere, Inc.
// SPDX-License-Identifier: GPL-3.0-only
//

#pragma once

#include <stdlib.h>

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "absl/hash/hash.h"
#include "absl/random/random.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/escaping.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "third_party/json/nlohmann_json.h"
#include "utils.hpp"

using std::array;
using std::cerr;
using std::cout;
using std::endl;
using std::lock_guard;
using std::make_pair;
using std::make_shared;
using std::make_tuple;
using std::make_unique;
using std::pair;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::tuple;
using std::unique_ptr;
using std::vector;

namespace asphr {
using json = nlohmann::json;

using absl::Base64Escape;
using absl::Base64Unescape;
using absl::InvalidArgumentError;
using absl::Status;
using absl::StatusOr;
using absl::StrCat;
using absl::StrFormat;
using absl::Time;

using std::unordered_map;
using std::unordered_set;
}  // namespace asphr

#define CEIL_DIV(a, b) (((a) + (b)-1) / (b))

// MESSAGE_SIZE is the size of the message in bytes
constexpr size_t MESSAGE_SIZE = 1024;
constexpr size_t MESSAGE_SIZE_BITS = MESSAGE_SIZE * 8;

// pir_value is the type of the value stored at one index in the database
using pir_value_t = array<byte, MESSAGE_SIZE>;
// pir_index is the type of the index in the database. this should correspond to
// the index in the schema/messenger.proto definition
using pir_index_t = uint32_t;

// initiate the debug mode macros
#ifdef DEBUG
#define dbg(x) x
// x is the value, y is the message
#define debug(x, y) cout << y << ": " << x << endl
#define DEBUG_PRINT(x) std::cout << x << std::endl
#define DEBUG_PRINT_VAR(x) std::cout << #x << ": " << x << std::endl
#define DEBUG_PRINT_VAR_VAR(x, y) \
  std::cout << #x << ": " << x << " " << #y << ": " << y << std::endl
#else
#define dbg(x)
#define debug(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINT_VAR(x)
#define DEBUG_PRINT_VAR_VAR(x, y)
#endif

// from:
// https://cs.github.com/abseil/abseil-cpp/blob/3204cc0625230e9876f0310a6dea0014210ab325/absl/base/config.h#L171
// ASPHR_HAVE_BUILTIN()
//
// Checks whether the compiler supports a Clang Feature Checking Macro, and if
// so, checks whether it supports the provided builtin function "x" where x
// is one of the functions noted in
// https://clang.llvm.org/docs/LanguageExtensions.html
//
// Note: Use this macro to avoid an extra level of #ifdef __has_builtin check.
// http://releases.llvm.org/3.3/tools/clang/docs/LanguageExtensions.html
#ifdef __has_builtin
#define ASPHR_HAVE_BUILTIN(x) __has_builtin(x)
#else
#define ASPHR_HAVE_BUILTIN(x) 0
#endif

// from:
// https://cs.github.com/abseil/abseil-cpp/blob/6c8dab80c06820f475ce0a1fe873b8022cb274f6/absl/base/optimization.h#L153
//
// ASPHR_PREDICT_TRUE, ASPHR_PREDICT_FALSE
//
// Enables the compiler to prioritize compilation using static analysis for
// likely paths within a boolean branch.
//
// Example:
//
//   if (ASPHR_PREDICT_TRUE(expression)) {
//     return result;                        // Faster if more likely
//   } else {
//     return 0;
//   }
//
// Compilers can use the information that a certain branch is not likely to be
// taken (for instance, a CHECK failure) to optimize for the common case in
// the absence of better information (ie. compiling gcc with `-fprofile-arcs`).
//
// Recommendation: Modern CPUs dynamically predict branch execution paths,
// typically with accuracy greater than 97%. As a result, annotating every
// branch in a codebase is likely counterproductive; however, annotating
// specific branches that are both hot and consistently mispredicted is likely
// to yield performance improvements.
#if ASPHR_HAVE_BUILTIN(__builtin_expect) || \
    (defined(__GNUC__) && !defined(__clang__))
#define ASPHR_PREDICT_FALSE(x) (__builtin_expect(false || (x), false))
#define ASPHR_PREDICT_TRUE(x) (__builtin_expect(false || (x), true))
#else
#define ASPHR_PREDICT_FALSE(x) (x)
#define ASPHR_PREDICT_TRUE(x) (x)
#endif

// copied from BOOST:
// Boost Software License - Version 1.0 - August 17th, 2003

// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:

// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

inline void assertion_failed_msg(char const* expr, char const* msg,
                                 char const* function, char const* /*file*/,
                                 long /*line*/) {
  std::cerr << "Expression '" << expr << "' is false in function '" << function
            << "': " << (msg ? msg : "<...>") << ".\n"
            << "Backtrace:\n"
            << boost::stacktrace::stacktrace() << '\n';
  /*<-*/ std::exit(0); /*->*/
                       /*=std::abort();*/
}

inline void assertion_failed(char const* expr, char const* function,
                             char const* file, long line) {
  ::boost::assertion_failed_msg(expr, 0 /*nullptr*/, function, file, line);
}

// asserts
#ifdef DEBUG
#define ASPHR_ASSERT(expr)                          \
  (ABSL_PREDICT_TRUE((expr)) ? static_cast<void>(0) \
                             : [] { assert(false && #expr); }())
