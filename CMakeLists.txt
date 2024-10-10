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

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# XCode chqnges binary dir depending on the configuration
set(CMAKE_INSTALL_RPATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}" "${CMAKE_BINARY_DIR}/Debug")
message(STATUS "RPATH: ${CMAKE_INSTALL_RPATH}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Only with .hlsl, .glsl, .spv, .icns, .png, .jpg, .jpeg, .fbx, .obj, .mtl, .tiff
file(GLOB_RECURSE RESOURCE_FILES
    "${CMAKE_SOURCE_DIR}/resources/**/*.hlsl"
    "${CMAKE_SOURCE_DIR}/resources/**/*.glsl"
    "${CMAKE_SOURCE_DIR}/resources/**/*.spv"
    "${CMAKE_SOURCE_DIR}/resources/**/*.png"
    "${CMAKE_SOURCE_DIR}/resources/**/*.png"
    "${CMAKE_SOURCE_DIR}/resources/**/*.jpg"
    "${CMAKE_SOURCE_DIR}/resources/**/*.jpeg"
    "${CMAKE_SOURCE_DIR}/resources/**/*.fbx"
    "${CMAKE_SOURCE_DIR}/resources/**/*.obj"
    "${CMAKE_SOURCE_DIR}/resources/**/*.mtl"
    "${CMAKE_SOURCE_DIR}/resources/**/*.tiff"
)

if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
    file(GLOB_RECURSE RESOURCE_FILES_APPLE
        "${CMAKE_SOURCE_DIR}/resources/**/*.icns"
        "${CMAKE_SOURCE_DIR}/resources/*.icns"
    )
    list(APPEND RESOURCE_FILES ${RESOURCE_FILES_APPLE})

    # Path to the code signing identity file
    set(CODESIGN_IDENTITY_FILE "${PROJECT_SOURCE_DIR}/Apple/codesign_identity.txt")

    # Configure the code signing identity file from a template only if it does not exist
    if (NOT EXISTS ${CODESIGN_IDENTITY_FILE})
        configure_file(${CODESIGN_IDENTITY_FILE}.in ${CODESIGN_IDENTITY_FILE} @ONLY)
    endif()

    # Read the code signing identity from the file
    file(READ ${CODESIGN_IDENTITY_FILE} CODESIGN_IDENTITY_CONTENTS)
    string(REGEX MATCH "CODE_SIGN_IDENTITY=(.*)$" _unused "${CODESIGN_IDENTITY_CONTENTS}")
    set(CODESIGN_IDENTITY "${CMAKE_MATCH_1}")

    # Ensure the codesign identity is found
    if(NOT CODESIGN_IDENTITY OR CODESIGN_IDENTITY STREQUAL "@CODE_SIGN_IDENTITY@")
        message(FATAL_ERROR "Code signing identity not specified. Please update Apple/codesign_identity.txt")
    else()
        message(STATUS "Code signing identity: ${CODESIGN_IDENTITY}")
    endif()

    # Get codesign location
    find_program(CODESIGN_EXECUTABLE codesign)
    # Define code-signing command
    set(CODESIGN_COMMAND ${CODESIGN_EXECUTABLE} --force --verify --verbose --sign ${CODESIGN_IDENTITY})
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