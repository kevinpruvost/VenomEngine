#extension GL_EXT_nonuniform_qualifier : enable

// Bindless textures
layout(binding = 0, set = 2) uniform texture2D bindlessTextures[];
//layout(binding = 1, set = 2) uniform int isTextureHDR[];

layout(binding = 0, set = 3) uniform sampler g_sampler;

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

layout(binding = 0, set = 6) uniform texture2D panoramaTexture;
layout(binding = 1, set = 6) uniform panoramaProps {
    float panoramaPeakLuminance;
};