#include "Resources.ps.hlsl.h"

// Lighting shader

struct VSOutput {
    float4 position : SV_POSITION;
    [[vk::location(0)]] float2 uv : TEXCOORD0;
};

[[vk::input_attachment_index(0)]] SubpassInput g_baseColor : register(t3, space7);
[[vk::input_attachment_index(1)]] SubpassInput g_normalSpecular : register(t4, space7);
[[vk::input_attachment_index(2)]] SubpassInput g_metallicRoughAo : register(t5, space7);
[[vk::input_attachment_index(3)]] SubpassInput g_position : register(t6, space7);

struct FragmentOutput {
    float4 color : SV_Target0;
};

// Placeholder for shadow calculation
float ComputeShadow(float3 position, float3 normal, float3 lightDir) {
    // This is a stub for shadow computation.
    // Typically, you'd perform a shadow map lookup or a ray-traced shadow test.
    // For this example, let's simulate a simple shadowing effect:
    float ndl = dot(normal, -lightDir);
    return ndl; // Shadow factor based on light direction and surface normal
}

FragmentOutput main(VSOutput input) {
    FragmentOutput output;

    float4 baseColor = g_baseColor.SubpassLoad();
    float3 normal = g_normalSpecular.SubpassLoad().xyz;
    float specular = g_normalSpecular.SubpassLoad().w;
    float metallic = g_metallicRoughAo.SubpassLoad().r;
    float roughness = g_metallicRoughAo.SubpassLoad().g;
    float ao = g_metallicRoughAo.SubpassLoad().b;
    float4 position = g_position.SubpassLoad();

    if (baseColor.r < 0.001) {
       discard;
    }

    // Calculate final shading
    float3 viewDir = normalize(cameraPos - position.xyz);

    // Compute lighting
    Light directionalLight = lights[0];
    //float3 lightDir = normalize(directionalLight.direction);
    float3 lightDir = normalize(float3(0.1, 0.5, 1.0));
    //float3 lightColor = directionalLight.color;
    float3 lightColor = float3(1.0, 1.0, 1.0);
    float lightIntensity = directionalLight.intensity;

    // Calculate shadow factor
    float shadowFactor = ComputeShadow(position.xyz, normal, lightDir) * 1.0f;

    output.color = float4(0.0, 0.0, 0.0, shadowFactor);
    return output;
}