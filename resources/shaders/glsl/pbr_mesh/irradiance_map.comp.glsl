///
/// Project: VenomEngineWorkspace
/// @file irradiance_map.comp.glsl
/// @date Dec, 17 2024
/// @brief
/// author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#version 450

layout(binding = 2, set = 4, rgba16f) uniform image2D irradianceMap;

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main() {
    uvec3 groupThreadID = gl_LocalInvocationID;
}