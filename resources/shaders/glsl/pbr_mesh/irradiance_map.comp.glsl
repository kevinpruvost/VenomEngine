///
/// Project: VenomEngineWorkspace
/// @file irradiance_map.comp.glsl
/// @date Dec, 17 2024
/// @brief
/// author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#version 450

layout(binding = 2, set = 4, rgba16f) uniform image2D irradianceMap;
layout(binding = 0, set = 3) uniform sampler g_sampler;
layout(binding = 0, set = 6) uniform texture2D panoramaTexture;

#define PI 3.1415926535897932384626433832795

vec3 sampleEnvironment(vec3 dir) {
    // Convert direction vector to UV coordinates for equirectangular map
    float u = atan(dir.z, dir.x) / (2.0 * PI) + 0.5;
    float v = asin(dir.y) / PI + 0.5;
    return texture(sampler2D(panoramaTexture, g_sampler), vec2(u, v)).rgb;
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(irradianceMap);

    vec2 uv = vec2(coords) / vec2(size);

    // Map UV to hemisphere directions (Lambert equal-area projection)
    float phi = uv.x * 2.0 * PI;  // Longitude
    float theta = uv.y * PI;      // Latitude

	vec3 N = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, N));
	up = cross(N, right);

	const float TWO_PI = PI * 2.0;
	const float HALF_PI = PI * 0.5;

	uint sampleCount = 128u;
    vec3 irradiance = vec3(0.0);
    for (int i = 0; i < sampleCount; ++i) {
        float sampleTheta = (float(i) / float(sampleCount)) * PI / 2.0;
        float samplePhi = (float(i) / float(sampleCount)) * 2.0 * PI;

        vec3 sampleDir = vec3(
            sin(sampleTheta) * cos(samplePhi),
            cos(sampleTheta),
            sin(sampleTheta) * sin(samplePhi)
        );

        float weight = max(dot(sampleDir, N), 0.0); // Lambertian weighting
        irradiance += sampleEnvironment(sampleDir) * weight;
    }
	irradiance = PI * irradiance / float(sampleCount);

	imageStore(irradianceMap, coords, vec4(irradiance, 1.0));
}