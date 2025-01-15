///
/// Project: VenomEngineWorkspace
/// @file shadow_map.vert.glsl
/// @date Jan, 9 2025
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#version 450

#extension GL_GOOGLE_include_directive : require

#include "../Resources.vert.glsl.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

void main() {
    gl_Position = models[gl_InstanceIndex] * vec4(inPosition, 1.0); // Apply the model matrix
    gl_Position = proj * view * vec4(gl_Position.xyz / gl_Position.w, 1.0);  // Apply the view matrix
}