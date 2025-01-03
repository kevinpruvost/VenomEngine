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
    vcm::Vec3 direction;
    float radius;
};

class VENOM_COMMON_API Light : public Component
{
public:
    Light();
    ~Light() override;

    void Init(Entity entity) override;
    void Update(Entity entity) override;
    void _GUI(const Entity entity) override;
    vc::String _GetComponentTitle() override;
    bool CanRemove(Entity entity) override;

    inline void SetType(const LightType type) { __type = type; }
    inline const LightType & GetType() const { return __type; }
    inline void SetColor(const vcm::Vec3 & color) { __color = color; }
    inline const vcm::Vec3 & GetColor() const { return __color; }
    inline void SetIntensity(const float intensity) { __intensity = intensity; }
    inline const float & GetIntensity() const { return __intensity; }
    inline float * GetIntensityPtr() { return &__intensity; }
    inline void SetRadius(const float radius) { __radius = radius; }
    inline const float & GetRadius() const { return __radius; }
    inline void SetDirection(const vcm::Vec3 & direction) { __direction = direction; }
    inline float * GetDirectionPtr() { return &__direction.x; }
    inline const vcm::Vec3 & GetDirection() const { return __direction; }
    inline LightShaderStruct GetShaderStruct() const { return {__transform->GetPosition(), __type, __color, __intensity, __direction, __radius}; }

private:
    Transform3D * __transform;
    vcm::Vec3 __color;
    LightType __type;
    float __intensity;
    float __radius;
    uint32_t __lightID;
    vcm::Vec3 __direction;
};
}
}