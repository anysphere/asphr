#
# Copyright 2022 Anysphere, Inc.
# SPDX-License-Identifier: GPL-3.0-only
#

"""Load asphr repos (part 5)."""

load("@rules_rust//crate_universe:defs.bzl", "crate", "crates_repository", "render_config")

def load_asphr_repos5():
    """Loads the remaining repositories for the asphr project (those that depend on load_asphr_repos4())."""

    crates_repository(
        name = "crate_index",
        lockfile = "//:Cargo.Bazel.lock",
        packages = {
            "diesel": crate.spec(
                version = "1.4.8",
                features = ["sqlite"]
            ),
            "cxx": crate.spec(
                version = "1.0"
            )
        },
        # Setting the default package name to `""` forces the use of the macros defined in this repository
        # to always use the root package when looking for dependencies or aliases. This should be considered
        # optional as the repository also exposes aliases for easy access to all dependencies.
        render_config = render_config(
            default_package_name = ""
        ),
    )
