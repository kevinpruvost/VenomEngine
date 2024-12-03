#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_samplerless_texture_functions : require

#include "Resources.frag.glsl.h"

layout(location = 0) in vec3 viewDir;

// Function to sample from the panorama texture
vec4 GetPanoramaColor(vec2 uv) {
    // Sample the texture using the provided UV coordinates
    vec4 color = texture(sampler2D(panoramaTexture, g_sampler), uv);
    float exposure = sceneSettings.targetLuminance / panoramaPeakLuminance;
    return vec4(color.rgb * 0.105, color.a); // Applying exposure factor
}

float atan2_custom(in float y, in float x)
{
    bool s = (abs(x) > abs(y));
    return mix(M_PI/2.0 - atan(x,y), atan(y,x), s);
}

void main() {
    // Normalize the view direction
    vec3 viewDir = normalize(viewDir);
    // Convert the view direction to spherical coordinates
    float phi = atan2_custom(viewDir.z, viewDir.x); // Azimuth angle
    float theta = asin(viewDir.y);                 // Inclination angle

    // Map spherical coordinates to UVs in the range [0, 1]
    vec2 uv;
    uv.x = phi / (2.0 * M_PI) + 0.5;  // Horizontal, azimuth
    uv.y = 1.0 - theta / M_PI + 0.5;   // Vertical, inclination

    // Box Blur Parameters
    uint width, height;
    ivec2 texDims = textureSize(panoramaTexture, 0); // Get texture dimensions
    width = texDims.x;
    height = texDims.y;

    // Calculate texel size for sampling offsets
    vec2 texelSize = vec2(1.0 / float(width), 1.0 / float(height));

    vec4 color = vec4(0.0);
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
            vec2 offset = texelSize * vec2(i, j);
            float distanceSquared = float(i * i + j * j);
            float weight = normalizationFactor * exp(-distanceSquared / sigma2);

            color += GetPanoramaColor(uv + offset) * weight;
            weightSum += weight;
        }
    }

    // Normalize by the sum of weights
    color /= weightSum;

    // Output the final color
    finalColor = color;
}