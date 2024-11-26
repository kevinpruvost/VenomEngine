SamplerState g_sampler : register(s0, space3);

#include "Textures.hlsl.h"
#include "Scene.hlsl.h"

struct GBufferOutput {
    float4 finalColor       : SV_Target0; // Final color
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
    float2 textureRepeatFactor;
};

cbuffer materialProps : register(b0, space4) {
    Material material;
};

// Function to get a texture value from a material component
float4 GetMaterialTexture(int componentType, float2 uv)
{
    return GetTexture(componentType,uv * material.textureRepeatFactor);
}

float MaterialComponentGetValue1(int componentType, float2 uv)
{
    float4 value;
    if (material.components[componentType].valueType == TEXTURE)
        value = GetMaterialTexture(componentType, uv);
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
        value = GetMaterialTexture(componentType, uv);
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
        value = GetMaterialTexture(componentType, uv);
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
        return GetMaterialTexture(componentType, uv);
    return material.components[componentType].value;
}