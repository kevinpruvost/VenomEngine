SamplerState g_sampler : register(s0, space3);

#include "Textures.hlsl.h"
#include "Scene.hlsl.h"

struct PSInput {
    float4 position : SV_POSITION;
    [[vk::location(0)]] float3 color : COLOR;
    [[vk::location(1)]] float2 texCoord : TEXCOORD;
    [[vk::location(2)]] float3 normal : NORMAL;
    //[[vk::location(3)]] float3 tangent : TANGENT;
    //[[vk::location(4)]] float3 bitangent : BITANGENT;
};

struct GBufferOutput {
    float4 baseColor        : SV_Target0; // Base color with optional alpha for opacity or transmission
    float4 normalSpecular   : SV_Target1; // World or view-space normal
    float4 metallicRoughAo  : SV_Target2; // Metallic and roughness parameters
    float4 position         : SV_Target3; // Position in world space (or depth if reconstructing later)
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

// Structure for material component
struct MaterialComponent
{
    float4 value;      // Generic value, can be interpreted based on valueType
    int valueType;     // Type of value stored (color3D, color4D, float, texture)
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
    if (material.components[componentType].valueType == TEXTURE)
        return GetTexture(componentType, uv).x;
        return material.components[componentType].value.x;
}

float2 MaterialComponentGetValue2(int componentType, float2 uv)
{
    if (material.components[componentType].valueType == TEXTURE)
        return GetTexture(componentType, uv).xy;
    return material.components[componentType].value.xy;
}

float3 MaterialComponentGetValue3(int componentType, float2 uv)
{
    if (material.components[componentType].valueType == TEXTURE)
        return GetTexture(componentType, uv).xyz;
    return material.components[componentType].value.xyz;
}

float4 MaterialComponentGetValue4(int componentType, float2 uv)
{
    if (material.components[componentType].valueType == TEXTURE)
        return GetTexture(componentType, uv);
    return material.components[componentType].value;
}

// Example usage of material in shader
GBufferOutput ComputeMaterialColor(PSInput input)
{
    float2 uv = input.texCoord;
    GBufferOutput output;
    float4 color = float4(0.5, 0.5, 0.5, 1); // Default color
    //return GetTexture(1, uv);

    // Base color (if PBR, then BASE_COLOR otherwise approx. of DIFFUSE)
    if (material.components[MaterialComponentType::BASE_COLOR].valueType != NONE)
        output.baseColor = MaterialComponentGetValue4(MaterialComponentType::BASE_COLOR, uv);
    else
        output.baseColor = MaterialComponentGetValue4(MaterialComponentType::DIFFUSE, uv);

    // Normal
    output.normalSpecular.rgb = MaterialComponentGetValue3(MaterialComponentType::NORMAL, uv);
    // Specular
    output.normalSpecular.w = MaterialComponentGetValue1(MaterialComponentType::SPECULAR, uv);

    // Metallic (if PBR, then METALLIC otherwise 0)
    if (material.components[MaterialComponentType::METALLIC].valueType != NONE)
        output.metallicRoughAo[0] = MaterialComponentGetValue1(MaterialComponentType::METALLIC, uv);
    else
        output.metallicRoughAo[0] = 0.0;
    // Roughness (if PBR, then ROUGHNESS otherwise square of 2/(SPECULAR+2))
    if (material.components[MaterialComponentType::ROUGHNESS].valueType != NONE)
        output.metallicRoughAo[1] = MaterialComponentGetValue1(MaterialComponentType::ROUGHNESS, uv);
    else if (material.components[MaterialComponentType::SPECULAR].valueType != NONE)
        output.metallicRoughAo[1] = sqrt(2.0 / (MaterialComponentGetValue1(MaterialComponentType::SPECULAR, uv) + 2));
    else
        output.metallicRoughAo[1] = 0.5;
    // Ambient occlusion
    if (material.components[MaterialComponentType::AMBIENT_OCCLUSION].valueType != NONE)
        output.metallicRoughAo[2] = MaterialComponentGetValue1(MaterialComponentType::AMBIENT_OCCLUSION, uv);
    else
        output.metallicRoughAo[2] = 1.0;

    // Position
    output.position = float4(input.position.xyz, 1);


    return output;
}