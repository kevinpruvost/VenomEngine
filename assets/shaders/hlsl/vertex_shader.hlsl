struct VSOutput {
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

static const float2 positions[3] = {
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5)
};

static const float3 colors[3] = {
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
};

VSOutput main(uint id : SV_VertexID) {
    VSOutput output;
    output.position = float4(positions[id], 0.0, 1.0);
    output.color = colors[id];
    return output;
}
