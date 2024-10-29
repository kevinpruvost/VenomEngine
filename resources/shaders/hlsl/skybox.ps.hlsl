#include "Resources.ps.hlsl.h"

struct PanoramInput {
    float3 viewDir : TEXCOORD0; // Normalized view direction from the vertex shader
};

float4 main(PanoramInput input) : SV_Target {
    // Convert the view direction to spherical coordinates
    float phi = atan2(input.viewDir.z, input.viewDir.x); // Azimuth angle
    float theta = asin(input.viewDir.y);                 // Inclination angle

    // Map spherical coordinates to UVs in the range [0, 1]
    float2 uv;
    uv.x = phi / (2.0 * 3.14159265359) + 0.5;  // Horizontal, azimuth
    uv.y = theta / 3.14159265359 + 0.5;        // Vertical, inclination

    // Sample the equirectangular texture stored in the first texture unit
    return GetTexture(0, uv);
}