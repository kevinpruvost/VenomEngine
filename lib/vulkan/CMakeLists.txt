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
add_library(${PROJECT_NAME} SHARED
    ${venom_vulkan_srcs} ${venom_vulkan_hdrs})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

# Find Vulkan
find_package(Vulkan REQUIRED) # error

if (APPLE)
# Copy vulkan in binary dir for packaging
# Gets all libvulkan.*.dylib
file(GLOB_RECURSE VULKAN_LIBRARY $ENV{VULKAN_SDK}/lib/libvulkan.*.dylib)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${VULKAN_LIBRARY} ${CMAKE_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
    VenomCommon   # Assuming venom_common_static is defined elsewhere
    glm
    Vulkan::Vulkan
)

else ()
# Link dependencies to the static library
target_link_libraries(${PROJECT_NAME}
    VenomCommon   # Assuming venom_common_static is defined elsewhere
    glm
    Vulkan::Vulkan
)
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC
    ${VULKAN_INCLUDE_DIRS}
)

# Installation
if (APPLE)
    install(TARGETS ${PROJECT_NAME}
            DESTINATION ${CMAKE_INSTALL_PREFIX}/VenomEngine.app/Contents/Frameworks
    )

    install(FILES ${VULKAN_LIBRARY}
            DESTINATION ${CMAKE_INSTALL_PREFIX}/VenomEngine.app/Contents/Frameworks
    )

    set(CPACK_BUNDLE_APPLE_CODESIGN_FILES
        ${CPACK_BUNDLE_APPLE_CODESIGN_FILES}
        ${CMAKE_INSTALL_PREFIX}/libvulkan.1.dylib
        ${CMAKE_INSTALL_PREFIX}/libvulkan.1.3.290.dylib
        ${CMAKE_INSTALL_PREFIX}/VenomVulkan.dylib
    )
endif()