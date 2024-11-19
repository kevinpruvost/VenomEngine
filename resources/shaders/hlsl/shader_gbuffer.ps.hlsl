#include "Resources.ps.hlsl.h"

GBufferOutput main(VSOutput input, bool isFrontFace : SV_IsFrontFace) {
    if (isFrontFace == false) {
        input.normal = -input.normal;
        input.tangent = -input.tangent;
        input.bitangent = -input.bitangent;
    }
    return ComputeMaterialColor(input);
}