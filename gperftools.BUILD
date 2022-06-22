# load("@rules_foreign_cc_airbnb//tools/build_defs:configure.bzl", "configure_make")
load("@rules_foreign_cc//foreign_cc:defs.bzl", "configure_make")
load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//:__pkg__"],
)

# TODO: currently requires autoconf: `sudo apt install autoconf`. Make it independant of that.
configure_make(
    name = "gperftools_build",
    configure_options = [
        "--enable-shared=no",
        "--enable-frame-pointers",
        "--disable-libunwind",
    ],
    configure_in_place = True,
    # autogen = True,
    autoreconf = True,
    lib_source = ":all_srcs",
    linkopts = ["-lpthread"],
    args = ["install-libLTLIBRARIES", "install-perftoolsincludeHEADERS"], # the list of arguments passed to the make commands.
    out_static_libs = select({
        # "//bazel:debug_tcmalloc": ["libtcmalloc_debug.a"],
        "//conditions:default": ["libtcmalloc_and_profiler.a"],
    }),
)

# Workaround for https://github.com/bazelbuild/rules_foreign_cc/issues/227

cc_library(
    name = "gperftools",
    deps = [
        "gperftools_build",
    ],
)
