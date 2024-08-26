// HLSL Vertex Shader for Vulkan using DXC

struct VSInput {
    [[vk::location(0)]] float3 inPosition : POSITION;
    [[vk::location(1)]] float3 inColor : NORMAL;
};

struct VSOutput {
    float4 outPosition : SV_POSITION; // Equivalent to gl_Position in GLSL
    [[vk::location(0)]] float3 fragColor : TEXCOORD0;     // Equivalent to layout(location = 0) out in GLSL
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.outPosition = float4(input.inPosition, 1.0); // Set the position
    output.fragColor = input.inColor;                        // Pass the color to the fragment shader
    return output;
}