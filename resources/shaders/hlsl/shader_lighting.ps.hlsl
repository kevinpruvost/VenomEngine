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
[[vk::input_attachment_index(4)]] SubpassInput g_specular : register(t7, space7);

struct FragmentOutput {
    float4 color : SV_Target0;
};

// Placeholder for shadow calculation
float ComputeShadow(float3 position, float3 normal, float3 lightDir) {
    float3 realNormal = normalize(position + normal);
    float ndl = dot(normal, -lightDir);
    return saturate(ndl); // Shadow factor based on light direction and surface normal
}

float DistributionGGX(float3 N, float3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = saturate(dot(N, H));
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (M_PI * denom * denom);
}

float3 FresnelSchlick(float cosTheta, float3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness) {
    float NdotV = saturate(dot(N, V));
    float NdotL = saturate(dot(N, L));
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

float3 CookTorranceBRDF(
    float3 N,           // Surface normal
    float3 V,           // View direction
    float3 L,           // Light direction
    float3 F0,          // Fresnel reflectance at normal incidence
    float roughness,    // Roughness factor
    float metallic,     // Metallic factor
    float3 albedo       // Base color
) {
    float3 H = normalize(V + L); // Halfway vector

    // Dot products
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));

    // Cook-Torrance components
    float D = DistributionGGX(N, H, roughness);
    float3 F = FresnelSchlick(VdotH, F0);
    float G = GeometrySmith(N, V, L, roughness);

    // Specular BRDF
    float3 specular = (D * F * G) / max(4.0 * NdotV * NdotL, 0.001);

    // Diffuse BRDF (Lambertian)
    float3 kD = float3(1.0, 1.0, 1.0) - F; // Energy conservation
    kD *= 1.0 - metallic;                 // Non-metallic surfaces only
    float3 diffuse = kD * albedo / M_PI;

    return (diffuse + specular) * NdotL;  // Apply Lambert's cosine law
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

    // Test normal

    // Calculate final shading
    float3 viewDir = normalize(cameraPos - position.xyz);

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    // Loop over lights
//    for (uint i = 0; i < lightCount; ++i) {
    for (uint i = 0; i < 1; ++i) {
        Light light = lights[i];

        // Compute BRDF for this light
        float3 lightColor = GetLightColor(light, position.xyz);
        float3 lightDir = GetLightDirection(light, position.xyz);
        //finalColor = baseColor.rgb * dot(normal, lightDir);
        // Fresnel reflectance at normal incidence
        float3 F0 = lerp(float3(0.04, 0.04, 0.04), specular.rgb, metallic);
        float3 radiance = lightColor * saturate(dot(normal, lightDir));
        finalColor += CookTorranceBRDF(normal, viewDir, lightDir, F0, roughness, metallic, baseColor.rgb) * radiance;
    }

    // Add ambient light
    finalColor += ComputeAmbient(baseColor.rgb, normal, viewDir, metallic, roughness, float3(0.2, 0.2, 0.2), 0.01f);

    // Calculate shadow factor
    // float shadowFactor = ComputeShadow(position.xyz, normal, lightDir) * 1.0f;

    finalColor *= ao;
    output.color = float4(finalColor, 1.0f);
    return output;
}