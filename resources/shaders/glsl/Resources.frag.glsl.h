#include "Textures.glsl.h"

layout(location = 0) out vec4 finalColor;

// Materials

const int MaterialComponentType_AMBIENT = 0;
const int MaterialComponentType_DIFFUSE = 1;
const int MaterialComponentType_SPECULAR = 2;
const int MaterialComponentType_EMISSIVE = 3;
const int MaterialComponentType_SHININESS = 4;
const int MaterialComponentType_OPACITY = 5;
const int MaterialComponentType_NORMAL = 6;
const int MaterialComponentType_HEIGHT = 7;
const int MaterialComponentType_REFLECTION = 8;
const int MaterialComponentType_REFLECTIVITY = 9;
const int MaterialComponentType_REFRACTION = 10;
const int MaterialComponentType_TRANSPARENT = 11;
const int MaterialComponentType_ANISOTROPY = 12;
const int MaterialComponentType_BASE_COLOR = 13;
const int MaterialComponentType_METALLIC = 14;
const int MaterialComponentType_ROUGHNESS = 15;
const int MaterialComponentType_AMBIENT_OCCLUSION = 16;
const int MaterialComponentType_EMISSION_COLOR = 17;
const int MaterialComponentType_TRANSMISSION = 18;
const int MaterialComponentType_SHEEN = 19;
const int MaterialComponentType_CLEARCOAT = 20;
const int MaterialComponentType_MAX_COMPONENT = 21;

const int MaterialComponentValueType_NONE = 0;
const int MaterialComponentValueType_FLOAT3D = 1;
const int MaterialComponentValueType_FLOAT4D = 2;
const int MaterialComponentValueType_FLOAT1D = 4;
const int MaterialComponentValueType_TEXTURE = 8;

const int MaterialComponentValueChannels_R = 1;
const int MaterialComponentValueChannels_G = 2;
const int MaterialComponentValueChannels_B = 4;
const int MaterialComponentValueChannels_A = 8;
const int MaterialComponentValueChannels_RG = MaterialComponentValueChannels_R | MaterialComponentValueChannels_G;
const int MaterialComponentValueChannels_GB = MaterialComponentValueChannels_G | MaterialComponentValueChannels_B;
const int MaterialComponentValueChannels_BA = MaterialComponentValueChannels_B | MaterialComponentValueChannels_A;
const int MaterialComponentValueChannels_RGB = MaterialComponentValueChannels_R | MaterialComponentValueChannels_G | MaterialComponentValueChannels_B;
const int MaterialComponentValueChannels_RGBA = MaterialComponentValueChannels_R | MaterialComponentValueChannels_G | MaterialComponentValueChannels_B | MaterialComponentValueChannels_A;

// Structure for material component
struct MaterialComponent {
    vec4 value;      // Generic value, can be interpreted based on valueType
    int valueType;   // Type of value stored (color3D, color4D, float, texture)
    int channels;    // Which channels are used
};

struct Material {
    MaterialComponent components[MaterialComponentType_MAX_COMPONENT];
    vec2 textureRepeatFactor;
};

layout(binding = 0, set = 4) uniform materialProps {
    Material material;
};

layout(binding = 1, set = 4) uniform texture2D brdfLUT;
layout(binding = 2, set = 4) uniform texture2D irradianceMap;

// Function to get a texture value from a material component
vec4 GetMaterialTexture(int componentType, vec2 uv) {
    // All the textures are in linear color space whether HDR is enabled or not, why ???
    return fromLinear(GetTexture(componentType, uv * material.textureRepeatFactor));
    return graphicsSettings.hdrEnabled == 1 ? fromLinear(GetTexture(componentType, uv * material.textureRepeatFactor)) : fromLinear(GetTexture(componentType, uv * material.textureRepeatFactor));
}

float MaterialComponentGetValue1(int componentType, vec2 uv) {
    float ret;
    if ((material.components[componentType].valueType & MaterialComponentValueType_TEXTURE) != 0) {
        vec4 value = GetMaterialTexture(componentType, uv);
        if ((material.components[componentType].channels & MaterialComponentValueChannels_G) != 0)
            ret = value.g;
        else if ((material.components[componentType].channels & MaterialComponentValueChannels_B) != 0)
            ret = value.b;
        else if ((material.components[componentType].channels & MaterialComponentValueChannels_A) != 0)
            ret = value.a;
        else
            ret = value.r;
        if ((material.components[componentType].valueType & MaterialComponentValueType_FLOAT4D) != 0) {
            ret *= material.components[componentType].value.r;
        } else if ((material.components[componentType].valueType & MaterialComponentValueType_FLOAT1D) != 0) {
            ret *= material.components[componentType].value.r;
        }
    } else
        ret = material.components[componentType].value.r;
    return ret;
}

vec2 MaterialComponentGetValue2(int componentType, vec2 uv) {
    vec2 ret;
    if ((material.components[componentType].valueType & MaterialComponentValueType_TEXTURE) != 0) {
        vec4 value = GetMaterialTexture(componentType, uv);
        if ((material.components[componentType].channels & MaterialComponentValueChannels_G) != 0 &&
            (material.components[componentType].channels & MaterialComponentValueChannels_B) != 0)
            ret = value.gb;
        else if ((material.components[componentType].channels & MaterialComponentValueChannels_B) != 0 &&
                (material.components[componentType].channels & MaterialComponentValueChannels_A) != 0)
            ret = value.ba;
        else if ((material.components[componentType].channels & MaterialComponentValueChannels_G) != 0 &&
                (material.components[componentType].channels & MaterialComponentValueChannels_A) != 0)
            ret = value.ga;
        else if ((material.components[componentType].channels & MaterialComponentValueChannels_R) != 0 &&
                (material.components[componentType].channels & MaterialComponentValueChannels_G) != 0)
            ret = value.rg;
        else if ((material.components[componentType].channels & MaterialComponentValueChannels_R) != 0 &&
                (material.components[componentType].channels & MaterialComponentValueChannels_B) != 0)
            ret = value.rb;
        else if ((material.components[componentType].channels & MaterialComponentValueChannels_R) != 0 &&
                (material.components[componentType].channels & MaterialComponentValueChannels_A) != 0)
            ret = value.ra;
        else
            ret = value.rg; // Default
        if ((material.components[componentType].valueType & MaterialComponentValueType_FLOAT4D) != 0) {
            ret *= material.components[componentType].value.rg;
        } else if ((material.components[componentType].valueType & MaterialComponentValueType_FLOAT1D) != 0) {
            ret *= material.components[componentType].value.r;
        }
    } else
        ret = material.components[componentType].value.rg;
    return ret;
}

vec3 MaterialComponentGetValue3(int componentType, vec2 uv) {
    vec3 ret;
    if ((material.components[componentType].valueType & MaterialComponentValueType_TEXTURE) != 0) {
       vec4 value = GetMaterialTexture(componentType, uv);
       if ((material.components[componentType].channels | MaterialComponentValueChannels_R) != 0 &&
              (material.components[componentType].channels | MaterialComponentValueChannels_G) != 0 &&
                (material.components[componentType].channels | MaterialComponentValueChannels_B) != 0)
           ret = value.rgb;
       else
           ret = value.gba; // Default fallback
        if ((material.components[componentType].valueType & MaterialComponentValueType_FLOAT4D) != 0) {
            ret *= material.components[componentType].value.rgb;
        } else if ((material.components[componentType].valueType & MaterialComponentValueType_FLOAT1D) != 0) {
            ret *= material.components[componentType].value.r;
        }
    } else
        ret = material.components[componentType].value.rgb;
    return ret;
}

vec4 MaterialComponentGetValue4(int componentType, vec2 uv) {
    if ((material.components[componentType].valueType & MaterialComponentValueType_TEXTURE) != 0) {
        vec4 ret = GetMaterialTexture(componentType, uv);
        if ((material.components[componentType].valueType & MaterialComponentValueType_FLOAT4D) != 0) {
            ret *= material.components[componentType].value;
        } else if ((material.components[componentType].valueType & MaterialComponentValueType_FLOAT1D) != 0) {
            ret *= material.components[componentType].value.r;
        }
        return ret;
    }
    return material.components[componentType].value;
}