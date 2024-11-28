#extension GL_EXT_nonuniform_qualifier : enable

// Bindless textures
layout(binding = 0, set = 2) uniform texture2D bindlessTextures[];
//layout(binding = 1, set = 2) uniform int isTextureHDR[];

layout(binding = 0, set = 3) uniform sampler g_sampler;

vec4 GetTexture(int u, vec2 texCoord) {
    return texture(sampler2D(bindlessTextures[u], g_sampler), texCoord);
}

layout(binding = 0, set = 6) uniform texture2D panoramaTexture;
layout(binding = 1, set = 6) uniform panoramaProps {
    float panoramaPeakLuminance;
};