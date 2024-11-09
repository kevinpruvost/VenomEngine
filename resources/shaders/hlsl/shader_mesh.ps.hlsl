#include "Resources.ps.hlsl.h"

float4 main(PSInput input) : SV_TARGET {
    return ComputeMaterialColor(input.texCoord, input.position);
}
