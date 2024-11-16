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

    // Box Blur
    // Get texture dimensions
    uint width, height;
    panoramaTexture.GetDimensions(width, height);

    // TODO: Optimize Gaussian Blur because it is VERY SLOW

    // Calculate texel size for sampling offsets
    float2 texelSize = float2(1.0 / width, 1.0 / height);

    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    float weightSum = 0.0;

    // Parameters for Gaussian kernel
    const int radius = 4;
    const float sigma = 4.0; // Adjust sigma based on desired blur

    // Precompute constants for Gaussian function
    float sigma2 = 2.0 * sigma * sigma;
    float normalizationFactor = 1.0 / (M_PI * sigma2);

    // Apply Gaussian weighted sampling
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            float2 offset = texelSize * float2(i, j);
            float distanceSquared = float(i * i + j * j);
            float weight = normalizationFactor * exp(-distanceSquared / sigma2);

            color += GetPanoramaColor(uv + offset) * weight;
            weightSum += weight;
        }
    }

    // Normalize by the sum of weights
    color /= weightSum;

    return color;
}