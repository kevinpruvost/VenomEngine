// HLSL Vertex Shader for Vulkan using DXC

#include "Resources.vs.hlsl.h"

struct LocalVSOutput {
    float4 outPosition : SV_POSITION; // Equivalent to gl_Position in GLSL
    [[vk::location(0)]] float4 fragColor : TEXCOORD0;     // Equivalent to layout(location = 0) out in GLSL
    [[vk::location(1)]] float2 fragTexCoord : TEXCOORD1;  // Equivalent to layout(location = 1) out in GLSL
};

LocalVSOutput main(VSInput input)
{
    LocalVSOutput output;
    output.outPosition = float4(input.inPosition, 1.0); // Set the position
    output.outPosition = mul(models[input.instanceID], output.outPosition); // Apply the model matrix
    output.outPosition = mul(view, output.outPosition);  // Apply the view matrix
    output.outPosition = mul(proj, output.outPosition);  // Apply the projection matrix
    output.fragColor = float4(1.0, 1.0, 1.0, 1.0);       // Pass the color to the fragment shader
    output.fragTexCoord = input.inTexCoord;              // Pass the texture
    return output;
}