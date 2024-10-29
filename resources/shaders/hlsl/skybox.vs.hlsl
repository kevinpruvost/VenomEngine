// HLSL Vertex Shader for Vulkan using DXC

#include "Resources.vs.hlsl.h"

struct VSOutput {
    float4 position : SV_Position;
    [[vk::location(0)]] float3 viewDir : TEXCOORD0;     // Equivalent to layout(location = 0) out in GLSL
};

struct PanoramaInput {
    [[vk::location(0)]] float3 inPosition : POSITION;
};

VSOutput main(PanoramaInput input)
{
    VSOutput output;

    // Convert NDC position to world space
    float4 worldspace = mul(inverse(view), mul(inverse(proj), float4(input.inPosition.xy, 1.0, 1.0)));

    // Normalize view direction for the fragment shader
    output.position = float4(input.inPosition.xyz, 1.0);
    //output.position = clipSpacePosition;
    output.viewDir = worldspace.xyz;
    return output;
}