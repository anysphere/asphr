//
// Copyright 2022 Anysphere, Inc.
// SPDX-License-Identifier: GPL-3.0-only
//

#pragma once

#include <iostream>

/*
ASPHR_LOG is the simplest possible logging mechanism.

Features we want:
* severity levels
* structured logging
* works fine with multiple threads

Features we don't want:
* everything else

Why not use a more standard logging library?
* glog reads environment variables, adds gflags, and adds a lot of other stuff
* log4cxx is a close cousin to log4j..... (need i say more?)
* nanolog requires a separate binary to decode the logfiles

If it is determined that the logging ever causes a performance bottleneck, it
may be worth looking into the above libraries. Until then, this simple approach
wins.

We log to stderr only.

Log level is set at compile time:
- ASPHR_LOGLEVEL_NONE: log only errors
- ASPHR_LOGLEVEL_ERR: log only errors
- ASPHR_LOGLEVEL_INFO: log errors and info
- ASPHR_LOGLEVEL_DEBUG: log everything

Default is ASPHR_LOGLEVEL_DEBUG.

*/

// ASPHR_LOG_ERR(msg, key, value, key, value, ...)
// ASPHR_LOG_INFO(msg, key, value, key, value, ...)
// ASPHR_LOG_DBG(msg, key, value, key, value, ...)

#if !defined(ASPHR_LOGLEVEL_NONE) && !defined(ASPHR_LOGLEVEL_ERR) && \
    !defined(ASPHR_LOGLEVEL_INFO) && !defined(ASPHR_LOGLEVEL_DEBUG)
#define ASPHR_LOGLEVEL_DEBUG
#endif

#if defined(ASPHR_LOGLEVEL_ERR) || defined(ASPHR_LOGLEVEL_INFO) || \
    defined(ASPHR_LOGLEVEL_DEBUG)
#define ASPHR_LOG_ERR(msg, ...)                                    \
  {                                                                \
    auto s = asbl::StrCat("[", __FILE__, ":", __LINE__, "] ", msg, \
                          __VA_ARGS__, "\n");                      \
    std::cerr << s;                                                \
    std::cerr.flush();                                             \
  }
#else
#define ASPHR_LOG_ERR(msg, ...) static_cast<void>(0)
#endif