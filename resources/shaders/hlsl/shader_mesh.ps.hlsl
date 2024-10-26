#include "Resources.ps.hlsl.h"

float4 main(PSInput input) : SV_TARGET {
    return float4(GetTexture(1, input.texCoord));
}
