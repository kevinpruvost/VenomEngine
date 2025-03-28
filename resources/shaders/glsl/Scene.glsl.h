#define M_PI 3.14159265358979323846264
#define MAX_LIGHTS 128
#define CASCADE_COUNT 3
#define BITS_PER_INT 32
#define NUM_FORWARD_PLUS_INTS ((MAX_LIGHTS + BITS_PER_INT - 1) / BITS_PER_INT)
#define FORWARD_PLUS_BLOCK_SIZE 32
#define FLT_EPSILON 1.19209290e-7

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
const int DebugVisualizationMode_ShadowMapping = 4;

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

const float PointLight_Threshold = 1.0;
const float SpotLight_Threshold = 0.2;

vec3 SpotAndDirectionalDirection(vec3 direction) {
    // Rotate the default light direction (0, -1, 0) using the light's direction as Euler angles
    float cx = cos(direction.x);
    float sx = sin(direction.x);
    float cy = cos(direction.y);
    float sy = sin(direction.y);
    float cz = cos(direction.z);
    float sz = sin(direction.z);

    // Direct computation of the rotated direction
    vec3 dir;
    dir.x = -cy * sz;
    dir.y = -sx * sy * sz - cx * cz;
    dir.z = -cx * sy * sz + sx * cz;

    return -normalize(dir);
}

struct Light {
    vec3 position;
    int type; // Use int to represent LightType
    vec3 color;
    float intensity;
    vec3 direction;
    float angle;
	vec3 padding;
    int shadowMapIndex;
};

layout(binding = 0, set = 7) uniform cl1 {
    Light lights[MAX_LIGHTS];
    vec3 padding;
    int lightCount;
};

//layout(binding = 1, set = 7) uniform cl2 {
//    int lightCount;
//};

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
#if defined(COMPUTE_FORWARD_PLUS)
layout(binding = 2, set = 7) buffer forwardPlusResultBufferData {
#else
layout(binding = 2, set = 7) readonly buffer forwardPlusResultBufferData {
#endif
    LightBlock forwardPlusResultBuffer[];
};

bool isLightInBlock(vec2 uv, int lightIndex) {
    int intIndex = getLightBlockIntIndex(lightIndex);
    int bitIndex = getLightBlockBitIndex(lightIndex);

    uv = vec2(uv.x / graphicsSettings.extentWidth, uv.y / graphicsSettings.extentHeight);
    int blockIndex = int(uv.x * FORWARD_PLUS_BLOCK_SIZE) + int(uv.y * FORWARD_PLUS_BLOCK_SIZE) * FORWARD_PLUS_BLOCK_SIZE;
    return (forwardPlusResultBuffer[blockIndex].block[intIndex] & (1 << bitIndex)) != 0;
}