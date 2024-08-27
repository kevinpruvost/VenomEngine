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
