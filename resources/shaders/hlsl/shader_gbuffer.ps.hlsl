#include "Resources.ps.hlsl.h"

GBufferOutput main(VSOutput input) {
    return ComputeMaterialColor(input);
}