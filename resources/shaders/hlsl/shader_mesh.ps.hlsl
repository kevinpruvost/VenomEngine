#include "Resources.ps.hlsl.h"

struct PSOutput {
    float4 outColor : SV_Target0; // Equivalent to gl_FragColor in GLSL
};

PSOutput main(PSInput input) {
    PSOutput output;
    output.outColor = GetTexture(1, input.texCoord);
    return output;
}
