// HLSL Vertex Shader for Vulkan using DXC

#include "Resources.vs.hlsl.h"

struct VSOutput {
    float4 position : SV_POSITION;
    [[vk::location(0)]] float2 uv : TEXCOORD0;
};

struct PanoramaInput {
    [[vk::location(0)]] float4 inPosition : POSITION;
};

VSOutput main(PanoramaInput input)
{
    VSOutput output;
    output.position = input.inPosition;
    output.uv.x = (input.inPosition.x + 1.0) / 2.0;
    output.uv.y = (input.inPosition.y + 1.0) / 2.0;
    return output;
}