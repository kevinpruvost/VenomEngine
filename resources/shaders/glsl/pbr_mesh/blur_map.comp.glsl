///
/// Project: VenomEngineWorkspace
/// @file blur_map.comp.glsl
/// @date Feb, 16 2024
/// @brief
/// author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#version 450

layout(binding = 8, set = 4, rgba16f) uniform image2D blurMap;
layout(binding = 1, set = 1) uniform sampler g_sampler;
layout(binding = 0, set = 6) uniform texture2D panoramaTexture;

const float M_PI = 3.1415926535897932384626433832795;

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
vec3 GaussianBlur(vec2 uv)
{
    // Calculate texel size (offset between each texel)
    ivec2 size = imageSize(blurMap);
    int width = size.x;
    int height = size.y;
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
            vec4 sampleColor = texture(sampler2D(panoramaTexture, g_sampler), sampleUv);
            kernelSum += kernel[(i + halfSize) * kernelSize + (j + halfSize)];
            finalColor += sampleColor.rgb * (kernel[(i + halfSize) * kernelSize + (j + halfSize)]);
        }
    }
    finalColor /= kernelSum;

    return finalColor; // Return the blurred color
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(blurMap);

    vec2 uv = vec2(coords) / vec2(size);

    vec3 blur = GaussianBlur(uv);
	imageStore(blurMap, coords, vec4(blur, 1.0));
}