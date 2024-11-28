#include "Scene.glsl.h"

//layout(location = 0) in vec3 inPosition;
//layout(location = 1) in vec3 inNormal;
//layout(location = 2) in vec2 inTexCoord;
//layout(location = 3) in vec3 inTangent;
//layout(location = 4) in vec3 inBitangent;

layout(std430, binding = 0) buffer UniformBufferObject {
    mat4 models[];
};