#version 450

#extension GL_GOOGLE_include_directive : require

#include "../Resources.frag.glsl.h"

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 outNormal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 fragColor;
layout(location = 4) in vec3 inputTangent;
layout(location = 5) in vec3 inputBitangent;

layout(location = 6) in vec2 screenPos;

layout(origin_upper_left) in vec4 gl_FragCoord;

vec3 Reflection(vec3 V, vec3 N, vec3 baseColor, float metallic, float roughness, float transmission, float ior, vec3 specularTint)
{
    return vec3(0.0);
}

//layout(location = 1) out vec4 lightingResult;

void main()
{
    vec4 baseColor;
    vec3 normal;
    vec4 specular;
    float metallic = 0.0;
    float roughness = 1.0;
    float ao = 1.0;
    vec4 emissive = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 position = worldPos;
    float opacity = 1.0;
    float transmission = 0.0;
    float ior = 1.5;
    vec3 specularTint = vec3(0.0);

    baseColor = toLinear(MaterialComponentGetValue4(MaterialComponentType_BASE_COLOR, uv));

    // Reverse normal if back face
    vec3 realNormal = outNormal;
    if (gl_FrontFacing == false) {
         discard;
         realNormal = -realNormal;
     }

    // mat3 TBN = mat3(inputTangent, inputBitangent, realNormal);
    bool tangentSpace = material.components[MaterialComponentType_NORMAL].valueType == MaterialComponentValueType_TEXTURE;
    normal = normalize(realNormal);
    vec3 T = inputTangent;
    vec3 B = inputBitangent;
    mat3 TBN = mat3(T, B, normal);
    if (tangentSpace) {
        vec3 N = GetMaterialTexture(MaterialComponentType_NORMAL, uv).rgb * 2.0 - 1.0;
        normal = normalize(TBN * N);
    }

    // Metallic (if PBR, then METALLIC otherwise 0)
    if (material.components[MaterialComponentType_METALLIC].valueType != MaterialComponentValueType_NONE)
        metallic = MaterialComponentGetValue1(MaterialComponentType_METALLIC, uv);
    // Roughness (if PBR, then ROUGHNESS otherwise square of 2/(SPECULAR+2))
    if (material.components[MaterialComponentType_ROUGHNESS].valueType != MaterialComponentValueType_NONE)
        roughness = MaterialComponentGetValue1(MaterialComponentType_ROUGHNESS, uv);
    // Ambient occlusion
    if (material.components[MaterialComponentType_AMBIENT_OCCLUSION].valueType != MaterialComponentValueType_NONE)
        ao = MaterialComponentGetValue1(MaterialComponentType_AMBIENT_OCCLUSION, uv);
    // Emissive
    if (material.components[MaterialComponentType_EMISSIVE].valueType != MaterialComponentValueType_NONE)
        emissive = toLinear(MaterialComponentGetValue4(MaterialComponentType_EMISSIVE, uv));

    // Transmission
    if (material.components[MaterialComponentType_TRANSMISSION].valueType != MaterialComponentValueType_NONE)
        transmission = MaterialComponentGetValue1(MaterialComponentType_TRANSMISSION, uv);
    // IOR
    if (material.components[MaterialComponentType_REFRACTION].valueType != MaterialComponentValueType_NONE)
        ior = MaterialComponentGetValue1(MaterialComponentType_REFRACTION, uv);

   // Opacity
   if (material.components[MaterialComponentType_OPACITY].valueType != MaterialComponentValueType_NONE)
        opacity = MaterialComponentGetValue1(MaterialComponentType_OPACITY, uv);

    // Specular Tint
    if (material.components[MaterialComponentType_SPECULAR].valueType != MaterialComponentValueType_NONE)
        specularTint = MaterialComponentGetValue3(MaterialComponentType_SPECULAR, uv);


    // Define additional material parameters for the Disney BRDF
    float subsurface = 0.0;  // Subsurface scattering amount
    float specularVal = 0.4; // Specular intensity
    float anisotropic = 0.0; // Anisotropy
    float sheen = 0.0;       // Sheen amount
    float sheenTint = 0.0;   // Sheen tint
    float clearCoat = 0.0;   // Clear coat amount
    float clearCoatGloss = 0.0; // Clear coat glossiness

    vec3 viewDir = normalize(cameraPos - position);

    // Loop over lights
    finalColor = vec4(0.0, 0.0, 0.0, 0.0);

    if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_None)
    {
        finalColor.rgb += emissive.rgb * emissive.a;
        finalColor.rgb += Reflection(viewDir, normal, baseColor.rgb, metallic, roughness, transmission, ior, specularTint);

        // Set transparency
        finalColor.a = opacity * baseColor.a;

        // Ambient Occlusion
        float occlusionStrength = 1.0;
        finalColor = mix(finalColor, finalColor * ao, occlusionStrength);
    }
    else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_Depth) {
        finalColor = vec4(vec3(gl_FragCoord.z), 1.0);
    } else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_Normals) {
        finalColor = vec4(normal * 0.5 + 0.5, 1.0);
    } else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_ForwardPlus) {
        return;
    } else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_ShadowMapping) {
        return;
    }
    //lightingResult = finalColor;

    //if (lightingResult.a < 0.2) {
        // No depth write if alpha is low (transparent)
      //  discard;
    //}
}