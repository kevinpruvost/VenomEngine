#include "Resources.ps.hlsl.h"

float4 main(PSInput input) : SV_TARGET {
    return GetTexture(1, input.texCoord);
    return ComputeMaterialColor(input.texCoord);
}
