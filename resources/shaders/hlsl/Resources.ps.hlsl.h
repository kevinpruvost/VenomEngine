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
    float4 baseColor       : SV_Target0; // Base color with optional alpha for opacity or transmission
    float3 normal          : SV_Target1; // World or view-space normal
    float2 metallicRough   : SV_Target2; // Metallic and roughness parameters
    float4 position        : SV_Target3; // Position in world space (or depth if reconstructing later)
    float ao               : SV_Target4; // Ambient occlusion factor (optional)

    // Extended components
    float4 specularReflect : SV_Target5; // Specular color and reflectivity
    float4 emissionColor   : SV_Target6; // Emission color
    float2 clearcoatSheen  : SV_Target7; // Clearcoat and sheen for cloth or coated materials
    float2 anisotropyTrans : SV_Target8; // Anisotropy and transmission
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

    // Example: If the material contains a BASE_COLOR as a texture, sample it
    for (int i = 0; i < MAX_COMPONENT; i++)
    {
        if (material.components[i].valueType == TEXTURE)
        {
            color = GetMaterialTexture(i, uv);
            break;
        }
        else if (material.components[i].valueType == COLOR4D)
        {
            color = material.components[i].value;
        }
        else if (material.components[i].valueType == COLOR3D)
        {
            color = float4(material.components[i].value.xyz, 1.0);
        }
        else if (material.components[i].valueType == VALUE)
        {
            color = float4(material.components[i].value.x, material.components[i].value.x, material.components[i].value.x, 1.0);
        }
    }
    output.baseColor = color;
    return output;
}