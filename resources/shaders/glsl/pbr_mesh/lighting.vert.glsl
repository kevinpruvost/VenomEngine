#version 450

#extension GL_GOOGLE_include_directive : require

#include "../Resources.vert.glsl.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

layout(location = 0) out vec3 worldPos;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec4 fragColor;
layout(location = 4) out vec3 tangent;
layout(location = 5) out vec3 bitangent;

layout(location = 6) out vec2 screenPos;

void main() {
    gl_Position = models[gl_InstanceIndex] * vec4(inPosition, 1.0); // Apply the model matrix
    worldPos = gl_Position.xyz / gl_Position.w; // Store the world position for later use
    gl_Position = proj * view * vec4(worldPos, 1.0);  // Apply the view matrix
    fragColor = vec4(1.0, 1.0, 1.0, 1.0); // Pass the color to the fragment shader

    // Transform normal
    mat3 normalMatrix = transpose(inverse(mat3(models[gl_InstanceIndex])));
    vec3 transformedNormal = normalize(normalMatrix * inNormal);
    normal = transformedNormal; // Normalize for correctness

    vec3 T, B;

    T = normalize(normalMatrix * inTangent);
    B = normalize(normalMatrix * inBitangent);

    tangent = T;
    bitangent = B;

    fragTexCoord = inTexCoord; // Pass the texture
    screenPos = gl_Position.xy / gl_Position.w; // Store the screen position for later use
}