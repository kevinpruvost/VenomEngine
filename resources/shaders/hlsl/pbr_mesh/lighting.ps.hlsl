#include "../PBR.hlsl.h"

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

GBufferOutput main(VSOutput input, bool isFrontFace : SV_IsFrontFace) {
    GBufferOutput output;
    if (isFrontFace == false) {
        input.normal = -input.normal;
        input.tangent = -input.tangent;
        input.bitangent = -input.bitangent;
    }
    float2 uv = input.fragTexCoord;

    float4 baseColor;
    float3 normal;
    float4 specular;
    float metallic = 0.0;
    float roughness = 0.5;
    float ao = 1.0;
    float4 emissive;
    float3 position = input.worldPos;

    baseColor = MaterialComponentGetValue4(MaterialComponentType::BASE_COLOR, uv);

    // Normal in tangent space
    float3x3 TBN = transpose(float3x3(input.tangent, input.bitangent, input.normal));
    bool tangentSpace = material.components[MaterialComponentType::NORMAL].valueType == TEXTURE;
    if (tangentSpace) {
        normal = GetMaterialTexture(MaterialComponentType::NORMAL, uv).rgb;
        normal = normalize(mul(TBN, normal));
        // normal = normalize(normal * input.normal);
    } else
    {
        normal = normalize(input.normal);
    }

    // Specular
    if (material.components[MaterialComponentType::SPECULAR].valueType != NONE)
        specular = MaterialComponentGetValue4(MaterialComponentType::SPECULAR, uv);
    else
        specular = float4(0.5, 0.5, 0.5, 1.0);

    // Metallic (if PBR, then METALLIC otherwise 0)
    if (material.components[MaterialComponentType::METALLIC].valueType != NONE)
        metallic = MaterialComponentGetValue1(MaterialComponentType::METALLIC, uv);
    // Roughness (if PBR, then ROUGHNESS otherwise square of 2/(SPECULAR+2))
    if (material.components[MaterialComponentType::ROUGHNESS].valueType != NONE)
        roughness = MaterialComponentGetValue1(MaterialComponentType::ROUGHNESS, uv);
    else if (material.components[MaterialComponentType::SHININESS].valueType != NONE)
        roughness = MaterialComponentGetValue1(MaterialComponentType::SHININESS, uv);
    // Ambient occlusion
    if (material.components[MaterialComponentType::AMBIENT_OCCLUSION].valueType != NONE)
        ao = MaterialComponentGetValue1(MaterialComponentType::AMBIENT_OCCLUSION, uv);

    if (disableMetallic != 0) {
        metallic = constant_metallic;
    }
    if (disableRoughness != 0) {
        roughness = constant_roughness;
    }

    // Define additional material parameters for the Disney BRDF
    float subsurface = 0.0f;  // Subsurface scattering amount
    float specularVal = 0.4; // Specular intensity
    float specularTint = 0.5; // Specular tint
    float anisotropic = 0.0; // Anisotropy
    float sheen = 0.0;       // Sheen amount
    float sheenTint = 0.0;   // Sheen tint
    float clearCoat = 0.0;   // Clear coat amount
    float clearCoatGloss = 0.0; // Clear coat glossiness

    float3 viewDir = normalize(cameraPos - position);

    // Switch of view dir to tangent space
    //if (tangentSpace) viewDir = mul(TBN, viewDir);

    // Loop over lights
    output.finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (uint i = 0; i < 1; ++i) {
        Light light = lights[i];

        // Compute BRDF for this light
        float3 lightColor = GetLightColor(light, position.xyz);
        float3 lightDir = GetLightDirection(light, position.xyz);

        // Switch of lightDir to tangent space
        //if (tangentSpace)
        //    lightDir = mul(TBN, lightDir);

        float3 radiance = lightColor * saturate(dot(normal, lightDir));

        output.finalColor.rgb += DisneyPrincipledBRDF(lightDir, viewDir, normal, input.tangent, input.bitangent, baseColor.rgb, metallic, roughness, subsurface, specularVal, specularTint, anisotropic, sheen, sheenTint, clearCoat, clearCoatGloss) * radiance;
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
        output.finalColor.r = output.finalColor.g = output.finalColor.b = dot(normal, viewDir);
    }

    // Set transparency
    output.finalColor.a = baseColor.a;
    output.finalColor.a = 1.0f;

    // Add ambient light
    //finalColor += ComputeAmbient(baseColor.rgb, normal, viewDir, metallic, roughness, float3(0.2, 0.2, 0.2), 0.01f);

    // Calculate shadow factor
    // float shadowFactor = ComputeShadow(position.xyz, normal, lightDir) * 1.0f;

    //finalColor *= ao;

    if (normalMapDraw != 0)
        output.finalColor = float4(normal, 1.0f);
    return output;
}