# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.25)

project("VenomEngine")

set(CMAKE_CXX_STANDARD 23)

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