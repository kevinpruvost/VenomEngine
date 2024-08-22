struct PSInput {
    float3 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET {
    return float4(input.color, 1.0);
}
