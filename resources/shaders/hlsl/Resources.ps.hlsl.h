SamplerState g_sampler : register(s0, space3);

#include "Textures.hlsl.h"
#include "Scene.hlsl.h"

struct GBufferOutput {
    float4 finalColor       : SV_Target0; // Final color
    float4 baseColor        : SV_Target1; // Base color with optional alpha for opacity or transmission
    float4 normal           : SV_Target2; // World or view-space normal
    float4 metallicRoughAo  : SV_Target3; // Metallic and roughness parameters
    float4 position         : SV_Target4; // Position in world space (or depth if reconstructing later)
    float4 specular         : SV_Target5; // Specular color
};

// Materials

enum MaterialComponentType
{
    AMBIENT = 0,
    DIFFUSE = 1,
    SPECULAR = 2,
    EMISSIVE = 3,
    SHININESS = 4,
    OPACITY = 5,
    NORMAL = 6,
    HEIGHT = 7,
    REFLECTION = 8,
    REFLECTIVITY = 9,
    REFRACTION = 10,
    TRANSPARENT = 11,
    ANISOTROPY = 12,
    BASE_COLOR = 13,
    METALLIC = 14,
    ROUGHNESS = 15,
    AMBIENT_OCCLUSION = 16,
    EMISSION_COLOR = 17,
    TRANSMISSION = 18,
    SHEEN = 19,
    CLEARCOAT = 20,
    MAX_COMPONENT = 21,
};

enum MaterialComponentValueType
{
    NONE = 0,
    COLOR3D = 1,
    COLOR4D = 2,
    VALUE = 3,
    TEXTURE = 4,
};

enum MaterialComponentValueChannels
{
    R = 0b1,
    G = 0b10,
    B = 0b100,
    A = 0b1000,
    RG = R | G,
    GB = G | B,
    BA = B | A,
    RGB = R | G | B,
    RGBA = R | G | B | A,
};

// Structure for material component
struct MaterialComponent
{
    float4 value;      // Generic value, can be interpreted based on valueType
    int valueType;     // Type of value stored (color3D, color4D, float, texture)
    int channels;      // Which channels are used
};

struct Material
{
    MaterialComponent components[MAX_COMPONENT];
};

// Function to get a texture value from a material component
float4 GetMaterialTexture(int componentType, float2 uv)
{
    return GetTexture(componentType,uv);
}

cbuffer materialProps : register(b0, space4) {
    Material material;
};

float MaterialComponentGetValue1(int componentType, float2 uv)
{
    float4 value;
    if (material.components[componentType].valueType == TEXTURE)
        value = GetTexture(componentType, uv);
    else
        value = material.components[componentType].value;

    if (material.components[componentType].channels & G)
        return value.g;
    else if (material.components[componentType].channels & B)
        return value.b;
    else if (material.components[componentType].channels & A)
        return value.a;
    else
        return value.r;
}

float2 MaterialComponentGetValue2(int componentType, float2 uv)
{
    float4 value;
    if (material.components[componentType].valueType == TEXTURE)
        value = GetTexture(componentType, uv);
    else
        value = material.components[componentType].value;
    if (material.components[componentType].channels & G && material.components[componentType].channels & B)
        return value.gb;
    else if (material.components[componentType].channels & B && material.components[componentType].channels & A)
        return value.ba;
    else if (material.components[componentType].channels & G && material.components[componentType].channels & A)
        return value.ga;
    else if (material.components[componentType].channels & R && material.components[componentType].channels & G)
        return value.rg;
    else if (material.components[componentType].channels & R && material.components[componentType].channels & B)
        return value.rb;
    else if (material.components[componentType].channels & R && material.components[componentType].channels & A)
        return value.ra;
    else
        return value.rg;
}

float3 MaterialComponentGetValue3(int componentType, float2 uv)
{
    float4 value;
    if (material.components[componentType].valueType == TEXTURE)
        value = GetTexture(componentType, uv);
    else
        value = material.components[componentType].value;

    if (material.components[componentType].channels == RGB)
        return value.rgb;
    else
        return value.gba;
}

float4 MaterialComponentGetValue4(int componentType, float2 uv)
{
    if (material.components[componentType].valueType == TEXTURE)
        return GetTexture(componentType, uv);
    return material.components[componentType].value;
}

// Example usage of material in shader
GBufferOutput ComputeMaterialColor(VSOutput input)
{
    float2 uv = input.fragTexCoord;
    GBufferOutput output;
    float4 color = float4(0.5, 0.5, 0.5, 1); // Default color

    // Base color (if PBR, then BASE_COLOR otherwise approx. of DIFFUSE)
    if (material.components[MaterialComponentType::BASE_COLOR].valueType != NONE)
        output.baseColor = MaterialComponentGetValue4(MaterialComponentType::BASE_COLOR, uv);
    else
        output.baseColor = MaterialComponentGetValue4(MaterialComponentType::DIFFUSE, uv);

    // Normal
    if (material.components[MaterialComponentType::NORMAL].valueType != NONE) {
        float3 q1 = ddx(input.worldPos.xyz);
        float3 q2 = ddy(input.worldPos.xyz);
        float2 st1 = ddx(uv);
        float2 st2 = ddy(uv);

        float3 T = normalize(q1 * st2.y - q2 * st1.y);
        //T = normalize(T - dot(T, input.normal) * input.normal);

        float3 B = normalize(cross(input.normal, T));

        T = normalize(cross(input.normal, float3(0.0, 1.0, 0.0)));
        B = normalize(cross(input.normal, T));
        float3x3 TBN = float3x3(T, B, input.normal);
        //float3x3 TBN = float3x3(input.tangent, input.bitangent, input.normal);
        float3 normal = normalize(mul(MaterialComponentGetValue3(MaterialComponentType::NORMAL, uv), TBN));
        //float3 normal = MaterialComponentGetValue3(MaterialComponentType::NORMAL, uv) * input.normal;
        output.normal = float4(normal, 1.0);
        //output.normal = float4(input.normal, 1.0);
    } else
        output.normal = float4(input.normal, 1.0);
    // Specular
    if (material.components[MaterialComponentType::SPECULAR].valueType != NONE)
        output.specular = MaterialComponentGetValue4(MaterialComponentType::SPECULAR, uv);
    else
        output.specular = float4(0.5, 0.5, 0.5, 1.0);

    // Metallic (if PBR, then METALLIC otherwise 0)
    if (material.components[MaterialComponentType::METALLIC].valueType != NONE)
        output.metallicRoughAo[0] = MaterialComponentGetValue1(MaterialComponentType::METALLIC, uv);
    else
        output.metallicRoughAo[0] = 0.0;
    // Roughness (if PBR, then ROUGHNESS otherwise square of 2/(SPECULAR+2))
    if (material.components[MaterialComponentType::ROUGHNESS].valueType != NONE)
        output.metallicRoughAo[1] = MaterialComponentGetValue1(MaterialComponentType::ROUGHNESS, uv);
    else if (material.components[MaterialComponentType::SHININESS].valueType != NONE)
        output.metallicRoughAo[1] = MaterialComponentGetValue1(MaterialComponentType::SHININESS, uv);
    else
        output.metallicRoughAo[1] = 0.5;
    // Ambient occlusion
    if (material.components[MaterialComponentType::AMBIENT_OCCLUSION].valueType != NONE)
        output.metallicRoughAo[2] = MaterialComponentGetValue1(MaterialComponentType::AMBIENT_OCCLUSION, uv);
    else
        output.metallicRoughAo[2] = 1.0;
    output.metallicRoughAo[3] = 1.0;

    // Position
    output.position = float4(input.worldPos, 1);
    output.finalColor = float4(0.0, 0.0, 0.0, 0.0);

    return output;
}