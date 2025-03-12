cmake_minimum_required(VERSION 3.20)

##
## CODESIGN_IDENTITY_FILE
##
## DO NOT FORGET TO CONFIGURE THE APPROPRIATE FILES
## CMake/codesign_identity.txt.in -> CMake/codesign_identity.txt
## Replace the variables with the appropriate values depending on your Apple Dev account
##

set(CODESIGN_IDENTITY_FILE "${PROJECT_SOURCE_DIR}/CMake/codesign_identity.txt")

# Configure the code signing identity file from a template only if it does not exist
if (NOT EXISTS ${CODESIGN_IDENTITY_FILE})
    # If the user has not given the variables CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM and CODE_SIGN_IDENTITY then error
    if (NOT DEFINED CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM)
        message(FATAL_ERROR "CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM & CODE_SIGN_IDENTITY not specified. Please give them as arguments to CMake with 'cmake -DCODE_SIGN_IDENTITY=? -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=?
            or update Apple/codesign_identity.txt
            It should look like this: CODESIGN_IDENTITY=\"Apple Development: Your Name (TEAM_ID)\"
                                      CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=8ZAWEASDZQC")
    endif ()
    configure_file(${CODESIGN_IDENTITY_FILE}.in ${CODESIGN_IDENTITY_FILE} @ONLY)
endif()

# Read the contents of the file
file(READ ${CODESIGN_IDENTITY_FILE} CODESIGN_IDENTITY_CONTENTS)

# Extract CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM
string(REGEX MATCH "CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=\"?([^\"]+)\"?" _unused "${CODESIGN_IDENTITY_CONTENTS}")
set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${CMAKE_MATCH_1}")

message(STATUS "Development team: ${CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM}")

# Macro to set XCode properties
macro(set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE XCODE_RELVERSION)
    set(XCODE_RELVERSION_I "${XCODE_RELVERSION}")
    if(XCODE_RELVERSION_I STREQUAL "All")
        set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} "${XCODE_VALUE}")
    else()
        set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}[variant=${XCODE_RELVERSION_I}] "${XCODE_VALUE}")
    endif()
endmacro()

# Function to get all dependencies of a target
function(get_all_dependencies_from_xcode_target target out_var)
    get_target_property(deps ${target} LINK_LIBRARIES)
    if (deps)
        foreach(dep ${deps})
            if (TARGET ${dep})
                # Check if the target produces a binary file
                get_target_property(TYPE ${dep} TYPE)
                if (TYPE STREQUAL "SHARED_LIBRARY" OR TYPE STREQUAL "MODULE_LIBRARY")
                    list(APPEND result "${dep}")
                endif()

                # Recursively collect dependencies
                get_all_dependencies_from_xcode_target(${dep} result)
            endif()
        endforeach()
        set(${out_var} ${result} PARENT_SCOPE)
    endif()
endfunction()

macro(include_frameworks target frameworks)
    set_target_properties(${target} PROPERTIES
            XCODE_EMBED_FRAMEWORKS "${frameworks}"
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY ON
            XCODE_LINK_BUILD_PHASE_MODE BUILT_ONLY
    )
endmacro()

macro(include_p_list target plist_path)
    set_target_properties(${target} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${plist_path}"
    )
    set(PLIST_FILE ${plist_path})
endmacro()

macro(include_asset_catalog target asset_catalog_path)
    message(STATUS "ASSET_CATALOG_PATH=${asset_catalog_path}")

    # Ensure the asset catalog is included in the bundle resources
    set_source_files_properties(${asset_catalog_path}
            PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
    )

    # Asset catalog app icon set
    # List everything that is in the asset catalog
    file(GLOB_RECURSE asset_catalog_files CONFIGURE_DEPENDS "${asset_catalog_path}/*" "${asset_catalog_path}/**/**")

    # Get name of the asset catalog
    get_filename_component(ASSET_CATALOG_NAME ${asset_catalog_path} NAME)
    message(STATUS "ASSET_CATALOG_NAME=${ASSET_CATALOG_NAME}")
    set_source_files_properties(${asset_catalog_files} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources/${ASSET_CATALOG_NAME}"
    )

    foreach (FILE ${asset_catalog_files})
        file(RELATIVE_PATH NEW_FILE "${asset_catalog_path}" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        message(STATUS "Copying asset catalog file: ${FILE} to ${NEW_FILE}")
        set_source_files_properties(${FILE}
                PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources/${ASSET_CATALOG_NAME}/${NEW_FILE_PATH}"
                XCODE_FILE_ATTRIBUTES "CodeSignOnCopy"
        )
        set(RESOURCES_BUNDLE_FILES ${RESOURCES_BUNDLE_FILES} ${RESOURCES_BUNDLE_LOCATION}/${NEW_FILE} PARENT_SCOPE)
    endforeach()

    target_sources(${PROJECT_NAME} PRIVATE
            ${asset_catalog_path}
    )
endmacro()

macro(include_storyboards target storyboard_directory)
    # Search for every storyboard in the dir
    file(GLOB_RECURSE STORYBOARD_FILES CONFIGURE_DEPENDS "${storyboard_directory}/*.storyboard" "${storyboard_directory}/**/*.storyboard")

    target_sources(${target} PRIVATE
            ${STORYBOARD_FILES}
    )

    set_target_properties(${target} PROPERTIES
            RESOURCE "${STORYBOARD_FILES}"
    )

    set_source_files_properties(${STORYBOARD_FILES} PROPERTIES
            XCODE_FILE_TYPE "file.storyboard"
    )

    foreach (FILE ${STORYBOARD_FILES})
        file(RELATIVE_PATH NEW_FILE "${storyboard_directory}" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        message(STATUS "Copying storyboard: ${FILE}")
        set_source_files_properties(${FILE}
                PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources"
                XCODE_FILE_ATTRIBUTES "CodeSignOnCopy"
        )
    endforeach()
endmacro()

macro(include_xcode_resource_files target resource_files resource_dir)
    target_sources(${target} PRIVATE
            ${resource_files}
    )

    foreach (FILE ${resource_files})
        file(RELATIVE_PATH NEW_FILE "${resource_dir}" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        message(STATUS "Copying ${FILE} to ${NEW_FILE_PATH}")
        set_source_files_properties(${FILE}
                PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}"
                XCODE_FILE_ATTRIBUTES "CodeSignOnCopy"
                HEADER_FILE_ONLY TRUE
        )
    endforeach()
endmacro()

macro(include_xcode_resource_files target resource_files resource_dir)
    target_sources(${target} PRIVATE
            ${resource_files}
    )

    foreach (FILE ${resource_files})
        file(RELATIVE_PATH NEW_FILE "${resource_dir}" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        message(STATUS "Copying ${FILE} to ${NEW_FILE_PATH}")
        set_source_files_properties(${FILE}
                PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}"
                XCODE_FILE_ATTRIBUTES "CodeSignOnCopy"
                HEADER_FILE_ONLY TRUE
        )
    endforeach()
endmacro()

macro(include_xcode_bundle_resource_files target resource_files resource_dir)
    target_sources(${target} PRIVATE
            ${resource_files}
    )

    set_target_properties(${target} PROPERTIES
            RESOURCE "${resource_files}"
    )

    foreach (FILE ${resource_files})
        file(RELATIVE_PATH NEW_FILE "${resource_dir}" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        message(STATUS "Copying ${FILE} to ${NEW_FILE_PATH}")
        set_source_files_properties(${FILE}
                PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}"
                XCODE_FILE_ATTRIBUTES "CodeSignOnCopy"
                HEADER_FILE_ONLY TRUE
        )
    endforeach()
endmacro()