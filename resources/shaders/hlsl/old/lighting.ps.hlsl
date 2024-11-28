#include "../PBR.hlsl.h"

// Lighting shader

struct LightingVSOutput {
    float4 position : SV_POSITION;
    [[vk::location(0)]] float2 uv : TEXCOORD0;
};

[[vk::input_attachment_index(0)]] SubpassInputMS g_baseColor : register(t3, space7);
[[vk::input_attachment_index(1)]] SubpassInputMS g_normal : register(t4, space7);
[[vk::input_attachment_index(2)]] SubpassInputMS g_metallicRoughAo : register(t5, space7);
[[vk::input_attachment_index(3)]] SubpassInputMS g_position : register(t6, space7);
[[vk::input_attachment_index(4)]] SubpassInputMS g_specular : register(t7, space7);
struct FragmentOutput {
    float4 color : SV_Target0;
};

// Placeholder for shadow calculation
float ComputeShadow(float3 position, float3 normal, float3 lightDir) {
    float3 realNormal = normalize(position + normal);
    float ndl = dot(normal, -lightDir);
    return saturate(ndl); // Shadow factor based on light direction and surface normal
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
        return -normalize(light.direction);
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

    // MaterialPBR mat;
    // mat.baseColor = g_baseColor.SubpassLoad().rgb;
    // mat.normal = g_normal.SubpassLoad().xyz;
    // mat.metallic = g_metallicRoughAo.SubpassLoad().r;
    // mat.roughness = g_metallicRoughAo.SubpassLoad().g;
    // mat.ao = g_metallicRoughAo.SubpassLoad().b;
    // mat.specular = g_specular.SubpassLoad().rgb;

    float4 baseColor = g_baseColor.SubpassLoad(2);
    float3 normal = g_normal.SubpassLoad(2).xyz;
    float4 specular = g_specular.SubpassLoad(2);
    float metallic = g_metallicRoughAo.SubpassLoad(2).r;
    float roughness = g_metallicRoughAo.SubpassLoad(2).g;
    float ao = g_metallicRoughAo.SubpassLoad(2).b;
    float4 position = g_position.SubpassLoad(2);

    if (disableMetallic != 0) {
        metallic = constant_metallic;
    }
    if (disableRoughness != 0) {
        roughness = constant_roughness;
    }

    if (position.x == 0.0 && position.y == 0.0 && position.z == 0.0) {
       discard;
    }

    // Test normal

    // Calculate final shadingxw
    float3 viewDir = normalize(cameraPos - position.xyz);

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    // Define additional material parameters for the Disney BRDF
    float subsurface = 0.0f;  // Subsurface scattering amount
    float specularVal = 0.4; // Specular intensity
    float specularTint = 0.5; // Specular tint
    float anisotropic = 0.0; // Anisotropy
    float sheen = 0.0;       // Sheen amount
    float sheenTint = 0.0;   // Sheen tint
    float clearCoat = 0.0;   // Clear coat amount
    float clearCoatGloss = 0.0; // Clear coat glossiness

    // Loop over lights
    for (uint i = 0; i < 1; ++i) {
        Light light = lights[i];

        // Compute BRDF for this light
        float3 lightColor = GetLightColor(light, position.xyz);
        float3 lightDir = GetLightDirection(light, position.xyz);

        float3 radiance = lightColor * saturate(dot(normal, lightDir));

        float3 X;
        if (normal.x > 0.0) {
            X = normalize(cross(normal, float3(0.0, 1.0, 0.0)));
        } else {
            X = normalize(cross(normal, float3(1.0, 0.0, 0.0)));
        }
        float3 Y = cross(normal, X);
        float3x3 TBN = float3x3(X, Y, normal);
        float3 V = mul(TBN, viewDir);
        finalColor += DisneyPrincipledBSDF(lightDir, V, normal, X, Y, baseColor.rgb, metallic, roughness, subsurface, specularVal, specularTint, anisotropic, sheen, sheenTint, clearCoat, clearCoatGloss) * radiance;
        // finalColor += DisneyPrincipledBSDF(
        //     normal,
        //     viewDir,
        //     lightDir,
        //     baseColor.rgb,
        //     metallic,
        //     roughness,
        //     subsurface,
        //     specularVal,
        //     specularTint,
        //     anisotropic,
        //     sheen,
        //     sheenTint
        // ) * radiance;
    }

    // Add ambient light
    //finalColor += ComputeAmbient(baseColor.rgb, normal, viewDir, metallic, roughness, float3(0.2, 0.2, 0.2), 0.01f);

    // Calculate shadow factor
    // float shadowFactor = ComputeShadow(position.xyz, normal, lightDir) * 1.0f;

    //finalColor *= ao;

    if (normalMapDraw != 0)
        finalColor = normal;
    output.color = float4(finalColor, 1.0f);
    return output;
}