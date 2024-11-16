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

struct LightShaderStruct
{
    vcm::Vec3 position;
    LightType type;
    vcm::Vec3 color;
    float intensity;
    float radius;
    int padding[3];
};

class VENOM_COMMON_API Light : public Transform3D
{
public:
    Light();
    ~Light() override;

    inline void SetType(const LightType type) { __type = type; }
    inline const LightType & GetType() const { return __type; }
    inline void SetColor(const vcm::Vec3 & color) { __color = color; }
    inline const vcm::Vec3 & GetColor() const { return __color; }
    inline void SetIntensity(const float intensity) { __intensity = intensity; }
    inline const float & GetIntensity() const { return __intensity; }
    inline void SetRadius(const float radius) { __radius = radius; }
    inline const float & GetRadius() const { return __radius; }
    inline LightShaderStruct GetShaderStruct() const { return {GetPosition(), __type, __color, __intensity, __radius}; }

private:
    vcm::Vec3 __color;
    LightType __type;
    float __intensity;
    float __radius;
    uint32_t __lightID;
};

class VENOM_COMMON_API LightManager
{
public:
    LightManager();
    ~LightManager();

    static const vc::Vector<Light *> & GetLights();
    static LightManager * Get();

private:
    vc::Vector<Light *> __lights;

    friend class Light;
};
}
}