///
/// Project: VenomEngineWorkspace
/// @file Light.cc
/// @date Nov, 12 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Light.h>

namespace venom
{
namespace common
{
Light::Light()
    : __type(LightType::Directional)
    , __color(vcm::Vec3(1.0f, 1.0f, 1.0f))
    , __intensity(1.0f)
{
}
}
}