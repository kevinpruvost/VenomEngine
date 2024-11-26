// HLSL Vertex Shader for Vulkan using DXC

#include "../Resources.vs.hlsl.h"

VSOutput main(VSInput input)
{
    VSOutput output;
    output.outPosition = float4(input.inPosition, 1.0); // Set the position
    output.outPosition = mul(models[input.instanceID], output.outPosition); // Apply the model matrix
    output.worldPos = output.outPosition.xyz; // Store the world position for later use
    output.outPosition = mul(view, output.outPosition);  // Apply the view matrix
    output.outPosition = mul(proj, output.outPosition);  // Apply the projection matrix
    output.fragColor = float4(1.0, 1.0, 1.0, 1.0);       // Pass the color to the fragment shader

    // Transform normal
    float4x4 normalMatrix = transpose(inverse(models[input.instanceID]));
    float3 transformedNormal = normalize(mul(normalMatrix, float4(input.inNormal, 0.0)).xyz);
    output.normal = transformedNormal; // Normalize for correctness

    // float3 T;
    // if (all(transformedNormal == float3(0.0, 1.0, 0.0)))
    //     T = normalize(cross(transformedNormal, float3(1.0, 0.0, 0.0)));
    // else
    //     T = normalize(cross(transformedNormal, float3(0.0, 1.0, 0.0)));
    // float3 B = normalize(cross(transformedNormal, T));

    float3 T = normalize(mul(models[input.instanceID], float4(input.inTangent, 0.0)).xyz);
    float3 B = normalize(mul(models[input.instanceID], float4(input.inBitangent, 0.0)).xyz);
    T = normalize(T - dot(T, transformedNormal) * transformedNormal);
    B = cross(transformedNormal, T);

    output.tangent = T;
    output.bitangent = B;

    output.fragTexCoord = input.inTexCoord;              // Pass the texture
    return output;
}