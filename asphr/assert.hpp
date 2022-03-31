//
// Copyright 2022 Anysphere, Inc.
// SPDX-License-Identifier: GPL-3.0-only
//

#pragma once

#include <boost/stacktrace.hpp>
#include <iostream>

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

// the following is inspired by BOOST. license:
// Boost Software License - Version 1.0 - August 17th, 2003

// Permission is hereby granted, free of charge, to any person or
// organization obtaining a copy of the software and accompanying
// documentation covered by this license (the "Software") to use, reproduce,
// display, distribute, execute, and transmit the Software, and to prepare
// derivative works of the Software, and to permit third-parties to whom the
// Software is furnished to do so, all subject to the following:

// The copyright notices in the Software and this entire statement,
// including the above license grant, this restriction and the following
// disclaimer, must be included in all copies of the Software, in whole or
// in part, and all derivative works of the Software, unless such copies or
// derivative works are solely in the form of machine-executable object code
// generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
// NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE
// DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY,
// WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#define PRINT_CERR(x, x_val) std::cerr << x << " = " << x_val << std::endl

// asserts
#ifndef NDEBUG
#define ASPHR_ASSERT_MSG(expr, msg)                                         \
  {                                                                         \
    if (ASPHR_PREDICT_TRUE(expr)) {                                         \
      static_cast<void>(0);                                                 \
    } else {                                                                \
      std::cerr << "Assertion failed: Expression '" << #expr                \
                << "' is false in function '" << __func__ << "' location '" \
                << __FILE__ << ":" << __LINE__ << "': '" << msg << "'.\n";  \
      std::cerr << boost::stacktrace::stacktrace() << std::endl;            \
      std::abort();                                                         \
    }                                                                       \
  }
#define ASPHR_ASSERT(expr) ASPHR_ASSERT_MSG(expr, "<no detail>")

#define ASPHR_ASSERT_EQ_MSG(a, b, msg)                                     \
  {                                                                        \
    auto a_val = a;                                                        \
    auto b_val = b;                                                        \
    if (ASPHR_PREDICT_TRUE((a_val == b_val))) {                            \
      static_cast<void>(0);                                                \
    } else {                                                               \
      PRINT_CERR(#a, a_val);                                               \
      PRINT_CERR(#b, b_val);                                               \
      std::cerr << "Assertion failed: Values '" << #a << "' and '" << #b   \
                << "' are different in function '" << __func__             \
                << "' location '" << __FILE__ << ":" << __LINE__ << "': '" \
                << msg << "'.\n";                                          \
      std::cerr << boost::stacktrace::stacktrace() << std::endl;           \
      std::abort();                                                        \
    }                                                                      \
  }
#define ASPHR_ASSERT_EQ(a, b) ASPHR_ASSERT_EQ_MSG(a, b, "<no detail>")

#else
#define ASPHR_ASSERT_MSG(expr, msg) static_cast<void>(0)
#define ASPHR_ASSERT(expr) static_cast<void>(0)
#define ASPHR_ASSERT_EQ_MSG(a, b, msg) static_cast<void>(0)
#define ASPHR_ASSERT_EQ(a, b) static_cast<void>(0)
#endif