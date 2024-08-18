# Include directories for the header-only library
cc_library(
    name = "glm_header_only",
    hdrs = glob([
        "glm/**/*.hpp",
        "glm/**/*.inl",
        "glm/**/*.natvis",
    ]),
    includes = ["."],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
)

# Conditional library definition depending on GLM_BUILD_LIBRARY flag
cc_library(
    name = "glm_library",
    srcs = glob([
        "glm/*.cpp",
        "glm/detail/**/*.cpp",
        "glm/ext/**/*.cpp",
        "glm/gtc/**/*.cpp",
        "glm/gtx/**/*.cpp",
        "glm/simd/**/*.cpp",
    ]),
    hdrs = glob([
        "glm/*.hpp",
        "glm/*.inl",
        "glm/detail/**/*.hpp",
        "glm/detail/**/*.inl",
        "glm/ext/**/*.hpp",
        "glm/ext/**/*.inl",
        "glm/gtc/**/*.hpp",
        "glm/gtc/**/*.inl",
        "glm/gtx/**/*.hpp",
        "glm/gtx/**/*.inl",
        "glm/simd/**/*.hpp",
        "glm/simd/**/*.h",
    ]),
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [":glm_header_only"],
)

# Alias for the glm::glm target
alias(
    name = "glm",
    actual = ":glm_library",
    visibility = ["//visibility:public"],
)
