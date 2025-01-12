#version 450

#extension GL_GOOGLE_include_directive : require

#include "../PBR.glsl.h"

// Placeholder for shadow calculation
float ComputeShadow(vec3 position, vec3 normal)
{
    return 0.0;
}

vec3 GetLightDirection(Light light, vec3 position)
{
    // If no rotation at all, then light direction is towards bottom (0.0, -1.0, 0.0)
    if (light.type == LightType_Directional) {
        return SpotAndDirectionalDirection(light.direction);
    } else if (light.type == LightType_Point) {
        return normalize(light.position - position);
    } else if (light.type == LightType_Spot) {
        return SpotAndDirectionalDirection(light.direction);
    }
    return vec3(0.0, 0.0, 0.0);
}

vec3 GetLightColor(Light light, vec3 position, vec3 direction)
{
    if (light.type == LightType_Directional) {
        return light.color * light.intensity;
    } else if (light.type == LightType_Point) {
        float distance = max(0.01, length(light.position - position));
        float attenuation = min(100.0, 1.0 / (distance * distance));
        attenuation *= light.intensity;
        if (attenuation <= PointLight_Threshold)
            return vec3(0.0, 0.0, 0.0);
        return light.color * attenuation;
    } else if (light.type == LightType_Spot) {
        float distance = length(light.position - position);
        if (distance <= 0.001)
            return vec3(0.0, 0.0, 0.0);
        float attenuation = 1.0 / (distance * distance + 0.01);
        float spotFactor = dot(normalize(direction), normalize(light.position - position));
        spotFactor = max(spotFactor, 0.0);
        float spotAngle = cos(light.angle * 0.5 / 180.0 * M_PI);
        if (spotFactor < spotAngle)
            return vec3(0.0, 0.0, 0.0);
        // Make spot factor lerp between 0 and spotAngle
        spotFactor = smoothstep(spotAngle, 1.0, spotFactor);
        return light.color * light.intensity * attenuation * spotFactor;
    }
    return vec3(0.0, 0.0, 0.0);
}

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 outNormal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 fragColor;
layout(location = 4) in vec3 inputTangent;
layout(location = 5) in vec3 inputBitangent;

layout(location = 6) in vec2 screenPos;

layout(origin_upper_left) in vec4 gl_FragCoord;

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
        // Calculate shadow factor
        float shadow = ComputeShadow(position, normal);

        if (shadow < 1.0 - FLT_EPSILON)
        {
            for (int i = 0; i < lightCount; ++i) {
                Light light = lights[i];

                if (isLightInBlock(gl_FragCoord.xy, i) == false)
                    continue;

                // Compute BRDF for this light
                vec3 lightDir = GetLightDirection(light, position);
                vec3 lightColor = GetLightColor(light, position, lightDir);
                if (length(lightColor) < 0.01)
                    continue;

                vec3 radiance = lightColor * clamp(dot(normal, lightDir), 0.0, 1.0);

                if (tangentSpace) {
                    finalColor.rgb += DisneyPrincipledBSDF(lightDir, viewDir, normal, T, B, baseColor.rgb, metallic, roughness, subsurface, specularVal, specularTint, anisotropic, sheen, sheenTint, clearCoat, clearCoatGloss) * radiance;
                } else {
                    finalColor.rgb += LambertCookTorrance(lightDir, viewDir, normal, baseColor.rgb, metallic, roughness) * radiance;
                }
            }
            finalColor.rgb *= 1.0 - shadow;
        }

        finalColor.rgb += emissive.rgb * emissive.a;
        finalColor.rgb += Reflection(viewDir, normal, baseColor.rgb, metallic, roughness);

        // Set transparency
        finalColor.a = baseColor.a;
        finalColor.a = 1.0;


        // Ambient Occlusion
        float occlusionStrength = 1.0;
        finalColor = mix(finalColor, finalColor * ao, occlusionStrength);

        finalColor.a = opacity;
    } else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_Depth) {
        finalColor = vec4(vec3(gl_FragCoord.z), 1.0);
    } else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_Normals) {
        finalColor = vec4(normal * 0.5 + 0.5, 1.0);
    } else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_ForwardPlus) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        for (int i = 0; i < lightCount; ++i) {
            if (isLightInBlock(gl_FragCoord.xy, i) == false)
                continue;
            finalColor.rgb += vec3(1.0, 1.0, 1.0) / float(lightCount);
        }
    }
}