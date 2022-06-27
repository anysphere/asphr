#
# Copyright 2022 Anysphere, Inc.
# SPDX-License-Identifier: GPL-3.0-only
#

"""Load asphr repos."""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

RUST_VERSION = "1.61.0"

def load_asphr_repos(asphr_path):
    """Loads the repositories for the asphr project.

    Args:
        asphr_path: The path to the asphr repository.
    """

    BAZEL_TOOLCHAIN_TAG = "0.6.3"

    BAZEL_TOOLCHAIN_SHA = "da607faed78c4cb5a5637ef74a36fdd2286f85ca5192222c4664efec2d529bb8"

    http_archive(
        name = "com_grail_bazel_toolchain",
        canonical_id = BAZEL_TOOLCHAIN_TAG,
        sha256 = BAZEL_TOOLCHAIN_SHA,
        strip_prefix = "bazel-toolchain-{tag}".format(tag = BAZEL_TOOLCHAIN_TAG),
        url = "https://github.com/grailbio/bazel-toolchain/archive/{tag}.tar.gz".format(tag = BAZEL_TOOLCHAIN_TAG),
    )

    http_archive(
        name = "rules_proto_grpc",
        sha256 = "507e38c8d95c7efa4f3b1c0595a8e8f139c885cb41a76cab7e20e4e67ae87731",
        strip_prefix = "rules_proto_grpc-4.1.1",
        urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/4.1.1.tar.gz"],
    )

    http_archive(
        name = "rules_proto",
        sha256 = "66bfdf8782796239d3875d37e7de19b1d94301e8972b3cbd2446b332429b4df1",
        strip_prefix = "rules_proto-4.0.0",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz",
            "https://github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz",
        ],
    )

    http_archive(
        name = "com_github_grpc_grpc",
        sha256 = "9647220c699cea4dafa92ec0917c25c7812be51a18143af047e20f3fb05adddc",
        strip_prefix = "grpc-1.43.0",
        urls = ["https://github.com/grpc/grpc/archive/refs/tags/v1.43.0.tar.gz"],
    )

    http_archive(
        name = "com_google_absl",
        sha256 = "c696ed0f7fe14d2d5a95d89116dbeb0fa945d7c249c4c6ffc10a469c303628cb",
        strip_prefix = "abseil-cpp-73316fc3c565e5998983b0fb502d938ccddcded2",
        urls = ["https://github.com/abseil/abseil-cpp/archive/73316fc3c565e5998983b0fb502d938ccddcded2.zip"],  # master as of 2022-02-11
    )

    http_archive(
        name = "com_google_googletest",
        sha256 = "5cf189eb6847b4f8fc603a3ffff3b0771c08eec7dd4bd961bfd45477dd13eb73",
        strip_prefix = "googletest-609281088cfefc76f9d0ce82e1ff6c30cc3591e5",
        urls = ["https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip"],
    )

    http_archive(
        name = "com_github_gperftools_gperftools",
        build_file = asphr_path + "//:gperftools.BUILD",
        patch_args = ["-p1"],
        patches = [asphr_path + "//:gperftools.patch"],  # random_shuffle not supported by c++20
        sha256 = "83e3bfdd28b8bcf53222c3798d4d395d52dadbbae59e8730c4a6d31a9c3732d8",
        strip_prefix = "gperftools-2.10",
        urls = ["https://github.com/gperftools/gperftools/releases/download/gperftools-2.10/gperftools-2.10.tar.gz"],
    )

    http_archive(
        name = "rules_cc",
        sha256 = "4dccbfd22c0def164c8f47458bd50e0c7148f3d92002cdb459c2a96a68498241",
        urls = ["https://github.com/bazelbuild/rules_cc/releases/download/0.0.1/rules_cc-0.0.1.tar.gz"],
    )

    http_archive(
        name = "rules_foreign_cc",
        sha256 = "62e364a05370059f07313ec46ae4205af23deb00e41f786f3233a98098c7e636",
        strip_prefix = "rules_foreign_cc-ae4ff42901354e2da8285dac4be8329eea2ea96a",
        url = "https://github.com/bazelbuild/rules_foreign_cc/archive/ae4ff42901354e2da8285dac4be8329eea2ea96a.tar.gz",  # v 0.7.1
        patch_args = ["-p1"],
        patches = [asphr_path + "//:rules_foreign_cc.0.7.1.patch"],  # from https://github.com/bazelbuild/rules_foreign_cc/issues/859#issuecomment-1058361769
    )

    _RULES_BOOST_COMMIT = "ef58870fe00ecb8047cd34324b8c21221387d5fc"

    http_archive(
        name = "com_github_nelhage_rules_boost",
        sha256 = "1557e4e1f2d009f14919dbf49b167f6616136d0cef1ca1cfada6ce0d4e3d6146",
        strip_prefix = "rules_boost-%s" % _RULES_BOOST_COMMIT,
        urls = [
            "https://github.com/nelhage/rules_boost/archive/%s.tar.gz" % _RULES_BOOST_COMMIT,
        ],
        patch_args = ["-p1"],
        patches = [asphr_path + "//:rules_boost.patch"],  # issue: https://github.com/nelhage/rules_boost/issues/160 (on certain linux distros, clang cannot find backtrace.h. see https://www.boost.org/doc/libs/1_71_0/doc/html/stacktrace/configuration_and_build.html#stacktrace.configuration_and_build.f3)
    )

    # To find additional information on this release or newer ones visit:
    # https://github.com/bazelbuild/rules_rust/releases
    http_archive(
        name = "rules_rust",
        sha256 = "872b04538ca20dad94791c348623f079ba93daf274c1d57ae6bfe0930ec77f0d",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/rules_rust/releases/download/0.6.0/rules_rust-v0.6.0.tar.gz",
            "https://github.com/bazelbuild/rules_rust/releases/download/0.6.0/rules_rust-v0.6.0.tar.gz",
        ],
    )

    http_archive(
        name = "cxx.rs",
        sha256 = "6fed9ef1c64a37c343727368b38c27fa4e15b27ca9924c6672a6a5496080c832",
        strip_prefix = "cxx-1.0.68",
        urls = ["https://github.com/dtolnay/cxx/archive/refs/tags/1.0.68.tar.gz"],
        patch_args = ["-p1"],
        patches = [asphr_path + "//:cxx.patch"],  # issue: we want to derive custom traits in Rust that C++ doesn't necessarily need to know about!
    )
