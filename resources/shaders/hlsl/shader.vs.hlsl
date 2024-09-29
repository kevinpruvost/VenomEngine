// HLSL Vertex Shader for Vulkan using DXC

cbuffer UniformBufferObject : register(b0) {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};

// struct C
// {
//     float4x4 model;
// };
// Actually not cool, because they are not available on DX12 and Metal
// [[vk::push_constant]]
// C cameraPC;

struct VSInput {
    [[vk::location(0)]] float3 inPosition : POSITION;
    [[vk::location(1)]] float3 inColor : NORMAL;
    [[vk::location(2)]] float2 inTexCoord : TEXCOORD;
};

struct VSOutput {
    float4 outPosition : SV_POSITION; // Equivalent to gl_Position in GLSL
    [[vk::location(0)]] float3 fragColor : TEXCOORD0;     // Equivalent to layout(location = 0) out in GLSL
    [[vk::location(1)]] float2 fragTexCoord : TEXCOORD1;  // Equivalent to layout(location = 1) out in GLSL
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.outPosition = float4(input.inPosition, 1.0); // Set the position
    output.outPosition = mul(model, output.outPosition); // Apply the model matrix
    output.outPosition = mul(view, output.outPosition);  // Apply the view matrix
    output.outPosition = mul(proj, output.outPosition);  // Apply the projection matrix
    output.fragColor = input.inColor;                        // Pass the color to the fragment shader
    output.fragTexCoord = input.inTexCoord;                  // Pass the texture
    return output;
}