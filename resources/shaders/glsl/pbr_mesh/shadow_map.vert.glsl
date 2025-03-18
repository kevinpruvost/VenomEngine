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

layout(push_constant, std430) uniform LightData
{
    mat4 lightSpaceMatrix;
    int lightType;
    int cascadeIndex;
} lightData;

void main() {
    //gl_Position = lightData.lightSpaceMatrix * vec4(inPosition, 1.0);  // Apply the view matrix
    gl_Position = models[gl_InstanceIndex] * vec4(inPosition, 1.0); // Apply the model matrix
    gl_Position = lightData.lightSpaceMatrix * vec4(gl_Position.xyz / gl_Position.w, 1.0);  // Apply the view matrix
    gl_Position.y = -gl_Position.y; // Invert the Y axis
}