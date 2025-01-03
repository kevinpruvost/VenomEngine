///
/// Project: VenomEngineWorkspace
/// @file forwardplus_lightculling.cs.glsl
/// @date Nov, 26 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#version 450
#extension GL_GOOGLE_include_directive : require

#include "../Scene.glsl.h"

bool isLightPointAffectingTile(Light light, uint tileMinX, uint tileMinY, uint tileMaxX, uint tileMaxY)
{
    // Calculate light position in camera space
    vec4 lightPos = view * vec4(light.position, 1.0);
    lightPos = proj * lightPos;

    // Calculate the light's bounding box
    vec3 lightMin = lightPos.xyz - light.radius;
    vec3 lightMax = lightPos.xyz + light.radius;

    return false;

    // Perform the AABB vs AABB test
    bool xOverlap = lightMin.x <= tileMaxX && lightMax.x >= tileMinX;
    bool yOverlap = lightMin.y <= tileMaxY && lightMax.y >= tileMinY;
    bool zOverlap = lightMin.z <= 0.0 && lightMax.z >= 1.0;

    return xOverlap && yOverlap && zOverlap;
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main() {
    uvec3 groupThreadID = gl_LocalInvocationID;

    // Compute the tile coordinates based on the dispatch thread ID
    uint tileX = groupThreadID.x;
    uint tileY = groupThreadID.y;

    // Determine the screen area covered by this tile
    uint tileMinX = tileX * 32;
    uint tileMinY = tileY * 32;
    uint tileMaxX = min(tileMinX + 32, graphicsSettings.screenWidth);
    uint tileMaxY = min(tileMinY + 32, graphicsSettings.screenHeight);

    // Now calculate which lights affect this tile
    LightBlock relevantLights = initLightBlock();

    // Loop through all lights (we are limiting to 65536 lights here)
    for (int i = 0; i < lightCount; ++i) {
        // Fetch the light from the bindless buffer (this is an offset-based access)
        Light light = lights[i];

        int intIndex = getLightBlockIntIndex(i);
        int bitIndex = getLightBlockBitIndex(i);

        if (light.type == LightType_Directional)
            relevantLights.block[intIndex] |= (1 << bitIndex); // Mark this light as affecting all tiles
        else if (light.type == LightType_Point) {
            // Perform the culling test (for simplicity, let's use point light radius check)
            //if (isLightPointAffectingTile(light, tileMinX, tileMinY, tileMaxX, tileMaxY)) {
             //   relevantLights.block[intIndex] |= (1 << bitIndex); // Mark this light as affecting all tiles
            //}
            relevantLights.block[intIndex] |= (1 << bitIndex); // Mark this light as affecting all tiles
        } else if (light.type == LightType_Spot) {
            // Perform the culling test (for simplicity, let's use spot light radius check)
            //if (isLightSpotAffectingTile(light, tileMinX, tileMinY, tileMaxX, tileMaxY)) {
             //   relevantLights.block[intIndex] |= (1 << bitIndex); // Mark this light as affecting all tiles
            //}
            relevantLights.block[intIndex] |= (1 << bitIndex); // Mark this light as affecting all tiles
        }
    }

    // Store the relevant light indices (for simplicity, assume we write to a result buffer)
    uint resultIndex = tileY * 32 + tileX;
    forwardPlusResultBuffer[resultIndex] = relevantLights;
}