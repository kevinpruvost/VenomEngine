#include "Resources.ps.hlsl.h"

GBufferOutput main(PSInput input) {
    return ComputeMaterialColor(input);
}