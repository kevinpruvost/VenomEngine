cmake_minimum_required(VERSION 3.20)

# Do not name same name as folder, it can generate problems with linking (e.g. VenomEngine/ folder in build/
# but VenomEngine target is also named VenomEngine)
project(VenomEngineLauncher)

##
# Compile Flags
##

if (CMAKE_SYSTEM_NAME MATCHES "iOS")
    set(PLATFORM_FLAG "TARGET_IOS")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(PLATFORM_FLAG "TARGET_MACOS")
endif ()

##
# Source files
##
file(GLOB_RECURSE VenomEngine_Srcs
    "./*.cc"
)

set(VenomEngine_Srcs
    ${VenomEngine_Srcs}
    main.cc
)

add_executable(${PROJECT_NAME}
    ${VenomEngine_Srcs}
)

# Specifying MACOSX_BUNDLE in add_executable will not work for some reason
set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE YES
)

##
# Include directories
##
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

##
# Link libraries
##
target_link_libraries(${PROJECT_NAME} PUBLIC
    VenomCommon
)

# Depend on link_resources to symlink/copy
add_dependencies(${PROJECT_NAME} link_resources)

##
# Dependencies dll loaded at runtime manually
##
if ((NOT APPLE) OR (NOT CMAKE_SYSTEM_NAME MATCHES "iOS"))
    add_dependencies(${PROJECT_NAME} VenomVulkan)
endif ()

if (APPLE)
add_dependencies(${PROJECT_NAME} VenomMetal)
elseif (WIN32)
#add_dependencies(${PROJECT_NAME} VenomDX12)
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (APPLE)

    ##
    # PLists
    ##

    # if iOS change Info.pList to another file
    if (CMAKE_SYSTEM_NAME MATCHES "iOS")
        include_p_list(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/iOS/Info.plist")
    elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        include_p_list(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/macOS/Info.plist")
    endif ()

    ##
    # Asset catalog app icon set
    ##

    include_asset_catalog(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/Assets.xcassets")

    ##
    # Frameworks
    ##

    ##
    # IF NEEDED, ADD MANUALLY EVERY DYLIB / FRAMEWORK THAT YOU MANUALLY LOAD AT RUNTIME HERE
    # THIS MACRO ONLY DETECTS REAL DEPENDENCIES
    ##
    get_all_dependencies_from_xcode_target(${PROJECT_NAME} EMBEDDED_LIBS)

    ##
    # IF NEEDED, ADD MANUALLY EVERY DYLIB / FRAMEWORK THAT YOU MANUALLY LOAD AT RUNTIME HERE
    ##
    list(APPEND EMBEDDED_LIBS ${VENOM_RUNTIME_LOADED_DLLS})
    include_frameworks(${PROJECT_NAME} "${EMBEDDED_LIBS}")

    ##
    # Bundle properties
    ##

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE YES
        MACOSX_BUNDLE_INFO_PLIST "${PLIST_FILE}"
        MACOSX_BUNDLE_BUNDLE_NAME "VenomEngine"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.pruvostkevin.venomengine"
        MACOSX_BUNDLE_ICON_FILE "VenomEngine.icns"
        MACOSX_BUNDLE_INFO_STRING "VenomEngine"
        MACOSX_BUNDLE_LONG_VERSION_STRING "1.0.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.pruvostkevin.venomengine"

        INSTALL_RPATH "@executable_path/../Frameworks"

        XCODE_LINK_BUILD_PHASE_MODE BUILT_ONLY
    )

    ##
    # Icon
    ##

    # Icon contained in the asset catalog
    set_xcode_property(${PROJECT_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon" "All")

    ##
    # Copy resources to bundle
    ##

    # If there are storyboards
    if (CMAKE_SYSTEM_NAME MATCHES "iOS")
        include_storyboards(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/Application/iOS/Base.lproj/")
    elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        include_storyboards(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/Application/macOS/Base.lproj/")
    endif ()

    include_xcode_resource_files(${PROJECT_NAME} "${RESOURCE_FILES}" "${CMAKE_SOURCE_DIR}/resources")

    set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")

    install(CODE "set(CODESIGN_COMMAND \"${CODESIGN_COMMAND}\")")
    install(CODE "set(PROJECT_PACKAGE_NAME \"${PROJECT_PACKAGE_NAME}\")")
    install(CODE [[
        include(BundleUtilities)
        # Note Mac specific extension .app
        set(APPS ${CMAKE_INSTALL_PREFIX}/${PROJECT_PACKAGE_NAME}.app)

        # Directories to look for dependencies
        set(DIRS ${CMAKE_INSTALL_PREFIX})

        message(STATUS "APPS=${APPS}")
        message(STATUS "DIRS=${DIRS}")

        file(GLOB PLUGIN_FILES ${APPS}/Contents/Frameworks/*.dylib)
        message(STATUS "PLUGIN_FILES=${PLUGIN_FILES}")

        # NOTE: No need `${DIRS}` when already setting rpath in exectuable in install by `set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")`
        fixup_bundle(${APPS}
        "${PLUGIN_FILES}"
        ${DIRS}
        )

        # Sign every file in the bundle
        file(GLOB_RECURSE DYLIB_FILES_TO_SIGN "${APPS}/Contents/Frameworks/*")
        file(GLOB_RECURSE RESOURCE_FILES_TO_SIGN "${APPS}/Contents/Resources/*")
        file(GLOB_RECURSE BINARY_FILES_TO_SIGN "${APPS}/Contents/MacOS/*")
        file(GLOB_RECURSE IOS_BINARY_FILES_TO_SIGN "${APPS}/Contents/iOS/*")
        set(FILES_TO_SIGN
        ${APPS}
        ${DYLIB_FILES_TO_SIGN}
        #${RESOURCE_FILES_TO_SIGN}
        ${BINARY_FILES_TO_SIGN}
        #${IOS_BINARY_FILES_TO_SIGN}
        )

        foreach(FILE ${FILES_TO_SIGN})
            message(STATUS "Signing file: ${CODESIGN_COMMAND} ${FILE}")
            execute_process(COMMAND ${CODESIGN_COMMAND} "${FILE}"
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                RESULT_VARIABLE CODESIGN_RESULT
                OUTPUT_VARIABLE CODESIGN_OUTPUT
                ERROR_VARIABLE CODESIGN_ERROR
            )
            message(STATUS "CODESIGN_OUTPUT=${CODESIGN_OUTPUT}")
            message(STATUS "CODESIGN_ERROR=${CODESIGN_ERROR}")
            message(STATUS "CODESIGN_RESULT=${CODESIGN_RESULT}")
        endforeach()
    ]])

    set(CPACK_GENERATOR "DRAGNDROP")
    set(CPACK_PACKAGE_NAME "VenomEngine")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_VENDOR "VenomEngine")
    set(CPACK_BUNDLE_APPLE_CODESIGN_PARAMETER "--deep -f --options runtime")
    include(CPack)
endif()