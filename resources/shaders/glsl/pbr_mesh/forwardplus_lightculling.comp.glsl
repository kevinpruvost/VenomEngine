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

// Function to calculate the intersection of a plane and cone
bool intersectPlaneCone(vec3 planeOrigin, vec3 planeNormal,
                        vec3 coneOrigin, vec3 coneDirection,
                        float coneAngle, float coneRange)
{
    // Calculate the angle between the cone direction and the plane normal
    float angle = dot(coneDirection, planeNormal);
    if (angle < cos(coneAngle))
        return false;

    // Calculate the distance from the cone origin to the plane
    float distance = dot(planeNormal, planeOrigin - coneOrigin);
    if (distance < 0.0)
        return false;

    // Calculate the distance from the cone origin to the intersection point
    float intersectionDistance = distance / angle;
    if (intersectionDistance > coneRange)
        return false;

    return true;
}

bool intersectionSphereToPlane(vec3 sphereCenter, float sphereRadius, vec3 planeNormal)
{
    vec3 v1 = sphereCenter - cameraPos;
    float b = abs(dot(planeNormal, v1));
    if (b > sphereRadius)
        return false;
    return true;
}

bool intersectionConeToPlane(vec3 coneApex, vec3 coneDir, float coneAngle, float range, vec3 planeNormal)
{
    return intersectPlaneCone(cameraPos, planeNormal, coneApex, coneDir, coneAngle, range);
}

float tileMinX;
float tileMinY;
float tileMaxX;
float tileMaxY;
bool isLightCentroidInFrustum(Light light, float range, vec3 dir)
{
    vec3 cent;
    if (light.type == LightType_Point) {
        cent = light.position;
    } else if (light.type == LightType_Spot) {
        cent = light.position + range * dir;
    }
    vec2 screenPos = WorldSpaceToScreenSpace(cent);
    return screenPos.x >= tileMinX && screenPos.x <= tileMaxX && screenPos.y >= tileMinY && screenPos.y <= tileMaxY;
}

vec3 frustumPlaneNormals[4];
bool isLightPointAffectingTile(Light light)
{
    // Calculate the light's bounding box
    float radius = sqrt(light.intensity / PointLight_Threshold);
    if (isLightCentroidInFrustum(light, 0.0, vec3(0.0, 0.0, 0.0)))
        return true;

    // Check intersections with the frustum planes
    bool horizontalIntersection = intersectionSphereToPlane(light.position, radius, frustumPlaneNormals[0]) || intersectionSphereToPlane(light.position, radius, frustumPlaneNormals[2]);
    bool verticalIntersection = intersectionSphereToPlane(light.position, radius, frustumPlaneNormals[1]) || intersectionSphereToPlane(light.position, radius, frustumPlaneNormals[3]);
    return horizontalIntersection && verticalIntersection;
}

bool isSpotLightAffectingTile(Light light, float tileMinX, float tileMinY, float tileMaxX, float tileMaxY)
{
    // Calculate the light's bounding box
    float angle = cos(light.angle / 180.0 * M_PI);
    float range = light.intensity / SpotLight_Threshold;
    vec3 direction = SpotAndDirectionalDirection(light.direction);
    if (isLightCentroidInFrustum(light, range, direction))
        return true;

    // Check intersections with the frustum planes
    bool horizontalIntersection = intersectionConeToPlane(light.position, direction, angle, range, frustumPlaneNormals[0])
        || intersectionConeToPlane(light.position, direction, angle, range, frustumPlaneNormals[2]);
    bool verticalIntersection = intersectionConeToPlane(light.position, direction, angle, range, frustumPlaneNormals[1])
        || intersectionConeToPlane(light.position, direction, angle, range, frustumPlaneNormals[3]);
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
    vec3 p1 = ScreenSpaceToWorldSpace(point1, 1.0);
    vec3 p2 = ScreenSpaceToWorldSpace(point2, 1.0);
    return normalize(cross(cameraPos - p1, cameraPos - p2));
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    uvec3 groupThreadID = gl_LocalInvocationID;

    // Compute the tile coordinates based on the dispatch thread ID
    uint tileX = groupThreadID.x;
    uint tileY = groupThreadID.y;

    // Determine the screen area covered by this tile
    tileMinX = float(tileX) / 32.0;
    tileMinY = float(tileY) / 32.0;
    tileMaxX = (tileX + 1) / 32.0;
    tileMaxY = (tileY + 1) / 32.0;

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
        } else if (light.type == LightType_Spot) {
            // Perform the culling test (for simplicity, let's use spot light radius check)
            if (isSpotLightAffectingTile(light, tileMinX, tileMinY, tileMaxX, tileMaxY)) {
                relevantLights.block[intIndex] |= (1 << bitIndex); // Mark this light as affecting all tiles
            }
            // relevantLights.block[intIndex] |= (1 << bitIndex); // Mark this light as affecting all tiles
        }
    }

    // Store the relevant light indices (for simplicity, assume we write to a result buffer)
    uint resultIndex = tileY * 32 + tileX;
    forwardPlusResultBuffer[resultIndex] = relevantLights;
}