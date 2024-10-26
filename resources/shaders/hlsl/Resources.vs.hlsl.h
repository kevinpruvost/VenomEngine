cbuffer UniformBufferObject : register(b0, space0) {
    StructuredBuffer<float4x4> models;
};

cbuffer UniformBufferCamera : register(b0, space1) {
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
    [[vk::location(1)]] float3 inNormal : NORMAL;
    //    [[vk::location(2)]] float4 inColor : COLOR;
    [[vk::location(3)]] float2 inTexCoord : TEXCOORD;
    uint instanceID : SV_InstanceID;
};