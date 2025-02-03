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

vec3 Reflection(vec3 V, vec3 N, vec3 baseColor, float metallic, float roughness)
{
    // Simplified Fresnel-weighted reflection
    float NDotV = dot(N, V);

    ivec2 imageSizeBrdfLUT = imageSize(brdfLUT);
    ivec2 coords = ivec2(NDotV * imageSizeBrdfLUT.x, roughness * imageSizeBrdfLUT.y);
    vec2 brdf = imageLoad(brdfLUT, coords).rg;

    vec3 R = normalize(reflect(-V, N));
    vec3 specularReflectionColor = GetPanoramaRadiance(R, roughness).rgb;
    vec3 specularReflectionBaseColor = mix(vec3(0.04), baseColor, metallic);
    vec3 specularReflection = specularReflectionColor * (specularReflectionBaseColor * brdf.x + brdf.y);

    vec3 diffuseLight = baseColor * (1.0 - metallic) * (1.0 - 0.04);
//    vec2 irradianceUvF = mod(PanoramaUvFromDir(N), vec2(1.0));
//    ivec2 irradianceMapSize = imageSize(irradianceMap);
//    ivec2 irradianceUV = ivec2(irradianceUvF.x * irradianceMapSize.x, irradianceUvF.y * irradianceMapSize.y);
//    vec3 irradiance = imageLoad(irradianceMap, irradianceUV).rgb;
    vec3 irradiance = GetPanoramaIrradiance(N).rgb;
    vec3 diffuseReflection = diffuseLight * irradiance;

    // TODO: Make specular reflection more blurry with roughness

    return
        specularReflection
        + diffuseReflection
        ;
}

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

    baseColor = MaterialComponentGetValue4(MaterialComponentType_BASE_COLOR, uv);
    baseColor = toLinear(baseColor);

    // Reverse normal if back face
    vec3 realNormal = outNormal;
    if (gl_FrontFacing == false)
         realNormal = -realNormal;

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

    // Specular
    if (material.components[MaterialComponentType_SPECULAR].valueType != MaterialComponentValueType_NONE)
        specular = toLinear(MaterialComponentGetValue4(MaterialComponentType_SPECULAR, uv));
    else
        specular = vec4(0.5, 0.5, 0.5, 1.0);

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

   // Opacity
   if (material.components[MaterialComponentType_OPACITY].valueType != MaterialComponentValueType_NONE)
        opacity = MaterialComponentGetValue1(MaterialComponentType_OPACITY, uv);

    // Define additional material parameters for the Disney BRDF
    float subsurface = 0.0;  // Subsurface scattering amount
    float specularVal = 0.4; // Specular intensity
    float specularTint = 0.5; // Specular tint
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
        finalColor.rgb += Reflection(viewDir, normal, baseColor.rgb, metallic, roughness);

        // Set transparency
        finalColor.a = opacity * baseColor.a;
        finalColor.a *= baseColor.a;

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
}