#include "PBR.hlsl.h"

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

// Disney Principled BRDF Helper Functions
// float SchlickFresnel(float u) {
//     return pow(1.0 - u, 5.0);
// }
//
// float GTR2(float NdotH, float a) {
//     float a2 = a * a;
//     float t = 1.0 + (a2 - 1.0) * NdotH * NdotH;
//     return a2 / (M_PI * t * t);
// }
//
// float GTR2Aniso(float NdotH, float3 H, float3 N, float ax, float ay) {
//     float3 X = normalize(cross(N, float3(0, 1, 0)));
//     float3 Y = normalize(cross(N, X));
//
//     float hdx = dot(H, X) / ax;
//     float hdy = dot(H, Y) / ay;
//     float ndh = dot(N, H);
//
//     return 1.0 / (M_PI * ax * ay *
//         sqrt(1.0 - ndh * ndh +
//         (hdx * hdx / (ax * ax)) +
//         (hdy * hdy / (ay * ay))));
// }
//
// float SmithGGXCorrelated(float NdotV, float NdotL, float a) {
//     float a2 = a * a;
//     float GGXL = NdotV * sqrt((-NdotL * a2 + NdotL) * NdotL + a2);
//     float GGXV = NdotL * sqrt((-NdotV * a2 + NdotV) * NdotV + a2);
//     return 0.5 / (GGXL + GGXV);
// }
//
// float GeometrySmithAniso(float NdotL, float NdotV, float NdotH, float LdotH, float roughness) {
//     float a = roughness * roughness;
//     float lambdaV = NdotL * sqrt((-NdotV * a + NdotV) * NdotV + a);
//     float lambdaL = NdotV * sqrt((-NdotL * a + NdotL) * NdotL + a);
//     return 2.0 * NdotL * NdotV / (lambdaV + lambdaL);
// }
//
// float3 DisneyPrincipledBRDF(
//     float3 N,           // Surface normal
//     float3 V,           // View direction
//     float3 L,           // Light direction
//     float3 baseColor,   // Base color
//     float metallic,     // Metallic factor
//     float roughness,    // Roughness factor
//     float subsurface,   // Subsurface scattering factor
//     float specular,     // Specular intensity
//     float specularTint, // Specular tint
//     float anisotropic,  // Anisotropic factor
//     float sheen,        // Sheen intensity
//     float sheenTint     // Sheen tint
// ) {
//     float NdotL = saturate(dot(N, L));
//     float NdotV = saturate(dot(N, V));
//     float3 H = normalize(V + L);
//     float NdotH = saturate(dot(N, H));
//     float LdotH = saturate(dot(L, H));
//
//     // Diffuse
//     float FL = SchlickFresnel(NdotL);
//     float FV = SchlickFresnel(NdotV);
//     float Fd90 = 0.5 + 2.0 * LdotH * LdotH * roughness;
//     float Fd = lerp(1.0, Fd90, FL) * lerp(1.0, Fd90, FV);
//
//     // Specular
//     float aspect = sqrt(1.0 - anisotropic * 0.9);
//     float ax = max(0.001, roughness * roughness / aspect);
//     float ay = max(0.001, roughness * roughness * aspect);
//
//     float Ds = GTR2Aniso(NdotH, H, N, ax, ay);
//     float FH = SchlickFresnel(LdotH);
//     float3 Fs = lerp(float3(specular, specular, specular), baseColor, specularTint) * FH;
//     float Gs = GeometrySmithAniso(NdotL, NdotV, NdotH, LdotH, roughness);
//
//     float3 specular_term = Ds * Fs * Gs / (4.0 * NdotL * NdotV);
//
//     // Sheen
//     float3 sheen_color = lerp(float3(1.0, 1.0, 1.0), baseColor, sheenTint);
//     float3 sheen_term = sheen * sheen_color * SchlickFresnel(LdotH);
//
//     // Subsurface scattering approximation
//     float Fss90 = LdotH * LdotH * roughness;
//     float Fss = lerp(1.0, Fss90, FL) * lerp(1.0, Fss90, FV);
//     float ss = 1.25 * (Fss * (1.0 / (NdotL + NdotV) - 0.5) + 0.5);
//
//     // To calculate
//     float3 finalSheen = float3(0.0, 0.0, 0.0);
//     float3 diffuse = ((lerp(Fd, ss, subsurface) * baseColor + FH * finalSheen) / M_PI) * (1.0 - metallic);// * (1.0 - specTrans);
//
//     // Combine components
//     // return NdotL * (
//     //     diffuse * (1.0 - subsurface) +
//     //     ss * subsurface * baseColor +
//     //     specular_term +
//     //     sheen_term
//     // );
//     return NdotL * (diffuse);
// }


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

    float4 baseColor = g_baseColor.SubpassLoad(1);
    float3 normal = g_normal.SubpassLoad(1).xyz;
    float4 specular = g_specular.SubpassLoad(1);
    float metallic = g_metallicRoughAo.SubpassLoad(1).r;
    float roughness = g_metallicRoughAo.SubpassLoad(1).g;
    float ao = g_metallicRoughAo.SubpassLoad(1).b;
    float4 position = g_position.SubpassLoad(1);

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

    // Calculate final shading
    float3 viewDir = normalize(cameraPos - position.xyz);

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    // Define additional material parameters for the Disney BRDF
    float subsurface = 0.0f;  // Subsurface scattering amount
    float specularVal = 0.8; // Specular intensity
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

        float3 X = normalize(cross(normal, float3(0.0, 1.0, 0.0)));
        float3 Y = cross(normal, X);
        finalColor += DisneyPrincipledBRDF(lightDir, viewDir, normal, X, Y, baseColor.rgb, metallic, roughness, subsurface, specularVal, specularTint, anisotropic, sheen, sheenTint, clearCoat, clearCoatGloss) * radiance;
        // finalColor += DisneyPrincipledBRDF(
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