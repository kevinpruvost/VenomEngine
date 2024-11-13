///
/// Project: VenomEngineWorkspace
/// @file Light.h
/// @date Nov, 12 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/Transform3D.h>

namespace venom
{
namespace common
{
enum class LightType
{
    Directional = 0,
    Point = 1,
    Spot = 2
};

class VENOM_COMMON_API Light : public Transform3D
{
public:
    Light();

    inline void SetType(const LightType type) { __type = type; }
    inline const LightType & GetType() const { return __type; }
    inline void SetColor(const vcm::Vec3 & color) { __color = color; }
    inline const vcm::Vec3 & GetColor() const { return __color; }
    inline void SetIntensity(const float intensity) { __intensity = intensity; }
    inline const float & GetIntensity() const { return __intensity; }

private:
    LightType __type;
    vcm::Vec3 __color;
    float __intensity;
};
}
}