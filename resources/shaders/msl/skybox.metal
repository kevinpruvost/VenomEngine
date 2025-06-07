#include <metal_stdlib>

using namespace metal;

// Buffer index values shared between shader and C code to ensure Metal shader buffer inputs
// match Metal API buffer set calls.
typedef enum AAPLVertexInputIndex
{
    AAPLVertexInputIndexVerticesPos     = 0,
    AAPLVertexInputIndexVerticesColor     = 1,
    AAPLVertexInputIndexViewportSize = 2,
} AAPLVertexInputIndex;

// Vertex shader outputs and fragment shader inputs
struct RasterizerData
{
    // The [[position]] attribute of this member indicates that this value
    // is the clip space position of the vertex when this structure is
    // returned from the vertex function.
    float4 position [[position]];
    float4 color;
};

vertex RasterizerData vertexShader(uint vertexID [[vertex_id]],
             constant vector_float2 *verticesPos [[buffer(AAPLVertexInputIndexVerticesPos)]],
             constant vector_float4 *verticesColor [[buffer(AAPLVertexInputIndexVerticesColor)]],
             constant vector_uint2 *viewportSizePointer [[buffer(AAPLVertexInputIndexViewportSize)]])
{
    RasterizerData out;

    out.position = vector_float4(verticesPos[vertexID], 0.0, 1.0);

    // Pass the input color directly to the rasterizer.
    out.color = verticesColor[vertexID];

    return out;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]])
{
    return in.color;
}

