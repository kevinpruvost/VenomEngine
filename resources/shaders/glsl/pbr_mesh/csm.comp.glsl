///
/// Project: VenomEngineWorkspace
/// @file csm.cs.glsl
/// @date Nov, 26 2024
/// @brief
/// author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#version 450
#extension GL_GOOGLE_include_directive : require

#include "../Scene.glsl.h"

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main() {
    uvec3 groupThreadID = gl_LocalInvocationID;
}