cmake_minimum_required(VERSION 3.10)

# Project name and version
project(VenomVulkan VERSION 1.0)

# Define the include directories
include_directories(include)

# Define platform-specific compile options
if(WIN32)
    add_compile_definitions(GLFW_DLL)
    add_compile_definitions(_GLFW_WIN32)
elseif(UNIX AND NOT APPLE) # Linux
    add_compile_definitions(GLFW_DLL)
    add_compile_definitions(_GLFW_X11)
    # Uncomment if Wayland support is desired
    # add_compile_definitions(_GLFW_WAYLAND)
elseif(APPLE)
    add_compile_definitions(GLFW_DLL)
    add_compile_definitions(_GLFW_COCOA)
endif()

# Define the source and header files
file(GLOB_RECURSE venom_vulkan_srcs src/*.cc)
file(GLOB_RECURSE venom_vulkan_hdrs include/venom/vulkan/**/**/**/*.h)

# Create the static library target
add_library(${PROJECT_NAME} SHARED ${venom_vulkan_srcs} ${venom_vulkan_hdrs})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

# Find Vulkan
find_package(Vulkan REQUIRED) # error

# Link dependencies to the static library
target_link_libraries(${PROJECT_NAME}
    VenomCommon   # Assuming venom_common_static is defined elsewhere
    glm
    Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${VULKAN_INCLUDE_DIRS}
)