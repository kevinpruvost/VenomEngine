#include "Resources.ps.hlsl.h"

// Lighting shader

struct LightingVSOutput {
    float4 position : SV_POSITION;
    [[vk::location(0)]] float2 uv : TEXCOORD0;
};

[[vk::input_attachment_index(0)]] SubpassInput g_baseColor : register(t3, space7);
[[vk::input_attachment_index(1)]] SubpassInput g_normal : register(t4, space7);
[[vk::input_attachment_index(2)]] SubpassInput g_metallicRoughAo : register(t5, space7);
[[vk::input_attachment_index(3)]] SubpassInput g_position : register(t6, space7);
[[vk::input_attachment_index(3)]] SubpassInput g_specular : register(t7, space7);

struct FragmentOutput {
    float4 color : SV_Target0;
};

// Placeholder for shadow calculation
float ComputeShadow(float3 position, float3 normal, float3 lightDir) {
    float3 realNormal = normalize(position + normal);
    float ndl = dot(normal, -lightDir);
    return saturate(ndl); // Shadow factor based on light direction and surface normal
}

float3 ComputeBRDF(
    float3 baseColor,
    float3 normal,
    float3 viewDir,
    float3 lightDir,
    float3 lightColor,
    float metallic,
    float roughness,
    float specular = 1.0f
    ) {
    // Ensure our vectors are normalized
    normal = normalize(normal);
    viewDir = normalize(viewDir);
    lightDir = normalize(lightDir);

    // Compute common vectors
    float3 halfVector = normalize(viewDir + lightDir);

    // Compute common dot products
    float NdotL = max(dot(normal, lightDir), 0.0f);
    float NdotV = max(dot(normal, viewDir), 0.0f);
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float HdotV = max(dot(halfVector, viewDir), 0.0f);

    // Roughness remapping
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;

    // Fresnel (Schlick approximation)
    float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), baseColor, metallic);
    float3 F = f0 + (1.0f - f0) * pow(1.0f - HdotV, 5.0f);

    // Distribution (GGX/Trowbridge-Reitz)
    float D = alpha2;
    float denomD = (NdotH * NdotH * (alpha2 - 1.0f) + 1.0f);
    D /= max(M_PI * denomD * denomD, 0.001f);

    // Geometry (Smith model with Schlick-GGX)
    float k = (roughness + 1.0f) * (roughness + 1.0f) / 8.0f;
    float G1V = NdotV / (NdotV * (1.0f - k) + k);
    float G1L = NdotL / (NdotL * (1.0f - k) + k);
    float G = G1V * G1L;

    // Combine terms for specular
    float3 specularTerm = (F * D * G) / max(4.0f * NdotV * NdotL, 0.001f);
    specularTerm *= specular;

    // Compute diffuse term
    float3 diffuseTerm = baseColor * (1.0f - metallic) / M_PI;

    // Combine everything
    float3 result = (diffuseTerm + specularTerm) * lightColor * NdotL;

    return result;
}


float3 GetLightColor(Light light, float3 position)
{
    if (light.type == LightType::Directional) {
        return light.color * light.intensity;
    }
    else if (light.type == LightType::Point) {
        float distance = length(light.position - position);
        float attenuation = 1.0 / (distance * distance + 1.0);
        return light.color * light.intensity * attenuation;
    }
    else if (light.type == LightType::Spot) {
        // float distance = length(light.position - position);
        // float attenuation = 1.0 / (distance * distance + 1.0);
        // float3 lightDir = normalize(light.position - position);
        // float spotFactor = dot(lightDir, light.direction);
        // float spotAttenuation = smoothstep(light.innerCutoff, light.outerCutoff, spotFactor);
        // return light.color * light.intensity * attenuation * spotAttenuation;
        return float3(0.0f, 0.0f, 0.0f);
    }
    return float3(0.0f, 0.0f, 0.0f);
}

float3 GetLightDirection(Light light, float3 position)
{
    if (light.type == LightType::Directional) {
        return -light.direction;
    }
    else if (light.type == LightType::Point) {
        return normalize(light.position - position);
    }
    else if (light.type == LightType::Spot) {
        return normalize(light.position - position);
    }
    return float3(0.0f, 0.0f, 0.0f);
}

float3 ComputeAmbient(
    float3 baseColor,
    float3 normal,
    float3 viewDir,
    float metallic,
    float roughness,
    float3 ambientColor,
    float ambientIntensity = 0.03f
) {
    float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), baseColor, metallic);
    float NdotV = max(dot(normal, viewDir), 0.0f);

    // Simple fresnel for ambient
    float3 F = f0 + (1.0f - f0) * pow(1.0f - NdotV, 5.0f);

    // Ambient diffuse
    float3 ambientDiffuse = baseColor * (1.0f - metallic) * ambientColor * ambientIntensity;

    // Ambient specular
    float3 ambientSpecular = F * ambientColor * ambientIntensity;

    return ambientDiffuse + ambientSpecular;
}

FragmentOutput main(LightingVSOutput input) {
    FragmentOutput output;

    float4 baseColor = g_baseColor.SubpassLoad();
    float3 normal = g_normal.SubpassLoad().xyz;
    float4 specular = g_specular.SubpassLoad();
    float metallic = g_metallicRoughAo.SubpassLoad().r;
    float roughness = g_metallicRoughAo.SubpassLoad().g;
    float ao = g_metallicRoughAo.SubpassLoad().b;
    float4 position = g_position.SubpassLoad();

    if (position.x == 0.0 && position.y == 0.0 && position.z == 0.0) {
       discard;
    }

    // Calculate final shading
    float3 viewDir = normalize(cameraPos - position.xyz);

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    // Loop over lights
//    for (uint i = 0; i < lightCount; ++i) {
    for (uint i = 0; i < 1; ++i) {
        Light light = lights[i];

        // Compute BRDF for this light
        float3 lightColor = GetLightColor(light, position.xyz);
        float3 lightDir = normalize(GetLightDirection(light, position.xyz));
        finalColor += ComputeBRDF(baseColor.rgb, normal, viewDir, lightDir, lightColor, metallic, roughness);
    }

    // Add ambient light
    finalColor += ComputeAmbient(baseColor.rgb, normal, viewDir, metallic, roughness, float3(0.2, 0.2, 0.2));

    // Calculate shadow factor
    // float shadowFactor = ComputeShadow(position.xyz, normal, lightDir) * 1.0f;

    output.color = float4(finalColor, 1.0f);
    return output;
}