workspace(
    name = "VenomEngineWorkspace",
)

# Load platforms
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",
    strip_prefix = "bazel-compile-commands-extractor-0e990032f3c5a866e72615cf67e5ce22186dcb97",

    # Replace the commit hash (0e990032f3c5a866e72615cf67e5ce22186dcb97) in both places (below) with the latest (https://github.com/hedronvision/bazel-compile-commands-extractor/commits/main), rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/0e990032f3c5a866e72615cf67e5ce22186dcb97.tar.gz",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()

load("@hedron_compile_commands//:workspace_setup_transitive.bzl", "hedron_compile_commands_setup_transitive")

hedron_compile_commands_setup_transitive()

load("@hedron_compile_commands//:workspace_setup_transitive_transitive.bzl", "hedron_compile_commands_setup_transitive_transitive")

hedron_compile_commands_setup_transitive_transitive()

load("@hedron_compile_commands//:workspace_setup_transitive_transitive_transitive.bzl", "hedron_compile_commands_setup_transitive_transitive_transitive")

hedron_compile_commands_setup_transitive_transitive_transitive()

# FMT library
git_repository(
    name = "fmt",
    branch = "master",
    patch_cmds = [
        "mv support/bazel/.bazelrc .bazelrc",
        "mv support/bazel/.bazelversion .bazelversion",
        "mv support/bazel/BUILD.bazel BUILD.bazel",
        "mv support/bazel/WORKSPACE.bazel WORKSPACE.bazel",
    ],
    # Windows related patch commands are only needed in the case MSYS2 is not installed
    patch_cmds_win = [
        "Move-Item -Path support/bazel/.bazelrc -Destination .bazelrc",
        "Move-Item -Path support/bazel/.bazelversion -Destination .bazelversion",
        "Move-Item -Path support/bazel/BUILD.bazel -Destination BUILD.bazel",
        "Move-Item -Path support/bazel/WORKSPACE.bazel -Destination WORKSPACE.bazel",
    ],
    remote = "https://github.com/fmtlib/fmt",
)

git_repository(
    name = "rules_vulkan",
    remote = "https://github.com/jadarve/rules_vulkan.git",
    tag = "v0.0.11",
)

load("@rules_vulkan//vulkan:repositories.bzl", "vulkan_repositories")

# Print environment variables for debugging
# print("DEBUG: BAZEL_VERSION = " + PATH)

vulkan_repositories()

# Bazel Skylib

http_archive(
    name = "bazel_skylib",
    sha256 = "bc283cdfcd526a52c3201279cda4bc298652efa898b10b4db0837dc51652756f",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.7.1/bazel-skylib-1.7.1.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.7.1/bazel-skylib-1.7.1.tar.gz",
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

# Bazel Gazelle

http_archive(
    name = "bazel_skylib_gazelle_plugin",
    sha256 = "e0629e3cbacca15e2c659833b24b86174d22b664ca0a67f377108ff6a207cc8c",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.7.1/bazel-skylib-gazelle-plugin-1.7.1.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.7.1/bazel-skylib-gazelle-plugin-1.7.1.tar.gz",
    ],
)

load("@bazel_skylib_gazelle_plugin//:workspace.bzl", "bazel_skylib_gazelle_plugin_workspace")

bazel_skylib_gazelle_plugin_workspace()

load("@bazel_skylib_gazelle_plugin//:setup.bzl", "bazel_skylib_gazelle_plugin_setup")

bazel_skylib_gazelle_plugin_setup()

# Bazel rules_foreign_cc (https://bazel-contrib.github.io/rules_foreign_cc/main/cmake.html)

http_archive(
    name = "rules_foreign_cc",
    sha256 = "a2e6fb56e649c1ee79703e99aa0c9d13c6cc53c8d7a0cbb8797ab2888bbc99a3",
    strip_prefix = "rules_foreign_cc-0.12.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/releases/download/0.12.0/rules_foreign_cc-0.12.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.12.0/flatten.html#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

load("@bazel_features//:deps.bzl", "bazel_features_deps")

bazel_features_deps()

# MacOS specific extensions
load(
    "@build_bazel_rules_apple//apple:repositories.bzl",
    "apple_rules_dependencies",
)

apple_rules_dependencies()

load(
    "@build_bazel_rules_swift//swift:repositories.bzl",
    "swift_rules_dependencies",
)

swift_rules_dependencies()

load(
    "@build_bazel_rules_swift//swift:extras.bzl",
    "swift_rules_extra_dependencies",
)

swift_rules_extra_dependencies()

load(
    "@build_bazel_apple_support//lib:repositories.bzl",
    "apple_support_dependencies",
)

apple_support_dependencies()
