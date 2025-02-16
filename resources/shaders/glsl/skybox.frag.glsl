#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_samplerless_texture_functions : require

#include "Resources.frag.glsl.h"

layout(location = 0) in vec3 viewDir;

float Gaussian(float x, float sigma) {
    return exp(-0.5 * (x * x) / (sigma * sigma)) / (sigma * sqrt(2.0 * M_PI));
}

const int kernelSize = 45;
const int halfSize = kernelSize / 2;

void GenerateGaussianKernel(float sigma, out float kernel[kernelSize * kernelSize])
{
    float sum = 0.0;

    // Generate the kernel
    for (int i = -halfSize; i <= halfSize; ++i) {
        for (int j = -halfSize; j <= halfSize; ++j) {
            float value = Gaussian(float(i), sigma) * Gaussian(float(j), sigma);
            kernel[(i + halfSize) * kernelSize + (j + halfSize)] = value;
            sum += value;
        }
    }

    // Normalize the kernel
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i * kernelSize + j] /= sum;
        }
    }
}

// Function to apply precomputed Gaussian blur
vec3 GaussianBlur(vec3 dir, int blurRadius)
{
    vec2 uv = PanoramaUvFromDir(dir);

    // Calculate texel size (offset between each texel)
    int width = textureSize(panoramaTexture, 0).x;
    int height = textureSize(panoramaTexture, 0).y;
    vec2 texelSize = vec2(1.0 / float(width), 1.0 / float(height));

    // Gaussian weights for a 15x15 kernel

    float kernel[kernelSize * kernelSize];
    GenerateGaussianKernel(5.0, kernel);
    vec3 finalColor = vec3(0.0);

    float kernelSum = 0.0;

    // Apply Gaussian blur
    for (int i = -halfSize; i <= halfSize; ++i) {
        for (int j = -halfSize; j <= halfSize; ++j) {
            vec2 offset = vec2(float(i * 2), float(j * 2)) * texelSize;
            vec2 sampleUv = uv + offset;
            vec4 sampleColor = GetPanoramaTexture(sampleUv);
            kernelSum += kernel[(i + halfSize) * kernelSize + (j + halfSize)];
            finalColor += sampleColor.rgb * (kernel[(i + halfSize) * kernelSize + (j + halfSize)]);
        }
    }
    finalColor /= kernelSum;

    return finalColor; // Return the blurred color
}

vec4 ToneMapReinhard(vec4 color) {
    return color / (color + vec4(1.0));
}

vec3 gammaCorrect(vec3 color) {
    return pow(color, vec3(1.0 / 2.2)); // Gamma 2.2
}

vec3 toSRGB(vec3 linearColor) {
    return mix(12.92 * linearColor, pow(1.055 * linearColor, vec3(1.0 / 2.4)) - vec3(0.055), step(0.0031308, linearColor));
}

vec3 filmicToneMap(vec3 color) {
    float A = 0.22; // Shoulder strength
    float B = 0.30; // Linear section
    float C = 0.10; // Toe strength
    float D = 0.20; // Toe denominator
    float E = 0.01; // Linear white level
    float F = 0.30; // Linear black level
    return ((color * (A * color + C * B) + D * E) /
            (color * (A * color + B) + D * F)) - E / F;
}

void main() {
    // Normalize the view direction
    vec3 viewDir = normalize(viewDir);
    // Convert the view direction to spherical coordinates
    float phi = atan2_custom(viewDir.z, viewDir.x); // Azimuth angle
    float theta = asin(viewDir.y);                 // Inclination angle

    // Map spherical coordinates to UVs in the range [0, 1]ts
    vec2 uv;
    uv.x = phi / (2.0 * M_PI) + 0.5;  // Horizontal, azimuth
    uv.y = 1.0 - theta / M_PI + 0.5;   // Vertical, inclination

    //finalColor = GetPanoramaTexture(viewDir);
    finalColor.rgb = GaussianBlur(viewDir, 1);
    finalColor.a = 1.0;
}