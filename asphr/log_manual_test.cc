//
// Copyright 2022 Anysphere, Inc.
// SPDX-License-Identifier: GPL-3.0-only
//

#include <cstdlib>   // std::exit
#include <iostream>  // std::cerr
#include <format>

#include "log.hpp"

int main(int argc, char** argv) {
  ASPHR_LOG_ERR("Message.", key, "value");
  ASPHR_LOG_ERR("Message.", key, "value", key2, 2);
  ASPHR_LOG_ERR("Message.");
  ASPHR_LOG_ERR("Message.", c++version, "c++20", key2, 2, key3, 3);
  return 0;
}