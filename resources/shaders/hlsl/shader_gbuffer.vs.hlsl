// HLSL Vertex Shader for Vulkan using DXC

#include "Resources.vs.hlsl.h"

VSOutput main(VSInput input)
{
    VSOutput output;
    output.outPosition = float4(input.inPosition, 1.0); // Set the position
    output.outPosition = mul(models[input.instanceID], output.outPosition); // Apply the model matrix
    output.outPosition = mul(view, output.outPosition);  // Apply the view matrix
    output.outPosition = mul(proj, output.outPosition);  // Apply the projection matrix
    output.fragColor = float4(1.0, 1.0, 1.0, 1.0);       // Pass the color to the fragment shader

    // Transform normal
    float4x4 modelMatrix = models[input.instanceID];
    float3 transformedNormal = mul(transpose(inverse((float3x3)modelMatrix)), input.inNormal);
    output.normal = normalize(transformedNormal); // Normalize for correctness

    // Correct tangent transformation
    float3 transformedTangent = normalize(mul((float3x3)modelMatrix, input.inTangent));
    output.tangent = transformedTangent;

    // Correct bitangent transformation
    float3 transformedBitangent = normalize(mul((float3x3)modelMatrix, input.inBitangent));
    output.bitangent = transformedBitangent;

    output.fragTexCoord = input.inTexCoord;              // Pass the texture
    return output;
}