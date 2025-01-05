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

vec2 WorldSpaceToScreenSpace(vec3 position)
{
    vec4 pos = proj * view * vec4(position, 1.0);
    vec2 screenPos = pos.xy / pos.w;
    screenPos = screenPos * 0.5 + 0.5;
    screenPos.y = 1.0 - screenPos.y;
    return screenPos;
}

bool intersectionSphereToPlane(vec3 sphereCenter, float sphereRadius, vec3 planeNormal)
{
    float numerator = abs(planeNormal.x * (sphereCenter.x - cameraPos.x) + planeNormal.y * (sphereCenter.y - cameraPos.y) + planeNormal.z * (sphereCenter.z - cameraPos.z));
    float denom = sqrt(planeNormal.x * planeNormal.x + planeNormal.y * planeNormal.y + planeNormal.z * planeNormal.z);
    float distance = numerator / denom;
    return distance < sphereRadius;
}

vec3 frustumPlaneNormals[4];
bool isLightPointAffectingTile(Light light)
{
    // Calculate the light's bounding box
    float radius = sqrt(light.intensity / PointLight_Threshold);

    // Check intersections with the frustum planes
    bool horizontalIntersection = intersectionSphereToPlane(light.position, radius, frustumPlaneNormals[0]) || intersectionSphereToPlane(light.position, radius, frustumPlaneNormals[2]);
    bool verticalIntersection = intersectionSphereToPlane(light.position, radius, frustumPlaneNormals[1]) || intersectionSphereToPlane(light.position, radius, frustumPlaneNormals[3]);
    return horizontalIntersection && verticalIntersection;
}

vec3 ScreenSpaceToWorldSpace(vec2 screenPos, float depth)
{
    vec4 clipSpace = vec4(screenPos.x * 2.0 - 1.0, 1.0 - screenPos.y * 2.0f, depth, 1.0);
    vec4 viewSpace = inverse(proj) * clipSpace;
    vec4 worldSpace = inverse(view) * viewSpace;
    return worldSpace.xyz / worldSpace.w;
}

vec3 frustumPlaneFrom2DPoints(vec2 point1, vec2 point2)
{
    vec3 p1 = ScreenSpaceToWorldSpace(point1, -1.0);
    vec3 p2 = ScreenSpaceToWorldSpace(point2, -1.0);
    return normalize(cross(p1 - cameraPos, p2 - cameraPos));
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    uvec3 groupThreadID = gl_LocalInvocationID;

    // Compute the tile coordinates based on the dispatch thread ID
    uint tileX = groupThreadID.x;
    uint tileY = groupThreadID.y;

    // Determine the screen area covered by this tile
    float tileMinX = float(tileX) / 32.0;
    float tileMinY = float(tileY) / 32.0;
    float tileMaxX = (tileMinX + 1) / 32.0;
    float tileMaxY = (tileMinY + 1) / 32.0;

    // Now calculate which lights affect this tile
    LightBlock relevantLights = initLightBlock();

    frustumPlaneNormals[0] = frustumPlaneFrom2DPoints(vec2(tileMinX, tileMinY), vec2(tileMinX, tileMaxY));
    frustumPlaneNormals[1] = frustumPlaneFrom2DPoints(vec2(tileMinX, tileMaxY), vec2(tileMaxX, tileMaxY));
    frustumPlaneNormals[2] = frustumPlaneFrom2DPoints(vec2(tileMaxX, tileMaxY), vec2(tileMaxX, tileMinY));
    frustumPlaneNormals[3] = frustumPlaneFrom2DPoints(vec2(tileMaxX, tileMinY), vec2(tileMinX, tileMinY));

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
            if (isLightPointAffectingTile(light)) {
                relevantLights.block[intIndex] |= (1 << bitIndex); // Mark this light as affecting all tiles
            }
            //relevantLights.block[intIndex] |= (1 << bitIndex); // Mark this light as affecting all tiles
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