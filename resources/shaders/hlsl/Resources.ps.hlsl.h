SamplerState g_sampler : register(s0, space3);

#include "Textures.hlsl.h"

struct PSInput {
    [[vk::location(0)]] float3 color : COLOR;
    [[vk::location(1)]] float2 texCoord : TEXCOORD;
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

Material material : register(b0, space4);

// Example usage of material in shader
float4 ComputeMaterialColor(float2 uv)
{
    float4 color = float4(0, 0, 0, 1); // Default color

    // Example: If the material contains a BASE_COLOR as a texture, sample it
    for (int i = 0; i < MAX_COMPONENT; i++)
    {
        if (material.components[i].valueType == TEXTURE)
        {
            color = GetMaterialTexture(i, uv);
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
    return color;
}