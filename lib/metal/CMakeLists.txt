cmake_minimum_required(VERSION 3.10)

# Project name and version
project(VenomMetal VERSION 1.0 LANGUAGES C CXX OBJC OBJCXX)

set(VENOM_RUNTIME_LOADED_DLLS "${VENOM_RUNTIME_LOADED_DLLS};${PROJECT_NAME}" CACHE INTERNAL "Venom runtime loaded DLLs")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Define the include directories
include_directories(include)

# Define platform-specific compile options
add_compile_definitions(GLFW_DLL)
add_compile_definitions(_GLFW_COCOA)

# Define the source and header files
file(GLOB_RECURSE venom_metal_srcs src/*.cc src/*.mm)
file(GLOB_RECURSE venom_metal_hdrs include/venom/metal/**/**/**/*.h)
message(STATUS "Venom Metal Sources: ${venom_metal_srcs}")

# Create the static library target
add_library(${PROJECT_NAME} SHARED
    ${venom_metal_srcs} ${venom_metal_hdrs}
    ${IMGUI_SOURCES}
    ${IMGUI_METAL_SOURCE}
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${venom_context_apple_include_dirs}
)
# If Target is not iOS, then add glfw
if (NOT CMAKE_SYSTEM_NAME MATCHES "iOS")
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${venom_context_glfw_include_dirs}
    )
endif ()
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    target_sources(${PROJECT_NAME} PUBLIC
        ${IMGUI_OSX_SOURCE}
    )
endif ()
set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    OBJCXX_STANDARD ${CMAKE_CXX_STANDARD}
    #COMPILE_FLAGS "-fno-objc-arc" # very important
)
message(STATUS "IMGUI Metal Sources: ${IMGUI_SOURCES}")

# Find required libraries and frameworks
find_library(METAL_FRAMEWORK Metal)
find_library(METALKIT_FRAMEWORK MetalKit)
find_library(QUARTZCORE_FRAMEWORK QuartzCore)
find_library(FOUNDATION_FRAMEWORK Foundation)
find_library(COREGRAPHICS_FRAMEWORK CoreGraphics)
find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)

target_link_libraries(${PROJECT_NAME}
    VenomCommon   # Assuming venom_common_static is defined elsewhere
    glm
    ${METAL_FRAMEWORK}
    ${METALKIT_FRAMEWORK}
    ${QUARTZCORE_FRAMEWORK}
    ${FOUNDATION_FRAMEWORK}
    ${COREGRAPHICS_FRAMEWORK}
    ${COREFOUNDATION_FRAMEWORK}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${VULKAN_INCLUDE_DIRS}
    ${IMGUI_INCLUDE_DIR}
)

# Installation
if (APPLE)
    install(TARGETS ${PROJECT_NAME}
            DESTINATION ${CMAKE_INSTALL_PREFIX}/VenomEngine.app/Contents/Frameworks
    )

#    set(CPACK_BUNDLE_APPLE_CODESIGN_FILES
#        ${CPACK_BUNDLE_APPLE_CODESIGN_FILES}
#        ${CMAKE_INSTALL_PREFIX}/libvulkan.1.dylib
#        ${CMAKE_INSTALL_PREFIX}/libvulkan.1.3.290.dylib
#        ${CMAKE_INSTALL_PREFIX}/VenomMetal.dylib
#    )
endif()