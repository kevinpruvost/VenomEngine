cc_library(
    name = "venom_common_static",
    srcs = glob(["src/*.cc"]),
    hdrs = glob(["include/venom/common/*.h"]),
    defines = [
        "VENOM_COMMON_EXPORTS",
    ],
    includes = [
        "include",
        "//lib/common",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lib/external:glfw_static",
        "@bazel_tools//tools/cpp/runfiles",
    ],
)

cc_shared_library(
    name = "VenomCommon",
    dynamic_deps = [
        "//lib/external:glfw",
    ],
    exports_filter = [
        ":venom_common_static",
        "//lib/external:glfw",
        "@bazel_tools//tools/cpp/runfiles",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":venom_common_static",
    ],
)
