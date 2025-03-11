set(CMAKE_CXX_STANDARD 20)

# assimp library
add_subdirectory("assimp")
# Disable warnings for assimp for unsafe buffer usage and unreachable code break and shorten-64-to-32
target_compile_options(assimp PRIVATE
    -Wno-error=unsafe-buffer-usage
    -Wno-error=unreachable-code-break
    -Wno-error=shorten-64-to-32
)
set_target_properties(assimp PROPERTIES
    FOLDER "ExternalLibraries"
)

# DirectXShadercompiler
# Set options by default
# set(DXC_USE_LIT ON CACHE BOOL "Enable DXC use of lit" FORCE)
# set(LLVM_ENABLE_ASSERTIONS ON CACHE BOOL "Enable LLVM assertions" FORCE)
# set(LLVM_LIT_ARGS "-v" CACHE STRING "Arguments to pass to LLVM lit" FORCE)
# include("${EXTERNAL_LIBS_PATH}DirectXShaderCompiler/cmake/caches/PredefinedParams.cmake")
# add_subdirectory("DirectXShaderCompiler")

# glaze
add_subdirectory("glaze")

# glfw
# if IOS, then skip
if ((NOT APPLE) OR (NOT SDK_NAME MATCHES "iphone"))
add_subdirectory("glfw")
set_target_properties(glfw PROPERTIES
    FOLDER "ExternalLibraries"
    #COMPILE_FLAGS "-fno-objc-arc"
)
endif ()

# glm
set(GLM_ENABLE_CXX_20 ON)
add_subdirectory("glm")
set_target_properties(glm PROPERTIES
    FOLDER "ExternalLibraries"
)

# SPIRV-Cross
add_subdirectory("SPIRV-Cross")
set_target_properties(spirv-cross spirv-cross-c spirv-cross-core spirv-cross-glsl spirv-cross-hlsl spirv-cross-msl spirv-cross-reflect spirv-cross-util
    PROPERTIES
    FOLDER "ExternalLibraries"
)

# SPIRV-Reflect
#add_subdirectory("SPIRV-Reflect")
#set_target_properties(spirv-reflect spirv-reflect-pp
#    PROPERTIES
#    FOLDER "ExternalLibraries"
#)

# stb_image
    file(GLOB_RECURSE stb_image_hdrs stb_image/*.h)
    set(stb_image_srcs stb_image/stb_vorbis.c)

    # Create the static library target
    add_library(stb_image SHARED ${stb_image_srcs} ${stb_image_hdrs})

    target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)

    set_target_properties(stb_image PROPERTIES
        FOLDER "ExternalLibraries"
    )

# VulkanMemoryAllocator
add_subdirectory("VulkanMemoryAllocator")
set_target_properties(VulkanMemoryAllocator PROPERTIES
    FOLDER "ExternalLibraries"
)

# flecs
# If building shared libraries, comment out '#define flecs_STATIC'
if (BUILD_SHARED_LIBS)
    set(FLECS_HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/flecs/distr/flecs.h")
    # Read the flecs.h file content
    file(READ "${FLECS_HEADER_PATH}" FLECS_HEADER_CONTENT)

    # Modify the content to comment out '#define flecs_STATIC'
    string(REPLACE "#define flecs_STATIC" "//#define flecs_STATIC" FLECS_HEADER_CONTENT_MODIFIED "${FLECS_HEADER_CONTENT}")

    # Write the modified content back to the file
    file(WRITE "${FLECS_HEADER_PATH}" "${FLECS_HEADER_CONTENT_MODIFIED}")
endif()
add_subdirectory("flecs")

# tbb
set(BUILD_SHARED_LIBS OFF)
set(BUILD_BENCHMARKS OFF)
set(BUILD_TESTS OFF)
set(TBB_TEST OFF)
add_subdirectory("tbb")
# remove errors from conversions for tbb
target_compile_options(tbb PRIVATE
    -Wno-conversion
    -Wno-error=conversion
)
target_compile_options(tbbmalloc PRIVATE
    -Wno-conversion
    -Wno-error=conversion
)

# blosc
add_subdirectory("blosc")

# boost
set(BOOST_IOSTREAMS_ENABLE_ZSTD OFF)
set(LIBDEFLATE_BUILD_GZIP OFF)
add_subdirectory("boost")

# openvdb
# Disable Blosc on iOS
set(USE_BLOSC OFF)
if (NOT SDK_NAME MATCHES "iphone")
    set(USE_BLOSC ON)
endif ()
set(OPENVDB_TOOL_USE_ABC ON)
set(OPENVDB_ENABLE_UNINSTALL OFF)
set(OPENVDB_BUILD_VDB_PRINT OFF)
set(OPENVDB_BUILD_BINARIES OFF)
add_subdirectory("openvdb")

# openexr
set(OPENEXR_FORCE_INTERNAL_IMATH ON)
set(OPENEXR_BUILD_TOOLS OFF)
set(OPENEXR_INSTALL_TOOLS OFF)
set(OPENEXR_BUILD_EXAMPLES OFF)
set(OPENEXR_BUILD_PYTHON OFF)
set(OPENEXR_TEST_LIBRARIES OFF)
set(OPENEXR_TEST_TOOLS OFF)
set(OPENEXR_TEST_PYTHON OFF)
add_subdirectory("openexr")
find_package(OpenEXR REQUIRED)
set(BUILD_SHARED_LIBS ON)

# imgui
# Define the list of ImGui source files
set(IMGUI_DIR_ "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
set(IMGUI_DIR "${IMGUI_DIR_}" PARENT_SCOPE)
set(IMGUI_SOURCES
    "${IMGUI_DIR_}/imgui.cpp"
    "${IMGUI_DIR_}/imgui_draw.cpp"
    "${IMGUI_DIR_}/imgui_demo.cpp"
    "${IMGUI_DIR_}/imgui_tables.cpp"
    "${IMGUI_DIR_}/imgui_widgets.cpp"
)

#if iOS, then ignore glfw
if (NOT SDK_NAME MATCHES "iphone")
    list(APPEND IMGUI_SOURCES "${IMGUI_DIR_}/backends/imgui_impl_glfw.cpp")
endif ()

# imguizmo
set(IMGUIZMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imguizmo")
list(APPEND IMGUI_SOURCES "${IMGUIZMO_DIR}/ImGuizmo.cpp")

# Vulkan
#list(APPEND IMGUI_SOURCES
#        ${IMGUI_DIR_}/backends/imgui_impl_vulkan.cpp
#)
set(IMGUI_VULKAN_SOURCE ${IMGUI_DIR_}/backends/imgui_impl_vulkan.cpp PARENT_SCOPE)

#if (WIN32)
#    # DirectX12
#    list(APPEND IMGUI_SOURCES
#            ${IMGUI_DIR_}/backends/imgui_impl_dx12.cpp
#    )
#elseif (APPLE)
#    # Metal
#    list(APPEND IMGUI_SOURCES
#            ${IMGUI_DIR_}/backends/imgui_impl_metal.mm
#    )
#elseif (UNIX)
#    list(APPEND IMGUI_SOURCES ${IMGUI_DIR_}/backends/imgui_impl_glfw.cpp)
#endif ()
set(IMGUI_DX12_SOURCE ${IMGUI_DIR_}/backends/imgui_impl_dx12.cpp PARENT_SCOPE)
set(IMGUI_METAL_SOURCE ${IMGUI_DIR_}/backends/imgui_impl_metal.mm PARENT_SCOPE)

# Add the ImGui target as a static library
#add_library(imgui STATIC ${IMGUI_SOURCES})

# Specify that ImGui should be treated as an "interface" library to enable shared includes.
set(IMGUI_SOURCES "${IMGUI_SOURCES}" PARENT_SCOPE)
set(IMGUI_INCLUDE_DIR ${IMGUIZMO_DIR} ${IMGUI_DIR_} ${IMGUI_DIR_}/backends PARENT_SCOPE)
#target_include_directories(imgui PUBLIC ${IMGUI_DIR_} ${IMGUI_DIR_}/backends)
#target_compile_features(imgui PUBLIC cxx_std_11)

#target_compile_definitions(imgui PRIVATE VK_PROTOTYPES)
#target_link_libraries(imgui glfw)

# Optional: Link to platform-specific libraries if needed
# TODO: Don't forget to include in Metal
#if(APPLE)
#    target_link_libraries(imgui "-framework Metal -framework MetalKit -framework Cocoa -framework IOKit -framework CoreVideo -framework QuartzCore")
#    target_link_directories(imgui PRIVATE /usr/local/lib /opt/homebrew/lib /opt/local/lib)
#    target_include_directories(imgui PRIVATE /usr/local/include /opt/homebrew/include /opt/local/include)
#elseif(UNIX AND NOT APPLE)
#    find_package(X11 REQUIRED)
#    target_link_libraries(imgui PRIVATE X11)
#endif()

# MikktSpace
set(MikkTSpace_src
    "${CMAKE_CURRENT_SOURCE_DIR}/MikkTSpace/mikktspace.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/MikkTSpace/mikktspace.h"
)

add_library(MikkTSpace SHARED
    ${MikkTSpace_src}
)

target_include_directories(MikkTSpace INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/MikkTSpace/"
)

set_target_properties(MikkTSpace PROPERTIES
        FOLDER "ExternalLibraries"
)

# nativefiledialog
# If platofrm is apple and target is ios, then skip native file dialog
if ((NOT APPLE) OR (NOT SDK_NAME MATCHES "iphone"))
## Set the source files
set(NFD_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog/src/nfd_common.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog/src/nfd_common.h"
)

if (WIN32)
    list(APPEND NFD_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog/src/nfd_win.cpp"
    )
elseif (APPLE)
    find_library(APPKIT_FRAMEWORK AppKit)
    list(APPEND NFD_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog/src/nfd_cocoa.m"
    )
elseif (UNIX)
    list(APPEND NFD_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog/src/nfd_gtk.c"
    )
endif ()

add_library(nfd SHARED
    ${NFD_SOURCES}
)

target_include_directories(nfd PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog/src/include/"
)

if (APPLE)
    # nfd needs AppKit
    target_link_libraries(nfd "-framework AppKit")
endif ()
endif ()

# IconFontCppHeaders
add_library(IconFontCppHeaders INTERFACE)
target_include_directories(IconFontCppHeaders INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/IconFontCppHeaders/"
)