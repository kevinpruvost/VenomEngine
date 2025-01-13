///
/// Project: VenomEngineWorkspace
/// @file Light.h
/// @date Nov, 12 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/GraphicsPluginObject.h>
#include <venom/common/Transform3D.h>

namespace venom
{
namespace common
{
class Camera;
class Light;

enum class LightType
{
    Directional = 0,
    Point = 1,
    Spot = 2,
    Count
};

struct LightShaderStruct
{
    vcm::Vec3 position;
    LightType type;
    vcm::Vec3 color;
    float intensity;
    vcm::Vec3 direction;
    float angle;
};

struct LightCascadedShadowMapConstantsStruct
{
    vcm::Mat4 lightSpaceMatrix;
    int lightType;
    int shadowMapIndex;
};

class VENOM_COMMON_API LightImpl : public PluginObjectImpl, public GraphicsPluginObject
{
public:
    LightImpl();
    ~LightImpl();

    vc::Error SetType(const LightType type);
    inline const LightType & GetLightType() const { return __lightType; }
    inline void SetColor(const vcm::Vec3 & color) { __color = color; }
    inline const vcm::Vec3 & GetColor() const { return __color; }
    inline void SetIntensity(const float intensity) { __intensity = intensity; }
    inline const float & GetIntensity() const { return __intensity; }
    inline float * GetIntensityPtr() { return &__intensity; }
    inline void SetAngle(const float angle) { __angle = angle; }
    inline const float & GetAngle() const { return __angle; }
    inline LightShaderStruct GetShaderStruct() const { return {__transform->GetPosition(), __lightType, __color, __intensity, __transform->GetRotation(), __angle}; }
    inline vc::Error Reinit() { return _SetType(__lightType); }
    LightCascadedShadowMapConstantsStruct GetShadowMapConstantsStruct(const int shadowMapIndex, const Camera * const camera) const;

protected:
    virtual vc::Error _SetType(const LightType type) = 0;

private:
    void __AllocateLightType();
    void __DeallocateLightType();

protected:
    int _lightIndexPerType;

private:
    Transform3D * __transform;
    vcm::Vec3 __color;
    LightType __lightType;
    float __intensity;
    float __angle;
    uint32_t __lightID;

    friend class Light;
};

class VENOM_COMMON_API Light : public Component, public PluginObjectImplWrapper
{
public:
    Light();
    ~Light() override;

    void Init(Entity entity) override;
    void Update(Entity entity) override;
    void _GUI(const Entity entity) override;
    vc::String _GetComponentTitle() override;
    bool CanRemove(Entity entity) override;
    inline void SetType(const LightType type) { _impl->As<LightImpl>()->SetType(type); }
    inline const LightType & GetLightType() const { return _impl->As<LightImpl>()->GetLightType(); }
    inline void SetColor(const vcm::Vec3 & color) { _impl->As<LightImpl>()->SetColor(color); }
    inline const vcm::Vec3 & GetColor() const { return _impl->As<LightImpl>()->GetColor(); }
    inline void SetIntensity(const float intensity) { _impl->As<LightImpl>()->SetIntensity(intensity); }
    inline const float & GetIntensity() const { return _impl->As<LightImpl>()->GetIntensity(); }
    inline float * GetIntensityPtr() { return _impl->As<LightImpl>()->GetIntensityPtr(); }
    inline void SetAngle(const float angle) { _impl->As<LightImpl>()->SetAngle(angle); }
    inline const float & GetAngle() const { return _impl->As<LightImpl>()->GetAngle(); }
    inline LightShaderStruct GetShaderStruct() const { return _impl->As<LightImpl>()->GetShaderStruct(); }
    inline LightCascadedShadowMapConstantsStruct GetShadowMapConstantsStruct(const int shadowMapIndex, const Camera * const camera) const { return _impl->As<LightImpl>()->GetShadowMapConstantsStruct(shadowMapIndex, camera); }

    static const size_t GetCountOfLights() { return __lights.size(); }
    static const vc::Vector<Light *> & GetLights() { return __lights; }

private:
    static vc::Vector<Light *> __lights;
};
}
}