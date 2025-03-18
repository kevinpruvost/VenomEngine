# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.25)

project("VenomEngineWorkspace")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_DEBUG_POSTFIX "")

set(PROJECT_PATH ${CMAKE_SOURCE_DIR}/)
set(EXTERNAL_LIBS_PATH ${CMAKE_SOURCE_DIR}/lib/external/)
set(LIBS_PATH ${CMAKE_SOURCE_DIR}/lib/)
set(GRAPHICS_DLL_PROPS_PATH ${CMAKE_SOURCE_DIR}/GraphicsDLLProps.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_SHARED_LIBRARY_VERSION_STRING "")
set(CMAKE_SHARED_LIBRARY_SOVERSION "")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# XCode chqnges binary dir depending on the configuration
set(CMAKE_INSTALL_RPATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}" "${CMAKE_BINARY_DIR}/Debug")
#set(CMAKE_INSTALL_RPATH "@executable_path/Frameworks")
message(STATUS "RPATH: ${CMAKE_INSTALL_RPATH}")

set(VENOM_RUNTIME_LOADED_DLLS "" CACHE INTERNAL "Venom runtime loaded DLLs")

# Only with .hlsl, .glsl, .metal, .metallib, .exr, .hdri, .ttf, .otf, .spv, .icns, .png, .jpg, .jpeg, .fbx, .obj, .mtl, .tiff
file(GLOB_RECURSE RESOURCE_FILES
    "${CMAKE_SOURCE_DIR}/resources/**/*.hlsl"
    "${CMAKE_SOURCE_DIR}/resources/**/*.glsl"
    "${CMAKE_SOURCE_DIR}/resources/**/*.metal"
    "${CMAKE_SOURCE_DIR}/resources/**/*.metallib"
    "${CMAKE_SOURCE_DIR}/resources/**/*.exr"
    "${CMAKE_SOURCE_DIR}/resources/**/*.hdri"
    "${CMAKE_SOURCE_DIR}/resources/**/*.ttf"
    "${CMAKE_SOURCE_DIR}/resources/**/*.otf"
    "${CMAKE_SOURCE_DIR}/resources/**/*.spv"
    "${CMAKE_SOURCE_DIR}/resources/**/*.png"
    "${CMAKE_SOURCE_DIR}/resources/**/*.jpg"
    "${CMAKE_SOURCE_DIR}/resources/**/*.jpeg"
    "${CMAKE_SOURCE_DIR}/resources/**/*.fbx"
    "${CMAKE_SOURCE_DIR}/resources/**/*.gltf"
    "${CMAKE_SOURCE_DIR}/resources/**/*.glb"
    "${CMAKE_SOURCE_DIR}/resources/**/*.obj"
    "${CMAKE_SOURCE_DIR}/resources/**/*.mtl"
    "${CMAKE_SOURCE_DIR}/resources/**/*.tiff"

    "${CMAKE_SOURCE_DIR}/resources/**/*.venomasset"
)

if (APPLE)
    enable_language(OBJC OBJCXX)

    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    if (NOT CMAKE_SYSTEM_NAME MATCHES "iOS")
        set(CMAKE_XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/../Frameworks @rpath")
    else ()
        set(CMAKE_XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/Frameworks @rpath")
    endif ()

    # Needed because the Apple Toolchain sets these vars to "NO", which causes packaging for iOS or other devices to fail
    # because they need to be signed
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES")
    # Needed to make CMake able to find the dependencies to link and bundle
    set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY TRUE)

    # Add Compile definitions to recognize ios or macos
    if (CMAKE_SYSTEM_NAME MATCHES "iOS")
        add_compile_definitions(TARGET_OS_IOS)
        set(CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2") # 1 = iPhone, 2 = iPad
    else()
        add_compile_definitions(TARGET_OS_OSX)
    endif ()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # AddressSanitizer for macOS
        message(STATUS "Enabling AddressSanitizer on macOS in Debug mode")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
    endif()

    if (CMAKE_SYSTEM_NAME MATCHES "iOS")
        set(CMAKE_OSX_DEPLOYMENT_TARGET "18.0")
    else()
        set(CMAKE_OSX_DEPLOYMENT_TARGET "15")
    endif ()
    set(XCODE_VERSION "16.0")

    include(./CMake/ios.toolchain_extended.cmake)
endif()

message(STATUS "Resource files: ${RESOURCE_FILES}")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Only apply this for macOS and Linux platforms
if(APPLE OR UNIX)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif()

# Libraries
add_subdirectory(${LIBS_PATH})

# Resources
set(SOURCE_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
# Depends on current config

set(DEST_RESOURCES_DIR "${CMAKE_BINARY_DIR}/resources")
message(STATUS "Resources directory: ${DEST_RESOURCES_DIR}")

# Though we need admin privilege on windows to make that work, so fallback on copying resources
if(WIN32)
    # Try creating the symbolic link
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${SOURCE_RESOURCES_DIR}" "${DEST_RESOURCES_DIR}"
        RESULT_VARIABLE symlink_result
        ERROR_QUIET
    )

    # Check if the symlink creation was successful
    if(NOT symlink_result EQUAL 0)
        message(WARNING "Symlink creation failed. Falling back to copying resources.")
        
        # Fallback: copy the directory instead
        add_custom_target(link_resources ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE_RESOURCES_DIR}" "${DEST_RESOURCES_DIR}"
            COMMENT "Copying resources folder to build directory"
        )
    else()
        add_custom_target(link_resources ALL
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${SOURCE_RESOURCES_DIR}" "${DEST_RESOURCES_DIR}"
            COMMENT "Linking resources folder to build directory"
        )
    endif()
else()
    # For non-Windows platforms, use symlink directly
    add_custom_target(link_resources ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${SOURCE_RESOURCES_DIR}" "${DEST_RESOURCES_DIR}"
        COMMENT "Linking resources folder to build directory"
    )
endif()

# Executable
add_subdirectory(${PROJECT_PATH}/VenomEngine)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
