#define M_PI 3.14159265358979323846264

cbuffer UniformBufferCamera : register(b0, space1) {
    float4x4 view;
    float4x4 proj;
};

cbuffer cb1 : register(b0, space5) {
    float targetLuminance;
};

struct ScreenProps
{
    int width;
    int height;
};
cbuffer cb2 : register(b1, space5) {
    ScreenProps screenProps;
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
    float radius;
};
cbuffer cl1 : register(b0, space7) {
    Light lights[128];
};
cbuffer cl2 : register(b1, space7) {
    int lightCount;
};
// Problem with this thing
RWStructuredBuffer<int> forwardPlusResultBuffer : register(u2, space7);