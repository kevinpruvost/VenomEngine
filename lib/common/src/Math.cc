///
/// Project: VenomEngine
/// @file Math.cc
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/math/Math.h>

namespace venom
{
namespace common
{
namespace math
{
float Lerp(const float a, const float b, const float t)
{
#if defined(VENOM_MATH_GLM)
    return glm::mix(a, b, t);
#elif defined(VENOM_MATH_DXMATH)
    return DirectX::XMVectorLerp(a, b, t);
#endif
}

float PingPongLerp(const float a, const float b, const float t)
{
    float factor = abs(cosf(t * M_PI_2)); // Oscillate between 0 and 1
    return Lerp(a, b, factor);
}
}
}
}