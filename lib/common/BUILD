cc_library(
    name = "venom_common_static",
    srcs = glob(["*.cc"]),
    hdrs = glob(["include/common/*.h"]),
    defines = [
        "VENOM_COMMON_EXPORTS",
    ],
    includes = [
        "include",
        "//lib/common",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lib/GLFW:glfw_static",
    ],
)

cc_shared_library(
    name = "VenomCommon",
    dynamic_deps = [
        "//lib/GLFW:glfw",
    ],
    exports_filter = [
        ":venom_common_static",
        "//lib/GLFW:glfw",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":venom_common_static",
    ],
)
