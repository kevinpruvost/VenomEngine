#version 450

#extension GL_GOOGLE_include_directive : require

#include "../PBR.glsl.h"

layout(push_constant, std430) uniform RenderData
{
    int i;
} lightData;

#define SHADOW_BIAS 0.0001

float pcf(texture2D map, vec2 uv, float depth, int gap)
{
    ivec2 tSize = textureSize(sampler2D(map, g_sampler), 0);
    float shadow = 0.0;
    const float bias = max(depth * 0.0001, SHADOW_BIAS);
    for (int x = -gap; x <= gap; ++x) {
        for (int y = -gap; y <= gap; ++y) {
            vec2 newUv = uv + vec2(x, y) / vec2(tSize);
            if (newUv.x > 1.0 || newUv.x < 0.0 || newUv.y > 1.0 || newUv.y < 0.0)
                continue;
            float pcfDepth = texture(sampler2D(map, g_ClampSampler), newUv).r;
            shadow += pcfDepth < (depth - bias) ? 1.0 : 0.0;
        }
    }
    return shadow / ((1 + gap * 2) * (1 + gap * 2));
}

vec3 rotateX(vec3 dir, float angle)
{
    float cosA = cos(angle);
    float sinA = sin(angle);

    return vec3(
        dir.x,                        // X remains unchanged
        dir.y * cosA - dir.z * sinA,  // Y' = Y * cos(θ) - Z * sin(θ)
        dir.y * sinA + dir.z * cosA   // Z' = Y * sin(θ) + Z * cos(θ)
    );
}

vec3 rotateY(vec3 dir, float angle)
{
    float cosA = cos(angle);
    float sinA = sin(angle);

    return vec3(
        dir.x * cosA + dir.z * sinA,  // X' = X * cos(θ) + Z * sin(θ)
        dir.y,                        // Y remains unchanged
        -dir.x * sinA + dir.z * cosA  // Z' = -X * sin(θ) + Z * cos(θ)
    );
}

float pcfPoint(vec3 dir, vec3 position, vec3 lightPos, int shadowMapIndex, int gap)
{
    float tSize = 1.0 / textureSize(sampler2D(shadowMaps[0], g_sampler), 0).x;
    float shadow = 0.0;
    float length = distance(lightPos, position);
    int loopNum = (gap * 2 + 1) * (gap * 2 + 1);

    int faceBase = GetFaceIndex(dir);
    vec4 clipSpaceBase = shadowMapsPointLightSpaceMatrices[faceBase + (6 * shadowMapIndex)] * vec4(position, 1.0);
    vec3 uvShadowBase = clipSpaceBase.xyz / clipSpaceBase.w;
    float depth = uvShadowBase.z;

    float bias = tSize;

    for (int x = -gap; x <= gap; ++x) {
        for (int y = -gap; y <= gap; ++y) {
            vec3 newDir = rotateX(rotateY(dir, x * tSize), y * tSize);
            vec3 newPosition = lightPos + newDir * length;

            int face = GetFaceIndex(newDir);
            vec4 clipSpace = shadowMapsPointLightSpaceMatrices[face + (6 * shadowMapIndex)] * vec4(newPosition, 1.0);
            vec3 uvShadow = clipSpace.xyz / clipSpace.w;
            uvShadow.y = -uvShadow.y;
            uvShadow.xy = uvShadow.xy * 0.5 + 0.5;
            if (uvShadow.z > 1.0 || uvShadow.z < 0.0 || uvShadow.x > 1.0 || uvShadow.x < 0.0 || uvShadow.y > 1.0 || uvShadow.y < 0.0) {
                continue;
            } else {
                float pcfDepth = texture(sampler2D(shadowMaps[face], g_ClampSampler), uvShadow.xy).r;
                shadow += (pcfDepth < (depth - bias) ? 1.0 : 0.0) / loopNum;
            }
        }
    }
    return shadow;
}

const vec2 poissonSamples[16] = vec2[](
    vec2( 0.00,  0.00),
    vec2( 0.25,  0.25),
    vec2(-0.25,  0.25),
    vec2( 0.25, -0.25),
    vec2(-0.25, -0.25),
    vec2( 0.50,  0.00),
    vec2( 0.00,  0.50),
    vec2( 0.50,  0.50),
    vec2(-0.50,  0.00),
    vec2( 0.00, -0.50),
    vec2(-0.50, -0.50),
    vec2( 0.75,  0.25),
    vec2( 0.25,  0.75),
    vec2(-0.75,  0.25),
    vec2(-0.25,  0.75),
    vec2( 0.75, -0.25)
);

float pcfPoisson(texture2D map, vec2 uv, float depth)
{
    ivec2 tSize = textureSize(sampler2D(map, g_sampler), 0);
    float shadow = 0.0;
    const float bias = max(depth * 0.001, 0.005);
    const int sampleCount = 16; // Number of samples in the Poisson disk pattern

    // Iterate through Poisson disk sample points
    for (int i = 0; i < sampleCount; ++i) {
        vec2 sampleOffset = poissonSamples[i]; // Get the sample offset
        vec2 sampleCoord = uv + sampleOffset / vec2(tSize); // Apply offset to UV coordinates
        float pcfDepth = texture(sampler2D(map, g_sampler), sampleCoord).r; // Sample shadow map

        // Compare depth values with a small bias to avoid shadow acne
        shadow += pcfDepth < (depth - bias) ? 1.0 : 0.0;
    }

    // Return the average shadow result
    return shadow / float(sampleCount);
}

// Placeholder for shadow calculation
float ComputeShadow(vec3 position, vec3 normal, Light light, int lightIndex)
{
    float shadow = 0.0;

    int shadowMapIndex = shadowMapsLayerIndices[lightIndex];
    if (light.type == LightType_Directional) {
        for (int i = 0; i < CASCADE_COUNT; ++i) {
           vec4 clipSpace = shadowMapsDirectionalLightSpaceMatrices[i + (CASCADE_COUNT * shadowMapIndex)] * vec4(position, 1.0);
           vec3 uvShadow = clipSpace.xyz / clipSpace.w;
           uvShadow.y = -uvShadow.y;
           uvShadow.xy = uvShadow.xy * 0.5 + 0.5;
           if (uvShadow.z > 1.0 || uvShadow.z < 0.0 || uvShadow.x > 1.0 || uvShadow.x < 0.0 || uvShadow.y > 1.0 || uvShadow.y < 0.0)
               continue;
           float shadowPCF = pcf(shadowMaps[i], uvShadow.xy, uvShadow.z, 3);
           shadow += shadowPCF;
           break;
        }
    } else if (light.type == LightType_Point) {
        vec3 dir = normalize(position - light.position);
        float shadowVal = pcfPoint(dir, position, light.position, shadowMapIndex, 1);
        //float shadowVal = 0.0;
        shadow = shadowVal;
    } else if (light.type == LightType_Spot) {
        vec4 clipSpace = shadowMapsSpotLightSpaceMatrices[shadowMapIndex] * vec4(position, 1.0);
        vec3 uvShadow = clipSpace.xyz / clipSpace.w;
        uvShadow.y = -uvShadow.y;
        uvShadow.xy = uvShadow.xy * 0.5 + 0.5;
        if (uvShadow.z > 1.0 || uvShadow.z < 0.0 || uvShadow.x > 1.0 || uvShadow.x < 0.0 || uvShadow.y > 1.0 || uvShadow.y < 0.0)
            return 0.0;
        float shadowVal = pcf(shadowMaps[0], uvShadow.xy, uvShadow.z, 1);
        //float shadowVal = texture(sampler2D(shadowMaps[0], g_sampler), uvShadow.xy).r;
        //shadow = shadowVal.r < (uvShadow.z - SHADOW_BIAS) ? 1.0 : 0.0;
        shadow = shadowVal;
    }
    return shadow;
}

vec3 GetLightDirection(Light light, vec3 position)
{
    // If no rotation at all, then light direction is towards bottom (0.0, -1.0, 0.0)
    if (light.type == LightType_Directional) {
        return (light.direction);
    } else if (light.type == LightType_Point) {
        return normalize(light.position - position);
    } else if (light.type == LightType_Spot) {
        return (light.direction);
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

layout(location = 1) out vec4 lightingResult;

void main()
{
    vec4 baseColor;
    vec3 normal;
    vec4 specular;
    float metallic = 0.0;
    float roughness = 1.0;
    float ao = 1.0;
    vec3 position = worldPos;
    float opacity = 1.0;

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
    lightingResult = vec4(0.0, 0.0, 0.0, opacity);
    finalColor = vec4(0.0, 0.0, 0.0, 0.0);

    if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_None)
    {
        if (lightData.i < lightCount)
        {
            Light light = lights[lightData.i];

            if (isLightInBlock(gl_FragCoord.xy, lightData.i) == false)
                return;

            float shadow = ComputeShadow(position, normal, light, lightData.i);

            if (shadow >= 0.995)
                return;

            // Compute BRDF for this light
            vec3 lightDir = GetLightDirection(light, position);
            vec3 lightColor = GetLightColor(light, position, lightDir);
            if (length(lightColor) < 0.01)
                return;

            vec3 radiance = lightColor * clamp(dot(normal, lightDir), 0.0, 1.0);
            vec3 colorToAdd = vec3(0.0, 0.0, 0.0);

            baseColor = MaterialComponentGetValue4(MaterialComponentType_BASE_COLOR, uv);
            baseColor = toLinear(baseColor);
            if (tangentSpace) {
                colorToAdd += DisneyPrincipledBSDF(lightDir, viewDir, normal, T, B, baseColor.rgb, metallic, roughness, subsurface, specularVal, specularTint, anisotropic, sheen, sheenTint, clearCoat, clearCoatGloss) * radiance;
            } else {
                colorToAdd += LambertCookTorrance(lightDir, viewDir, normal, baseColor.rgb, metallic, roughness) * radiance;
            }
            colorToAdd *= (1.0 - shadow);
            lightingResult.rgb += colorToAdd;
        }
    }
    else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_Depth) {
        lightingResult = vec4(vec3(gl_FragCoord.z), 1.0);
    } else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_Normals) {
        lightingResult = vec4(normal * 0.5 + 0.5, 1.0);
    } else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_ForwardPlus) {
        for (int i = 0; i < lightCount; ++i) {
            if (isLightInBlock(gl_FragCoord.xy, i) == false)
                continue;
            lightingResult.rgb += vec3(1.0, 1.0, 1.0) / float(lightCount);
        }
    } else if (graphicsSettings.debugVisualizationMode == DebugVisualizationMode_ShadowMapping) {
        for (int i = 0; i < lightCount; ++i) {
            Light light = lights[i];
            float shadow = ComputeShadow(position, normal, light, i);
            lightingResult.rgb += vec3(1.0 - shadow) * vec3(1.0, 1.0, 1.0) / float(lightCount);
        }
    }
}