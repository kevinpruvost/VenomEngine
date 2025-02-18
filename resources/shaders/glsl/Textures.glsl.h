#include "Scene.glsl.h"

#extension GL_EXT_nonuniform_qualifier : enable

// Bindless textures
layout(binding = 0, set = 2) uniform texture2D bindlessTextures[];
//layout(binding = 1, set = 2) uniform int isTextureHDR[];

layout(binding = 1, set = 1) uniform sampler g_sampler;
layout(binding = 2, set = 1) uniform sampler g_ClampSampler;

vec4 GetTexture(int u, vec2 texCoord) {
    return texture(sampler2D(bindlessTextures[u], g_sampler), texCoord);
}

// Converts a color from linear light gamma to sRGB gamma
vec4 fromLinear(vec4 linearRGB)
{
    bvec3 cutoff = lessThan(linearRGB.rgb, vec3(0.0031308));
    vec3 higher = vec3(1.055)*pow(linearRGB.rgb, vec3(1.0/2.4)) - vec3(0.055);
    vec3 lower = linearRGB.rgb * vec3(12.92);

    return vec4(mix(higher, lower, cutoff), linearRGB.a);
}

vec4 toLinear(vec4 sRGB)
{
    bvec3 cutoff = lessThan(sRGB.rgb, vec3(0.04045));
    vec3 higher = pow((sRGB.rgb + vec3(0.055)) / vec3(1.055), vec3(2.4));
    vec3 lower = sRGB.rgb / vec3(12.92);

    return vec4(mix(higher, lower, cutoff), sRGB.a);
}

vec3 toLinear(vec3 sRGB)
{
    bvec3 cutoff = lessThan(sRGB.rgb, vec3(0.04045));
    vec3 higher = pow((sRGB.rgb + vec3(0.055)) / vec3(1.055), vec3(2.4));
    vec3 lower = sRGB.rgb / vec3(12.92);

    return vec3(mix(higher, lower, cutoff));
}

layout(binding = 0, set = 6) uniform texture2D panoramaTexture;
layout(binding = 1, set = 6) uniform panoProps {
    float panoramaPeakLuminance;
    float panoramaAverageLuminance;
    float blurFactor;
} panoramaProps;
layout(binding = 2, set = 6) uniform texture2D panoramaIrradianceMap;
layout(binding = 3, set = 6) uniform texture2D panoramaRadianceMap;
layout(binding = 4, set = 6) uniform texture2D panoramaBlurMap;

float atan2_custom(in float y, in float x)
{
    bool s = (abs(x) > abs(y));
    return mix(M_PI/2.0 - atan(x,y), atan(y,x), s);
}

vec2 PanoramaUvFromDir(vec3 dir) {
    // Normalize the view direction
    vec3 viewDir = normalize(dir);
    // Convert the view direction to spherical coordinates
    float phi = atan2_custom(viewDir.z, viewDir.x); // Azimuth angle
    float theta = asin(viewDir.y);                 // Inclination angle

    // Map spherical coordinates to UVs in the range [0, 1]
    vec2 uv;
    uv.x = phi / (2.0 * M_PI) + 0.5;  // Horizontal, azimuth
    uv.y = 1.0 - theta / M_PI + 0.5;   // Vertical, inclination

    return uv;
}

vec4 GetPanoramaTexture(vec2 uv) {
    // Sample the texture using the provided UV coordinates
    vec4 baseColor = texture(sampler2D(panoramaTexture, g_sampler), uv);
    vec4 blurColor = texture(sampler2D(panoramaBlurMap, g_sampler), uv);
    vec4 color = mix(baseColor, blurColor, panoramaProps.blurFactor);
    float exposure = sceneSettings.targetLuminance / panoramaProps.panoramaPeakLuminance;
    color = vec4(color.rgb * exposure, 1.0); // Applying exposure factor

    return color;
}

vec4 GetPanoramaTexture(vec3 dir) {
    vec2 uv = PanoramaUvFromDir(dir);
    // Sample the texture using the provided UV coordinates
    return GetPanoramaTexture(uv);
}

vec4 GetPanoramaRadiance(vec3 dir, float roughness) {
    vec2 uv = PanoramaUvFromDir(dir);
    // CHECK LOD
    float lod = min((roughness * 10.0), 10.0);
    vec4 color = textureLod(sampler2D(panoramaRadianceMap, g_sampler), uv, lod);
    float exposure = sceneSettings.targetLuminance / panoramaProps.panoramaPeakLuminance;
    color = vec4(color.rgb * exposure, 1.0); // Applying exposure factor

    return color;
}

vec4 GetPanoramaIrradiance(vec3 dir) {
    vec2 uv = PanoramaUvFromDir(dir);
    vec4 color = texture(sampler2D(panoramaIrradianceMap, g_sampler), uv);
    float exposure = sceneSettings.targetLuminance / panoramaProps.panoramaPeakLuminance;
    color = vec4(color.rgb * exposure, 1.0); // Applying exposure factor

    return color;
}