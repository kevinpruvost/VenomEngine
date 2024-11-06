#include "Resources.ps.hlsl.h"

struct PanoramInput {
    [[vk::location(0)]] float3 viewDir : TEXCOORD0;     // Equivalent to layout(location = 0) out in GLSL
};

float4 GetPanoramaColor(float2 uv) {
    float4 color = panoramaTexture.Sample(g_sampler, uv);
    float exposure = targetLuminance / panoramaPeakLuminance;
    return float4(color.rgb * 0.105, color.a);
}

float4 main(PanoramInput input) : SV_Target {
    input.viewDir = normalize(input.viewDir);
    // Convert the view direction to spherical coordinates
    float phi = atan2(input.viewDir.z, input.viewDir.x); // Azimuth angle
    float theta = asin(input.viewDir.y);                 // Inclination angle

    // Map spherical coordinates to UVs in the range [0, 1]
    float2 uv;
    uv.x = phi / (2.0 * M_PI) + 0.5;  // Horizontal, azimuth
    uv.y = 1.0 - theta / M_PI + 0.5;        // Vertical, inclination

    // TODO: IMPLEMENT AUTO EXPOSURE

    // return GetPanoramaColor(uv);
    // Box Blur
    // Get texture dimensions
    uint width, height;
    panoramaTexture.GetDimensions(width, height);
    return GetPanoramaColor(uv);

    // Calculate texel size for sampling offsets
    float2 texelSize = float2(1.0 / width, 1.0 / height);

    // Initialize color accumulator and sample count
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    const int radius = 14; // Radius for 15x15 kernel

    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            float2 offset = texelSize * float2(i, j);
            color += GetPanoramaColor(uv + offset);
        }
    }

    // Normalize by the total number of samples
    color /= (radius * 2 + 1) * (radius * 2 + 1);

    return color;
}