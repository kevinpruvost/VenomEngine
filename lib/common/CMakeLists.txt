cmake_minimum_required(VERSION 3.25)

# Project name and version
project(VenomCommon VERSION 1.0)

# Define the static library target
file(GLOB_RECURSE venom_common_srcs
    "src/*.cc"
    "src/**/*.cc"
)
if (APPLE)
    # Add Objective-C++ source files
    file(GLOB_RECURSE venom_common_srcs_apple
        "src/*.mm"
        "src/*.m"
        "src/**/*.mm"
        "src/**/*.m"
    )
endif()
file(GLOB_RECURSE venom_common_hdrs 
    "include/venom/common/*.h"
    "include/venom/common/**/*.h"
)

add_library(${PROJECT_NAME} SHARED
        ${venom_common_srcs}
        ${venom_common_srcs_apple}
        ${venom_common_hdrs}
)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

set(venom_common_libraries
    assimp
    glm
    stb_image
    flecs::flecs
    OpenEXR::OpenEXR
    openvdb_static
    MikkTSpace
    IconFontCppHeaders
    flatbuffers
    lz4_static
)

# Skip GLFW & NFD on Iphone
if ((NOT APPLE) OR (NOT CMAKE_SYSTEM_NAME MATCHES "iOS"))
    set(venom_common_libraries ${venom_common_libraries} glfw nfd)
endif ()

if (APPLE)
    if (CMAKE_SYSTEM_NAME MATCHES "iOS")
        find_library(UIKIT_FRAMEWORK UIKit)
        list(APPEND venom_common_libraries ${UIKIT_FRAMEWORK})
    else ()
        find_library(COCOA_FRAMEWORK Cocoa)
        list(APPEND venom_common_libraries ${COCOA_FRAMEWORK})
    endif ()
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(METAL_FRAMEWORK Metal)
    find_library(METALKIT_FRAMEWORK MetalKit)
    find_library(QTZCORE_FRAMEWORK QuartzCore)
    # Game controller only doesnt work on watchOS
    find_library(GAMECONTROLLER_FRAMEWORK GameController)
    list(APPEND venom_common_libraries
        ${COREFOUNDATION_FRAMEWORK}
        ${METAL_FRAMEWORK}
        ${METALKIT_FRAMEWORK}
        ${QTZCORE_FRAMEWORK}
        ${GAMECONTROLLER_FRAMEWORK}
    )
endif ()

target_link_libraries(${PROJECT_NAME}
    ${venom_common_libraries}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    VENOM_COMMON_EXPORTS
    CMAKE_PROJECT_DIR="${CMAKE_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "include/"
)

set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_STYLE "AUTOMATIC")