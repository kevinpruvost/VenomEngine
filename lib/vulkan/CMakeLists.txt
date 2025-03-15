cmake_minimum_required(VERSION 3.10)

# Project name and version
project(VenomVulkan VERSION 1.0)

set(VENOM_RUNTIME_LOADED_DLLS "${VENOM_RUNTIME_LOADED_DLLS};${PROJECT_NAME}" CACHE INTERNAL "Venom runtime loaded DLLs")

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

if (APPLE)
    file(GLOB_RECURSE venom_vulkan_srcs_apple src/*.m src/*.mm src/**/*.m src/**/*.mm)
    set(venom_vulkan_srcs ${venom_vulkan_srcs} ${venom_vulkan_srcs_apple})
endif ()

# Create the static library target
add_library(${PROJECT_NAME} SHARED
    ${venom_vulkan_srcs} ${venom_vulkan_hdrs}
    ${IMGUI_SOURCES}
    ${IMGUI_VULKAN_SOURCE}
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${venom_context_glfw_include_dirs}
)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
message(STATUS "IMGUI Sources: ${IMGUI_SOURCES}")

if (CMAKE_SYSTEM_NAME MATCHES "iOS")
    # Ensure CMake looks for Vulkan in the iOS SDK directory
    set(VULKAN_SDK_PATH "$ENV{VULKAN_SDK}/../iOS")
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
else()
    set(VULKAN_SDK_PATH "$ENV{VULKAN_SDK}/../macOS")
    # Also add imgui osx source
    target_sources(${PROJECT_NAME} PRIVATE
        ${IMGUI_OSX_SOURCE}
    )
endif()

# Override CMAKE_PREFIX_PATH to make sure CMake looks in the right location
set(CMAKE_PREFIX_PATH "${VULKAN_SDK_PATH}" CACHE PATH "Path to Vulkan SDK")

# Find Vulkan
if (CMAKE_SYSTEM_NAME MATCHES "iOS")
    find_package(Vulkan REQUIRED COMPONENTS MoltenVK)
else ()
    find_package(Vulkan REQUIRED) # error
endif ()

# show where the libraries are
message(STATUS "Vulkan libraries: ${Vulkan_LIBRARIES}")

set(Vulkan_lib Vulkan::Vulkan)
if (APPLE)
    if (CMAKE_SYSTEM_NAME MATCHES "iOS")
        target_sources(${PROJECT_NAME} PRIVATE
                $ENV{VULKAN_SDK}/../iOS/share/vulkan
        )
        set_source_files_properties($ENV{VULKAN_SDK}/../iOS/share/vulkan PROPERTIES
                MACOSX_PACKAGE_LOCATION Resources
        )
        set(VENOM_RUNTIME_LOADED_DLLS "${VENOM_RUNTIME_LOADED_DLLS};${Vulkan_LIBRARIES};${Vulkan_MoltenVK_LIBRARY};${Vulkan_Layer_VALIDATION};${Vulkan_Layer_SHADER_OBJECT};${Vulkan_Layer_SYNC2}" CACHE INTERNAL "Venom runtime loaded DLLs")
    elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        target_sources(${PROJECT_NAME} PRIVATE
                $ENV{VULKAN_SDK}/../macOS/share/vulkan
        )
        set_source_files_properties($ENV{VULKAN_SDK}/../macOS/share/vulkan PROPERTIES
                MACOSX_PACKAGE_LOCATION Resources
        )
        # Different path because there is a useless symlink in macOS version
        set(temp_libs "$ENV{VULKAN_SDK}/../macOS/lib/libvulkan.${Vulkan_VERSION}.dylib")
        # Same for MoltenVK it should not link the framework but the dylib instead
        list(APPEND temp_libs "$ENV{VULKAN_SDK}/../macOS/lib/libMoltenVK.dylib")
        # Then add the other layer libs
        list(APPEND temp_libs "${Vulkan_Layer_VALIDATION}")
        list(APPEND temp_libs "${Vulkan_Layer_SHADER_OBJECT}")
        list(APPEND temp_libs "${Vulkan_Layer_SYNC2}")
        set(Vulkan_Framework_Libs ${temp_libs} CACHE INTERNAL "Vulkan Framework Libs")
    endif ()
    message(STATUS "VULKAN_SDK: ${VULKAN_SDK}")
endif ()

# Show Vulkan libraries paths
message(STATUS "Venom Runtime Loaded Dlls in Vulkan: ${VENOM_RUNTIME_LOADED_DLLS}")

#if (APPLE)
#    # Copy vulkan in binary dir for packaging
#    # Gets all libvulkan.*.dylib
#    file(GLOB_RECURSE VULKAN_LIBRARY $ENV{VULKAN_SDK}/lib/libvulkan.*.dylib)
#    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy ${VULKAN_LIBRARY} ${CMAKE_BINARY_DIR}
#    )
#    # if VULKAN_LIBRARY is empty, then error
#    if (NOT VULKAN_LIBRARY)
#        message(FATAL_ERROR "Vulkan library not found, check your environment variables")
#    endif ()
#
#else ()

#endif ()

target_link_libraries(${PROJECT_NAME}
    VenomCommon   # Assuming venom_common_static is defined elsewhere
    glm
    ${Vulkan_lib}
)

message(STATUS "Vulkan include dir: ${Vulkan_INCLUDE_DIRS}")
target_include_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_INCLUDE_DIRS}
    ${IMGUI_INCLUDE_DIR}
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