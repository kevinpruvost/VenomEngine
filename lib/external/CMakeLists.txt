set(CMAKE_CXX_STANDARD 20)

# assimp library
add_subdirectory("assimp")
# Disable warnings for assimp for unsafe buffer usage and unreachable code break and shorten-64-to-32
target_compile_options(assimp PRIVATE
    -Wno-error=unsafe-buffer-usage
    -Wno-error=unreachable-code-break
    -Wno-error=shorten-64-to-32
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

# glm
add_subdirectory("glm")

# SPIRV-Cross
add_subdirectory("SPIRV-Cross")

# SPIRV-Reflect
add_subdirectory("SPIRV-Reflect")

# stb_image
    file(GLOB_RECURSE stb_image_hdrs stb_image/*.h)
    set(stb_image_srcs stb_image/stb_vorbis.c)

    # Create the static library target
    add_library(stb_image SHARED ${stb_image_srcs} ${stb_image_hdrs})

    target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)

# VulkanMemoryAllocator
add_subdirectory("VulkanMemoryAllocator")