#define M_PI 3.14159265358979323846264

struct MaterialPBR {
    vec3 baseColor;
    float metallic;
    vec3 normal;
    float roughness;
    vec3 specular;
    float ao;
};

struct VSOutput {
    vec4 outPosition; // Equivalent to gl_Position in GLSL
    vec2 fragTexCoord;  // Equivalent to layout(location = 1) out in GLSL
    vec3 normal;
    vec4 fragColor;     // Equivalent to layout(location = 0) out in GLSL
    vec3 tangent;
    vec3 bitangent;
    vec3 worldPos;      // Equivalent to layout(location = 2) out in GLSL
};

layout(binding = 0, set = 1) uniform UniformBufferCamera {
    mat4 view;
    mat4 proj;
    vec3 cameraPos;
    vec3 cameraDirection;
};

layout(binding = 0, set = 5) uniform cb1 {
    float targetLuminance;
};

layout(binding = 1, set = 5) uniform cb2 {
    int screenWidth;
    int screenHeight;
    int normalMapDraw;
    int disableMetallic;
    int disableRoughness;
    float constant_metallic;
    float constant_roughness;
    int multisamplingSamples;
};

const int LightType_Directional = 0;
const int LightType_Point = 1;
const int LightType_Spot = 2;

struct Light {
    vec3 position;
    int type; // Use int to represent LightType
    vec3 color;
    float intensity;
    vec3 direction;
    float radius;
};

layout(binding = 0, set = 7) uniform cl1 {
    Light lights[256];
};

layout(binding = 1, set = 7) uniform cl2 {
    int lightCount;
};

// Problem with this thing
layout(binding = 2, set = 7) buffer forwardPlusResultBuffer {
    int data[];
};