///
/// Project: VenomEngineWorkspace
/// @file shadow_map.cs.glsl
/// @date Jan, 9 2025
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#version 450
#extension GL_GOOGLE_include_directive : require

#include "../Scene.glsl.h"

vec2 WorldSpaceToScreenSpace(vec3 position)
{
    vec4 pos = proj * view * vec4(position, 1.0);
    vec2 screenPos = pos.xy / pos.w;
    screenPos = screenPos * 0.5 + 0.5;
    screenPos.y = 1.0 - screenPos.y;
    return screenPos;
}

float dOfPlane(vec3 planeNormal, vec3 planeOrigin)
{
    return -dot(planeNormal, planeOrigin);
}

float tileMinX;
float tileMinY;
float tileMaxX;
float tileMaxY;

void main()
{
    // Loop through all lights (we are limiting to 65536 lights here)
    for (int i = 0; i < lightCount; ++i) {
        // Fetch the light from the bindless buffer (this is an offset-based access)
        Light light = lights[i];

        if (light.type == LightType_Directional) {

        } else if (light.type == LightType_Point) {

        } else if (light.type == LightType_Spot) {

        }
    }
}