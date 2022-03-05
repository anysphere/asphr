#
# Copyright 2022 Anysphere, Inc.
# SPDX-License-Identifier: GPL-3.0-only
#

# intentionally empty!

toolchain(
    name = "built_cmake_toolchain_linux",
    exec_compatible_with = [
        "@bazel_tools//platforms:linux",
        "@bazel_tools//platforms:x86_64",
    ],
    toolchain = "@rules_foreign_cc//tools/build_defs/native_tools:built_cmake",
    toolchain_type = "@rules_foreign_cc//tools/build_defs:cmake_toolchain",
)

toolchain(
    name = "built_ninja_toolchain_linux",
    exec_compatible_with = [
        "@bazel_tools//platforms:linux",
        "@bazel_tools//platforms:x86_64",
    ],
    toolchain = "@rules_foreign_cc//tools/build_defs/native_tools:built_ninja",
    toolchain_type = "@rules_foreign_cc//tools/build_defs:ninja_toolchain",
)
