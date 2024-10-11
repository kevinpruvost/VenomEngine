struct PSInput {
    [[vk::location(0)]] float3 color : COLOR;
    [[vk::location(1)]] float2 texCoord : TEXCOORD;
};

SamplerState g_sampler : register(s2);
Texture2D g_texture : register(t3);

float4 main(PSInput input) : SV_TARGET {
    return float4(g_texture.Sample(g_sampler, input.texCoord));
}
