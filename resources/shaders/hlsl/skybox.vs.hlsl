// HLSL Vertex Shader for Vulkan using DXC

#include "Resources.vs.hlsl.h"

struct VSOutput {
    [[vk::location(0)]] float3 viewDir : TEXCOORD0;     // Equivalent to layout(location = 0) out in GLSL
};

struct PanoramaInput {
    [[vk::location(0)]] float3 inPosition : POSITION;
};

VSOutput main(PanoramaInput input)
{
    VSOutput output;

    // Transform the position into a view direction
    float4 worldPos = mul(float4(input.inPosition, 1.0), proj);
    float4 viewPos = mul(worldPos, view);

    output.viewDir = normalize(viewPos.xyz);
    return output;
}