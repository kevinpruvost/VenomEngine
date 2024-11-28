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
    float3x3 normalMatrix = transpose(inverse(models[input.instanceID]));
    float3 transformedNormal = normalize(mul(normalMatrix, input.inNormal));
    output.normal = transformedNormal; // Normalize for correctness

    float3 T, B;

    // Perturb normal, see http://www.thetenthplanet.de/archives/1180
    float3 q1 = ddx(output.worldPos);
    float3 q2 = ddy(output.worldPos);
    float2 st1 = ddx(input.inTexCoord);
    float2 st2 = ddy(input.inTexCoord);

    T = normalize(q1 * st2.y - q2 * st1.y);
    B = -normalize(cross(transformedNormal, T));

    // if (all(transformedNormal == float3(0.0, 1.0, 0.0)))
    //     T = normalize(cross(transformedNormal, float3(1.0, 0.0, 0.0)));
    // else
    //     T = normalize(cross(transformedNormal, float3(0.0, 1.0, 0.0)));
    // B = normalize(cross(transformedNormal, T));

    // T = normalize(mul(normalMatrix, input.inTangent));
    // B = normalize(mul(normalMatrix, input.inBitangent));
    //
    // T = normalize(T - dot(transformedNormal, T) * transformedNormal);
    // B = normalize(cross(transformedNormal, T) + 0.00001);

    output.tangent = T;
    output.bitangent = B;

    output.fragTexCoord = input.inTexCoord;              // Pass the texture
    return output;
}