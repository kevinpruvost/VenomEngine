// Bindless textures
Texture2D bindlessTextures[] : register(t0, space2);
//StructuredBuffer<int> isTextureHDR : register(t1, space2);

float4 GetTexture(int u, float2 texCoord, SamplerState sampler = g_sampler)
{
    //if (isTextureHDR[u])
        return bindlessTextures[u].Sample(sampler, texCoord);
    //else
        return bindlessTextures[u].Sample(sampler, texCoord);
}

Texture2D panoramaTexture : register(t0, space6);
cbuffer panoramaProps : register(b1, space6) {
    float panoramaPeakLuminance;
};