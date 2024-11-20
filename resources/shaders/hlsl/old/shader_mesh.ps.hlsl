#include "Resources.ps.hlsl.h"

struct PSOutput {
    float4 outColor : SV_Target0; // Equivalent to gl_FragColor in GLSL
};

struct LocalVSOutput {
    float4 outPosition : SV_POSITION; // Equivalent to gl_Position in GLSL
    [[vk::location(0)]] float4 fragColor : TEXCOORD0;     // Equivalent to layout(location = 0) out in GLSL
    [[vk::location(1)]] float2 fragTexCoord : TEXCOORD1;  // Equivalent to layout(location = 1) out in GLSL
};

PSOutput main(LocalVSOutput input) {
    PSOutput output;
    output.outColor = GetTexture(1, input.fragTexCoord);
    return output;
}
