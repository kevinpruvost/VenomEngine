#version 450
#extension GL_GOOGLE_include_directive : require

#include "Resources.vert.glsl.h"

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 viewDir;

void main() {
    // Convert NDC position to world space
    mat4 invView = view;
    // Remove translation
    invView[3][0] = invView[3][1] = invView[3][2] = 0.0;
    invView = inverse(invView);
    mat4 invProj = inverse(proj);

    vec4 worldspace = invView * invProj * vec4(inPosition.xy, 1.0, 1.0);

    gl_Position = vec4(inPosition, 1.0);
    viewDir = worldspace.xyz;
}