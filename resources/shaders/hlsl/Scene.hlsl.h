#define M_PI 3.14159265358979323846264

struct MaterialPBR
{
    float3 baseColor;
    float metallic;
    float3 normal;
    float roughness;
    float3 specular;
    float ao;
};

struct VSInput {
    [[vk::location(0)]] float3 inPosition : POSITION;
    [[vk::location(1)]] float3 inNormal : NORMAL;
    [[vk::location(2)]] float2 inTexCoord : TEXCOORD;
    [[vk::location(3)]] float3 inTangent : TANGENT;
    [[vk::location(4)]] float3 inBitangent : BITANGENT;
    uint instanceID : SV_InstanceID;
};

struct VSOutput {
    float4 outPosition : SV_POSITION; // Equivalent to gl_Position in GLSL
    float2 fragTexCoord : TEXCOORD0;  // Equivalent to layout(location = 1) out in GLSL
    float3 normal : NORMAL;
    float4 fragColor : TEXCOORD1;     // Equivalent to layout(location = 0) out in GLSL
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float3 worldPos : TEXCOORD2;
};

cbuffer UniformBufferCamera : register(b0, space1) {
    float4x4 view;
    float4x4 proj;
    float3 cameraPos;
    float3 cameraDirection;
};

cbuffer cb1 : register(b0, space5) {
    float targetLuminance;
};

cbuffer cb2 : register(b1, space5) {
    int screenWidth;
    int screenHeight;
    int normalMapDraw;
    int disableMetallic;
    int disableRoughness;
    float constant_metallic;
    float constant_roughness;
    int multisamplingSamples;
};

enum class LightType
{
    Directional,
    Point,
    Spot
};

struct Light
{
    float3 position;
    LightType type;
    float3 color;
    float intensity;
    float3 direction;
    float radius;
};
cbuffer cl1 : register(b0, space7) {
    Light lights[256];
};
cbuffer cl2 : register(b1, space7) {
    int lightCount;
};
// Problem with this thing
RWStructuredBuffer<int> forwardPlusResultBuffer : register(u2, space7);