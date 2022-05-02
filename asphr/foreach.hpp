//
// Copyright 2022 Anysphere, Inc.
// SPDX-License-Identifier: GPL-3.0-only
//

/*
Inspired by https://www.scs.stanford.edu/~dm/blog/va-opt.html.

Usage: 
* FOR_EACH(F, a, b, c, 1, 2, 3) // => F(a), F(b), F(c), F(1), F(2), F(3)
* FOR_EACH2(F1, F2, a, b, c, 1, 2, 3) // => F1(a), F2(b), F1(c), F2(1), F1(2), F2(3)
*/



#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, between, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1)                                                     \
  __VA_OPT__(, FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define FOR_EACH2(macro1, macro2, ...) \
    __VA_OPT__(EXPAND(FOR_EACH2_HELPER(macro1, macro2, __VA_ARGS__)))
#define FOR_EACH2_HELPER(macro1, macro2, a1, a2, ...)               \
  macro1(a1), macro2(a2) __VA_OPT__(, FOR_EACH2_AGAIN PARENS (macro1, macro2, __VA_ARGS__))
#define FOR_EACH2_AGAIN() FOR_EACH2_HELPER
