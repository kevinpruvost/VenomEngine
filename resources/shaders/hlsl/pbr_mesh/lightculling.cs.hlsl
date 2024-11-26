#include "../Scene.hlsl.h"

bool isLightPointAffectingTile(Light light, uint tileMinX, uint tileMinY, uint tileMaxX, uint tileMaxY)
{
    // Calculate light position in camera space
    float4 lightPos = mul(float4(light.position, 1.0f), view);
    lightPos = mul(lightPos, proj);

    // Calculate the light's bounding box
    float3 lightMin = lightPos.xyz - light.radius;
    float3 lightMax = lightPos.xyz + light.radius;

    return false;

    // Perform the AABB vs AABB test
    bool xOverlap = lightMin.x <= tileMaxX && lightMax.x >= tileMinX;
    bool yOverlap = lightMin.y <= tileMaxY && lightMax.y >= tileMinY;
    bool zOverlap = lightMin.z <= 0.0f && lightMax.z >= 1.0f;

    return xOverlap && yOverlap && zOverlap;
}

[numthreads(32, 32, 1)]
void main(uint3 groupThreadID : SV_GroupThreadID)
{
    // Compute the tile coordinates based on the dispatch thread ID
    uint tileX = groupThreadID.x;
    uint tileY = groupThreadID.y;

    // Determine the screen area covered by this tile
    uint tileMinX = tileX * 32;
    uint tileMinY = tileY * 32;
    uint tileMaxX = min(tileMinX + 32, screenWidth);
    uint tileMaxY = min(tileMinY + 32, screenHeight);

    // Now calculate which lights affect this tile
    int relevantLights = 0;

    // Loop through all lights (we are limiting to 65536 lights here)
    for (int i = 0; i < lightCount; ++i)
    {
        // Fetch the light from the bindless buffer (this is an offset-based access)
        Light light = lights[i];

        if (light.type == LightType::Directional)
            relevantLights |= (1 << i); // Mark this light as affecting all tiles
        else if (light.type == LightType::Point)
        {
            // Perform the culling test (for simplicity, let's use point light radius check)
            if (isLightPointAffectingTile(light, tileMinX, tileMinY, tileMaxX, tileMaxY))
            {
                relevantLights |= (1 << i); // Mark this light as affecting this tile
            }
        }
    }

    // Store the relevant light indices (for simplicity, assume we write to a result buffer)
    uint resultIndex = tileY * 32 + tileX;
    forwardPlusResultBuffer[resultIndex] = relevantLights;
}
