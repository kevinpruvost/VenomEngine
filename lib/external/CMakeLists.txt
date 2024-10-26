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
add_subdirectory("glfw")
set_target_properties(glfw PROPERTIES
    FOLDER "ExternalLibraries"
)

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

# openvdb
set(OPENVDB_TOOL_USE_ABC ON)
set(OPENVDB_ENABLE_UNINSTALL OFF)
add_subdirectory("openvdb")