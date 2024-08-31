# Notes

- [Havok Physics](https://www.havok.com/havok-physics/): Consider applying for access to their code when the project is more advanced.
- [Comparison of Math Libraries](https://bargor.github.io/Comparison-of-3d-math-libraries/):
    - Consider using [`glm` with `SIMD`](https://glm.g-truc.net/0.9.1/api/a00002.html) mode for better performance.
    - Note the different coordinate systems between `glm` (Left-Handed) and `DirectXMath` (Right-Handed).
- [DirectXShaderCompiler: HLSL for Vulkan](https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/SPIR-V.rst#hlsl-semantic-and-vulkan-location): `[[vk:Location(X)]]` is the equivalent of `layout(location = X)` in GLSL.
- [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross): Useful for `SPIR-V` reflection and conversion to high-level shading languages.
- [Indexed vs. Direct & Interleaved vs. Non-Interleaved Vertex Buffers](https://stackoverflow.com/questions/17503787/vertex-buffers-indexed-or-direct-interlaced-or-separate)
- [flecs: ECS library](https://github.com/SanderMertens/flecs)
- Avoid templated code through dynamic libraries to prevent code replication.
- [RayTracing in Vulkan with HLSL](https://github.com/microsoft/DirectXShaderCompiler/blob/9c6b2c1275e41cc385b2034aa2716ec2303a62d6/docs/SPIR-V.rst#raytracing-in-vulkan-and-spirv)
- [Nanite: LOD Handling](https://advances.realtimerendering.com/s2021/Karis_Nanite_SIGGRAPH_Advances_2021_final.pdf)
- [Glaze: Optimized JSON parsing](https://github.com/stephenberry/glaze?tab=readme-ov-file)
- [Vulkan vs. DX12 vs. Optix for Raytracing](https://forums.developer.nvidia.com/t/ray-tracing-performance-in-optix-vs-vulkan-dxr/260558)
- [METIS: Graph Partitioning lib (used by Nanite)](https://github.com/KarypisLab/METIS)
- [PBRTv4: Scenes for test](https://github.com/mmp/pbrt-v4-scenes?tab=readme-ov-file)
- [Dr.Memory: Valgrind-like Software for Windows/Linux/MacOS](https://drmemory.org/page_download.html)
- [Memory Allocator for Vulkan](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)
- [Memory Allocator for DX12](https://github.com/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator)

### Learning Notes
