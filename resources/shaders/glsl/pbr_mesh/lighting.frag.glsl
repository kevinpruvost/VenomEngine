#version 450

#extension GL_GOOGLE_include_directive : require

#include "../PBR.glsl.h"

// Placeholder for shadow calculation
float ComputeShadow(vec3 position, vec3 normal, vec3 lightDir) {
    vec3 realNormal = normalize(position + normal);
    float ndl = dot(normal, -lightDir);
    return clamp(ndl, 0.0, 1.0); // Shadow factor based on light direction and surface normal
}

vec3 GetLightColor(Light light, vec3 position) {
    if (light.type == LightType_Directional) {
        return light.color * light.intensity;
    } else if (light.type == LightType_Point) {
        float distance = length(light.position - position);
        float attenuation = 1.0 / (distance * distance + 1.0);
        return light.color * light.intensity * attenuation;
    } else if (light.type == LightType_Spot) {
        // float distance = length(light.position - position);
        // float attenuation = 1.0 / (distance * distance + 1.0);
        // vec3 lightDir = normalize(light.position - position);
        // float spotFactor = dot(lightDir, light.direction);
        // float spotAttenuation = smoothstep(light.innerCutoff, light.outerCutoff, spotFactor);
        // return light.color * light.intensity * attenuation * spotAttenuation;
        return vec3(0.0, 0.0, 0.0);
    }
    return vec3(0.0, 0.0, 0.0);
}

vec3 GetLightDirection(Light light, vec3 position) {
    if (light.type == LightType_Directional) {
        return -normalize(light.direction);
    } else if (light.type == LightType_Point) {
        return normalize(light.position - position);
    } else if (light.type == LightType_Spot) {
        return normalize(light.position - position);
    }
    return vec3(0.0, 0.0, 0.0);
}

vec3 ComputeAmbient(
    vec3 baseColor,
    vec3 normal,
    vec3 viewDir,
    float metallic,
    float roughness,
    vec3 ambientColor,
    float ambientIntensity
) {
    vec3 f0 = mix(vec3(0.04, 0.04, 0.04), baseColor, metallic);
    float NdotV = max(dot(normal, viewDir), 0.0);

    // Simple fresnel for ambient
    vec3 F = f0 + (1.0 - f0) * pow(1.0 - NdotV, 5.0);

    // Ambient diffuse
    vec3 ambientDiffuse = baseColor * (1.0 - metallic) * ambientColor * ambientIntensity;

    // Ambient specular
    vec3 ambientSpecular = F * ambientColor * ambientIntensity;

    return ambientDiffuse + ambientSpecular;
}

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 outNormal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 fragColor;
layout(location = 4) in vec3 inputTangent;
layout(location = 5) in vec3 inputBitangent;

layout(location = 6) in vec2 screenPos;

void main()
{
    vec4 baseColor;
    vec3 normal;
    vec4 specular;
    float metallic = 0.0;
    float roughness = 1.0;
    float ao = 1.0;
    vec4 emissive;
    vec3 position = worldPos;
    float opacity = 1.0;

    vec2 invertedUv = vec2(uv.x, 1.0 - uv.y);

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
    } else {

    }

    // Specular
    if (material.components[MaterialComponentType_SPECULAR].valueType != MaterialComponentValueType_NONE)
        specular = MaterialComponentGetValue4(MaterialComponentType_SPECULAR, uv);
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
        emissive = MaterialComponentGetValue4(MaterialComponentType_EMISSIVE, uv);
    else
        emissive = vec4(0.0, 0.0, 0.0, 0.0);

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
    for (int i = 0; i < lightCount; ++i) {
        Light light = lights[i];

        if (isLightInBlock(screenPos, i) == false)
            continue;

        // Compute BRDF for this light
        vec3 lightColor = GetLightColor(light, position);
        vec3 lightDir = GetLightDirection(light, position);

        vec3 radiance = lightColor * clamp(dot(normal, lightDir), 0.0, 1.0);

        if (tangentSpace) {
            finalColor.rgb += DisneyPrincipledBSDF(lightDir, viewDir, normal, T, B, baseColor.rgb, metallic, roughness, subsurface, specularVal, specularTint, anisotropic, sheen, sheenTint, clearCoat, clearCoatGloss) * radiance;
        }
        // finalColor.rgb = normal;
        // position.y = -position.y;
        // vec3 q1 = dFdx(position);
        // vec3 q2 = dFdy(position);
        // vec2 st1 = dFdx(uv);
        // vec2 st2 = dFdy(uv);
        // vec3 T = normalize(q1 * st2.t - q2 * st1.t);
        // vec3 N = normalize(realNormal);
        // vec3 B = -normalize(cross(N, T));
        // mat3 TBN_ = mat3(T, B, N);
        // vec3 normalTest = graphicsSettings.hdrEnabled == 1 ? fromLinear(GetMaterialTexture(MaterialComponentType_NORMAL, uv)).rgb : GetMaterialTexture(MaterialComponentType_NORMAL, uv).rgb;
        // normalTest = normalTest * 2.0 - 1.0;
        // normalTest = normalize(TBN_ * normalTest);

        // finalColor.rgb = normalTest;
        //finalColor.rgb = vec3(dot(normalTest, viewDir));
        //finalColor.rgb = vec3(TBN[0][0], TBN[1][1], TBN[2][2]);
        finalColor.rgb += emissive.rgb * emissive.a;
    }
    finalColor.rgb += Reflection(viewDir, normal, baseColor.rgb, metallic, roughness);

    // Set transparency
    finalColor.a = baseColor.a;
    finalColor.a = 1.0;

    // Add ambient light
    // finalColor.rgb += ComputeAmbient(baseColor.rgb, normal, viewDir, metallic, roughness, vec3(0.1, 0.1, 0.1), 0.03);

    // Calculate shadow factor
    // float shadow = ComputeShadow(position, normal, lightDir);

    // finalColor.rgb *= shadow;
    // finalColor.rgb *= ao;
    // finalColor = toLinear(finalColor);
    // finalColor = vec4(pow(finalColor.rgb, vec3(1.0 / 2.2)), finalColor.a);
    // finalColor.rgb = pow(finalColor.rgb, vec3(1.0 / 1.5));

    // Ambient Occlusion
    float occlusionStrength = 1.0;
    finalColor = mix(finalColor, finalColor * ao, occlusionStrength);

    finalColor.a = opacity;
}