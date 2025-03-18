///
/// Project: VenomEngineWorkspace
/// @file irradiance_map.comp.glsl
/// @date Dec, 17 2024
/// @brief
/// author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#version 450

layout(binding = 8, set = 4, rgba16f) uniform image2D irradianceMap;
layout(binding = 3, set = 4) uniform radianceMapVars {
    float roughness;
};
layout(binding = 1, set = 1) uniform sampler g_sampler;
layout(binding = 0, set = 6) uniform texture2D panoramaTexture;

#define PI 3.1415926535897932384626433832795
#define NUM_SAMPLES 256

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

// Based omn http://byteblacksmith.com/improvements-to-the-canonical-one-liner-glsl-rand-for-opengl-es-2-0/
float random(vec2 co)
{
	float a = 12.9898;
	float b = 78.233;
	float c = 43758.5453;
	float dt= dot(co.xy ,vec2(a,b));
	float sn= mod(dt,3.14);
	return fract(sin(sn) * c);
}

vec2 hammersley2d(uint i, uint N)
{
	// Radical inverse based on http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
	uint bits = (i << 16u) | (i >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	float rdi = float(bits) * 2.3283064365386963e-10;
	return vec2(float(i) /float(N), rdi);
}

// Based on http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_slides.pdf
vec3 importanceSample_GGX(vec2 Xi, float roughness, vec3 normal)
{
	// Maps a 2D point to a hemisphere with spread based on roughness
	float alpha = roughness * roughness;
	float phi = 2.0 * PI * Xi.x + random(normal.xz) * 0.1;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (alpha*alpha - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	vec3 H = vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);

	// Tangent space
	vec3 up = abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangentX = normalize(cross(up, normal));
	vec3 tangentY = normalize(cross(normal, tangentX));

	// Convert to world Space
	return normalize(tangentX * H.x + tangentY * H.y + normal * H.z);
}

// Normal Distribution function
float D_GGX(float dotNH, float roughness)
{
	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha;
	float denom = dotNH * dotNH * (alpha2 - 1.0) + 1.0;
	return (alpha2)/(PI * denom*denom);
}

// Prefilter the environment map based on roughness
vec3 prefilterEnvMap(vec3 N, float roughness) {
    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;

    if (roughness == 0.0) {
        return texture(sampler2D(panoramaTexture, g_sampler), vec2(atan(N.y, N.x) / (2.0 * PI) + 0.5, acos(N.z) / PI)).rgb;
    }

    vec3 V = N;

    for (int i = 0; i < NUM_SAMPLES; ++i) {
        vec2 xi = hammersley2d(i, NUM_SAMPLES);
        vec3 H = importanceSample_GGX(xi, roughness, N);
        vec3 L = 2.0 * dot(V, H) * H - V;

        float NdotL = clamp(dot(N, L), 0.0, 1.0);
        if (NdotL > 0.0) {
            vec2 uv = vec2(atan(L.y, L.x) / (2.0 * PI) + 0.5, acos(L.z) / PI);

            vec3 texColor = texture(sampler2D(panoramaTexture, g_sampler), uv).rgb;
            prefilteredColor += texColor * NdotL;
            totalWeight += NdotL;
        }
    }

    return prefilteredColor / totalWeight;
}

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(irradianceMap);

    vec2 uv = vec2(coords) / vec2(size);
    uv.y = 1.0 - uv.y;

    // Map UV to hemisphere directions (Lambert equal-area projection)
    float phi = uv.x * 2.0 * PI - PI;  // Longitude
    float theta = PI - uv.y * PI;      // Latitude

    vec3 N = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

    // Compute prefiltered environment map color
    vec3 prefilteredColor = prefilterEnvMap(N, roughness);

    // Write result to output image
    imageStore(irradianceMap, coords, vec4(prefilteredColor, 1.0));
}