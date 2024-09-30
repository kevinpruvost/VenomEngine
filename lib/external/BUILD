##
## GLFW
##

cc_shared_library(
    name = "glfw",
    exports_filter = [
        ":glfw_static",
        ":glfw_platform",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":glfw_platform",
        ":glfw_static",
    ],
)

cc_library(
    name = "glfw_static",
    srcs = [
        "glfw/src/context.c",
        "glfw/src/egl_context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/null_init.c",
        "glfw/src/null_joystick.c",
        "glfw/src/null_monitor.c",
        "glfw/src/null_window.c",
        "glfw/src/osmesa_context.c",
        "glfw/src/platform.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c",
    ],
    hdrs = [
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/internal.h",
        "glfw/src/mappings.h",
    ],
    # Macros: GLFW_BUILD_WIN32,GLFW_BUILD_COCOA,GLFW_BUILD_X11
    copts = select({
        "@platforms//os:windows": [
            "-D_GLFW_WIN32",
            "-D_GLFW_BUILD_DLL",
        ],
        "@platforms//os:linux": [
            "-D_GLFW_X11",
            #"-D_GLFW_WAYLAND",
            "-D_GLFW_BUILD_DLL",
        ],
        "@platforms//os:macos": [
            "-D_GLFW_COCOA",
            "-D_GLFW_BUILD_DLL",
        ],
    }),
    includes = ["glfw/include"],
    strip_include_prefix = "./glfw/",
    visibility = ["//visibility:public"],
    deps = [
        ":glfw_platform",
    ],
)

cc_library(
    name = "glfw_platform",
    srcs = select({
        "@platforms//os:windows": [
            "glfw/src/wgl_context.c",
            "glfw/src/win32_init.c",
            "glfw/src/win32_joystick.c",
            "glfw/src/win32_module.c",
            "glfw/src/win32_monitor.c",
            "glfw/src/win32_thread.c",
            "glfw/src/win32_time.c",
            "glfw/src/win32_window.c",
        ],
        "@platforms//os:linux": [
            "glfw/src/glx_context.c",
            "glfw/src/posix_thread.c",
            "glfw/src/posix_time.c",
            "glfw/src/x11_init.c",
            "glfw/src/x11_monitor.c",
            "glfw/src/x11_window.c",
            "glfw/src/xkb_unicode.c",
        ],
        "@platforms//os:macos": [
            "glfw/src/cocoa_init.m",
            "glfw/src/cocoa_joystick.m",
            "glfw/src/cocoa_monitor.m",
            "glfw/src/cocoa_time.c",
            "glfw/src/cocoa_window.m",
            "glfw/src/nsgl_context.m",
            "glfw/src/posix_thread.c",
        ],
    }),
    hdrs = [
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/internal.h",
        "glfw/src/null_joystick.h",
        "glfw/src/null_platform.h",
        "glfw/src/platform.h",
    ] + select({
        "@platforms//os:windows": [
            "glfw/src/win32_joystick.h",
            "glfw/src/win32_platform.h",
            "glfw/src/win32_thread.h",
            "glfw/src/win32_time.h",
        ],
        "@platforms//os:linux": [
            "glfw/src/glx_context.h",
            "glfw/src/posix_thread.h",
            "glfw/src/posix_time.h",
            "glfw/src/x11_init.h",
            "glfw/src/x11_monitor.h",
            "glfw/src/x11_platform.h",
            "glfw/src/x11_window.h",
            "glfw/src/xkb_unicode.h",
        ],
        "@platforms//os:macos": [
            "glfw/src/cocoa_joystick.h",
            "glfw/src/cocoa_monitor.h",
            "glfw/src/cocoa_platform.h",
            "glfw/src/cocoa_time.h",
            "glfw/src/cocoa_window.h",
            "glfw/src/nsgl_context.h",
            "glfw/src/posix_thread.h",
        ],
    }),
    # Macros: GLFW_BUILD_WIN32,GLFW_BUILD_COCOA,GLFW_BUILD_X11
    copts = select({
        "@platforms//os:windows": [
            "-D_GLFW_WIN32",
            "-D_GLFW_BUILD_DLL",
        ],
        "@platforms//os:linux": [
            "-D_GLFW_X11",
            #"-D_GLFW_WAYLAND",
            "-D_GLFW_BUILD_DLL",
        ],
        "@platforms//os:macos": [
            "-D_GLFW_COCOA",
            "-D_GLFW_BUILD_DLL",
        ],
    }),
    linkopts = select({
        "@platforms//os:windows": [
            "-DEFAULTLIB:gdi32.lib",
            "-DEFAULTLIB:shell32.lib",
            "-DEFAULTLIB:kernel32.lib",
            "-DEFAULTLIB:user32.lib",
        ],
        "//conditions:default": [],
    }),
    target_compatible_with = select({
        "@platforms//os:windows": ["@platforms//os:windows"],
        "@platforms//os:linux": ["@platforms//os:linux"],
        "@platforms//os:macos": ["@platforms//os:macos"],
    }),
    visibility = ["//visibility:public"],
    deps = [
    ],
)

##
## GLM
##

# Include directories for the header-only library
cc_library(
    name = "glm_header_only",
    hdrs = glob([
        "glm/glm/**/*.hpp",
        "glm/glm/**/*.inl",
        "glm/glm/**/*.natvis",
    ]),
    includes = ["glm"],
    strip_include_prefix = "./glm",
    visibility = ["//visibility:public"],
)

# Conditional library definition depending on GLM_BUILD_LIBRARY flag
cc_library(
    name = "glm_library",
    srcs = glob([
        "glm/glm/*.cpp",
        "glm/glm/detail/**/*.cpp",
        "glm/glm/ext/**/*.cpp",
        "glm/glm/gtc/**/*.cpp",
        "glm/glm/gtx/**/*.cpp",
        "glm/glm/simd/**/*.cpp",
    ]),
    hdrs = glob([
        "glm/glm/*.hpp",
        "glm/glm/*.inl",
        "glm/glm/detail/**/*.hpp",
        "glm/glm/detail/**/*.inl",
        "glm/glm/ext/**/*.hpp",
        "glm/glm/ext/**/*.inl",
        "glm/glm/gtc/**/*.hpp",
        "glm/glm/gtc/**/*.inl",
        "glm/glm/gtx/**/*.hpp",
        "glm/glm/gtx/**/*.inl",
        "glm/glm/simd/**/*.hpp",
        "glm/glm/simd/**/*.h",
    ]),
    strip_include_prefix = "./glm",
    visibility = ["//visibility:public"],
    deps = [":glm_header_only"],
)

# Alias for the glm::glm target
alias(
    name = "glm",
    actual = ":glm_library",
    visibility = ["//visibility:public"],
)

# VulkanMemoryAllocator

cc_library(
    name = "VulkanMemoryAllocator",
    srcs = [],
    hdrs = glob(["VulkanMemoryAllocator/include/*.h"]),
    includes = ["VulkanMemoryAllocator/include"],
    visibility = ["//visibility:public"],
    deps = [],
)

# stb_image

cc_library(
    name = "stb_image",
    srcs = ["stb_image/stb_vorbis.c"],
    hdrs = glob(["stb_image/*.h"]),
    includes = ["stb_image/"],
    visibility = ["//visibility:public"],
    deps = [],
)

# SPIRV-Reflect

cc_library(
    name = "SPIRVReflect",
    srcs = [
        "SPIRV-Reflect/spirv_reflect.cpp",
    ],
    hdrs = glob(["SPIRV-Reflect/*.h"]),
    includes = ["SPIRV-Reflect/"],
    visibility = ["//visibility:public"],
    deps = [],
)

# assimp

load("@bazel_skylib//rules:copy_file.bzl", "copy_file")

copy_file(
    name = "copy_assimp_config",
    src = "assimp/assimp_bazel/config.h",
    out = "assimp/assimp/include/assimp/config.h",
)

copy_file(
    name = "copy_assimp_revision",
    src = "assimp/assimp_bazel/revision.h",
    out = "assimp/assimp/include/assimp/revision.h",
)

copy_file(
    name = "copy_assimp_zconf",
    src = "assimp/assimp/contrib/zlib/zconf.h.in",
    out = "assimp/assimp/contrib/zlib/zconf.h",
)

cc_library(
    name = "assimp",
    srcs = glob([
        "assimp/assimp/code/AssetLib/3D/*.cpp",
        "assimp/assimp/code/AssetLib/3DS/*.cpp",
        "assimp/assimp/code/AssetLib/3MF/*.cpp",
        "assimp/assimp/code/AssetLib/AC/*.cpp",
        "assimp/assimp/code/AssetLib/AMF/*.cpp",
        "assimp/assimp/code/AssetLib/ASE/*.cpp",
        "assimp/assimp/code/AssetLib/ASSBIN/*.cpp",
        "assimp/assimp/code/AssetLib/ASSJSON/*.cpp",
        "assimp/assimp/code/AssetLib/ASSXML/*.cpp",
        "assimp/assimp/code/AssetLib/B3D/*.cpp",
        "assimp/assimp/code/AssetLib/BLEND/*.cpp",
        "assimp/assimp/code/AssetLib/BVH/*.cpp",
        "assimp/assimp/code/AssetLib/COB/*.cpp",
        "assimp/assimp/code/AssetLib/COLLADA/*.cpp",
        "assimp/assimp/code/AssetLib/CSM/*.cpp",
        "assimp/assimp/code/AssetLib/DXF/*.cpp",
        "assimp/assimp/code/AssetLib/FBX/*.cpp",
        "assimp/assimp/code/AssetLib/GLTF/*.cpp",
        "assimp/assimp/code/AssetLib/HMP/*.cpp",
        "assimp/assimp/code/AssetLib/IFC/IFCLoader.cpp",
        "assimp/assimp/code/AssetLib/IFC/IFCLoader.h",
        "assimp/assimp/code/AssetLib/IFC/IFCReaderGen1_2x3.cpp",
        "assimp/assimp/code/AssetLib/IFC/IFCReaderGen2_2x3.cpp",
        "assimp/assimp/code/AssetLib/IFC/IFCReaderGen_2x3.h",
        "assimp/assimp/code/AssetLib/IFC/IFCUtil.h",
        "assimp/assimp/code/AssetLib/IFC/IFCUtil.cpp",
        "assimp/assimp/code/AssetLib/IFC/IFCGeometry.cpp",
        "assimp/assimp/code/AssetLib/IFC/IFCMaterial.cpp",
        "assimp/assimp/code/AssetLib/IFC/IFCProfile.cpp",
        "assimp/assimp/code/AssetLib/IFC/IFCCurve.cpp",
        "assimp/assimp/code/AssetLib/IFC/IFCBoolean.cpp",
        "assimp/assimp/code/AssetLib/IFC/FCOpenings.cpp",
        "assimp/assimp/code/AssetLib/IQM/*.cpp",
        "assimp/assimp/code/AssetLib/IRR/*.cpp",
        "assimp/assimp/code/AssetLib/IRRMESH/*.cpp",
        "assimp/assimp/code/AssetLib/LWO/*.cpp",
        "assimp/assimp/code/AssetLib/LWS/*.cpp",
        "assimp/assimp/code/AssetLib/M3D/*.cpp",
        "assimp/assimp/code/AssetLib/MD2/*.cpp",
        "assimp/assimp/code/AssetLib/MD3/*.cpp",
        "assimp/assimp/code/AssetLib/MD5/*.cpp",
        "assimp/assimp/code/AssetLib/MDC/*.cpp",
        "assimp/assimp/code/AssetLib/MDL/*.cpp",
        "assimp/assimp/code/AssetLib/MMD/*.cpp",
        "assimp/assimp/code/AssetLib/MS3D/*.cpp",
        "assimp/assimp/code/AssetLib/NDO/*.cpp",
        "assimp/assimp/code/AssetLib/NFF/*.cpp",
        "assimp/assimp/code/AssetLib/OBJ/*.cpp",
        "assimp/assimp/code/AssetLib/OFF/*.cpp",
        "assimp/assimp/code/AssetLib/OGRE/*.cpp",
        "assimp/assimp/code/AssetLib/OPENGEX/*.cpp",
        "assimp/assimp/code/AssetLib/PLY/*.cpp",
        "assimp/assimp/code/AssetLib/Q3BSP/*.cpp",
        "assimp/assimp/code/AssetLib/Q3D/*.cpp",
        "assimp/assimp/code/AssetLib/RAW/*.cpp",
        "assimp/assimp/code/AssetLib/SIB/*.cpp",
        "assimp/assimp/code/AssetLib/SMD/*.cpp",
        "assimp/assimp/code/AssetLib/STEP/*.cpp",
        "assimp/assimp/code/AssetLib/STEPParser/*.cpp",
        "assimp/assimp/code/AssetLib/STL/*.cpp",
        "assimp/assimp/code/AssetLib/TERRAGEN/*.cpp",
        "assimp/assimp/code/AssetLib/X/*.cpp",
        "assimp/assimp/code/AssetLib/X3D/*.cpp",
        "assimp/assimp/code/AssetLib/XGL/*.cpp",
        "assimp/assimp/code/CApi/**/*.cpp",
        "assimp/assimp/code/Common/**/*.cpp",
        "assimp/assimp/code/Geometry/**/*.cpp",
        "assimp/assimp/code/Material/**/*.cpp",
        "assimp/assimp/code/PostProcessing/**/*.cpp",
        "assimp/assimp/contrib/zlib/*.c",
        "assimp/assimp/contrib/zlib/*.h",
        "assimp/assimp/contrib/clipper/*.cpp",
        "assimp/assimp/contrib/Open3DGC/*.cpp",
        "assimp/assimp/contrib/poly2tri/**/*.cc",
        "assimp/assimp/contrib/pugixml/src/*.cpp",
        "assimp/assimp/contrib/unzip/*.c",
        "assimp/assimp/contrib/pugixml/*.cpp",
        "assimp/assimp/contrib/clipper/*.hpp",
        "assimp/assimp/contrib/Open3DGC/*.h",
        "assimp/assimp/contrib/openddlparser/include/openddlparser/*.h",
        "assimp/assimp/contrib/poly2tri/**/*.h",
        "assimp/assimp/contrib/pugixml/src/*.h",
        "assimp/assimp/contrib/stb/stb_image.h",
        "assimp/assimp/contrib/pugixml/*.hpp",
        "assimp/assimp/contrib/unzip/*.h",
        "assimp/assimp/code/CApi/**/*.h",
        "assimp/assimp/code/Common/**/*.h",
        "assimp/assimp/code/Geometry/**/*.h",
        "assimp/assimp/code/Material/**/*.h",
        "assimp/assimp/code/PostProcessing/**/*.h",
    ]) + [
        ":copy_assimp_config",
        ":copy_assimp_revision",
        ":copy_assimp_zconf",
    ],
    hdrs = glob([
        "assimp/assimp/include/assimp/Compiler/*.h",
        "assimp/assimp/include/assimp/*.hpp",
        "assimp/assimp/include/assimp/*.h",
    ]),
    copts = select({
        "@platforms//os:windows": [
            "-D_SCL_SECURE_NO_WARNINGS",
            "-D_CRT_SECURE_NO_WARNINGS",
        ],
        "@platforms//os:linux": [
        ],
        "@platforms//os:macos": [
            "-D_ENABLE_BITCODE",
        ],
    }) + [
        "-DASSIMP_BUILD_NO_EXPORT",
        "-DASSIMP_BUILD_DLL_EXPORT",
        "-DASSIMP_BUILD_NO_IFC_IMPORTER",
        "-DASSIMP_BUILD_NO_USD_IMPORTER",
        #"-DASSIMP_DOUBLE_PRECISION",
    ],
    includes = [
        "assimp/assimp/",
        "assimp/assimp/code/",
        "assimp/assimp/contrib/",
        "assimp/assimp/contrib/openddlparser/include/",
        "assimp/assimp/contrib/pugixml/src/",
        "assimp/assimp/contrib/rapidjson/include/",
        "assimp/assimp/contrib/stb/",
        "assimp/assimp/contrib/unzip/",
        "assimp/assimp/contrib/utf8cpp/source/",
        "assimp/assimp/contrib/zlib/",
        "assimp/assimp/include/",
    ],
    visibility = ["//visibility:public"],
    deps = [
        #"@zlib",
    ],
)
