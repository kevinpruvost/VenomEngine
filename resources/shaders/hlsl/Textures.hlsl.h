#define VENOM_TEXTURES 32

#ifndef MACOS
// Bindless textures
Texture2D bindlessTextures[] : register(t0, space2);
#else
// Dynamically bound textures
Texture2D dynamicTextures[VENOM_TEXTURES] : register(t0, space2);
#endif

float4 GetTexture(int u, float2 texCoord, SamplerState sampler = g_sampler)
{
#ifndef MACOS
        return bindlessTextures[u].Sample(sampler, texCoord);
#else
        return dynamicTextures[u].Sample(sampler, texCoord);
#endif
}