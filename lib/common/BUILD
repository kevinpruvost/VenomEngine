cc_library(
    name = "venom_common_static",
    srcs = glob(["src/*.cc"]),
    hdrs = glob([
        "include/venom/common/*.h",
        "include/venom/common/**/*.h",
    ]),
    defines = [
        "VENOM_COMMON_EXPORTS",
    ],
    includes = [
        "include",
        "//lib/common",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lib/external:assimp",
        "//lib/external:glfw_static",
        "//lib/external:glm",
        "//lib/external:stb_image",
        "@bazel_tools//tools/cpp/runfiles",
    ],
)

cc_shared_library(
    name = "VenomCommon",
    dynamic_deps = [
        "//lib/external:glfw",
        "//lib/external:assimp_shared",
    ],
    exports_filter = [
        "//lib/external:glfw",
        "//lib/external:glm_library",
        "@bazel_tools//tools/cpp/runfiles",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":venom_common_static",
    ],
)
