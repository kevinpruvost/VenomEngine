#define M_PI 3.14159265358979323846264
#define MAX_LIGHTS 256
#define BITS_PER_INT 32
#define NUM_FORWARD_PLUS_INTS ((MAX_LIGHTS + BITS_PER_INT - 1) / BITS_PER_INT)
#define FORWARD_PLUS_BLOCK_SIZE 32

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

struct SceneSettings {
    float targetLuminance;
};

const int DebugVisualizationMode_None = 0;
const int DebugVisualizationMode_Depth = 1;
const int DebugVisualizationMode_Normals = 2;
const int DebugVisualizationMode_ForwardPlus = 3;

struct GraphicsSettings {
    int screenWidth;
    int screenHeight;
    int extentWidth;
    int extentHeight;
    int multisamplingMode;
    int multisamplingSamples;
    int hdrEnabled;
    int debugVisualizationMode;
};

layout(binding = 0, set = 5) uniform cb1 {
    SceneSettings sceneSettings;
};

layout(binding = 1, set = 5) uniform cb2 {
    GraphicsSettings graphicsSettings;
};

const int LightType_Directional = 0;
const int LightType_Point = 1;
const int LightType_Spot = 2;

const float PointLight_Threshold = 0.2;

struct Light {
    vec3 position;
    int type; // Use int to represent LightType
    vec3 color;
    float intensity;
    vec3 direction;
    float angle;
};

layout(binding = 0, set = 7) uniform cl1 {
    Light lights[MAX_LIGHTS];
};

layout(binding = 1, set = 7) uniform cl2 {
    int lightCount;
};

struct LightBlock {
    int block[NUM_FORWARD_PLUS_INTS];
};

LightBlock initLightBlock() {
    LightBlock block;
    for (int i = 0; i < NUM_FORWARD_PLUS_INTS; i++) {
        block.block[i] = 0;
    }
    return block;
}

int getLightBlockIntIndex(int lightIndex) {
    return lightIndex / BITS_PER_INT;
}

int getLightBlockBitIndex(int lightIndex) {
    return lightIndex % BITS_PER_INT;
}

// Problem with this thing
layout(binding = 2, set = 7) buffer forwardPlusResultBufferData {
    LightBlock forwardPlusResultBuffer[];
};

bool isLightInBlock(vec2 uv, int lightIndex) {
    int intIndex = getLightBlockIntIndex(lightIndex);
    int bitIndex = getLightBlockBitIndex(lightIndex);

    uv = vec2(uv.x / graphicsSettings.extentWidth, uv.y / graphicsSettings.extentHeight);
    int blockIndex = int(uv.x * FORWARD_PLUS_BLOCK_SIZE) + int(uv.y * FORWARD_PLUS_BLOCK_SIZE) * FORWARD_PLUS_BLOCK_SIZE;
    return (forwardPlusResultBuffer[blockIndex].block[intIndex] & (1 << bitIndex)) != 0;
}