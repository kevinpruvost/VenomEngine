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
static LightManager * s_lightManager = nullptr;
Light::Light()
    : __type(LightType::Directional)
    , __color(vcm::Vec3(1.0f, 1.0f, 1.0f))
    , __intensity(1.0f)
    , __radius(10.0f)
{
    s_lightManager->__lights.emplace_back(this);
}

Light::~Light()
{
    if (s_lightManager) s_lightManager->__lights.erase(std::remove(s_lightManager->__lights.begin(), s_lightManager->__lights.end(), this), s_lightManager->__lights.end());
}

LightManager::LightManager()
{
    venom_assert(s_lightManager == nullptr, "LightManager already exists");
    s_lightManager = this;
    __lights.reserve(VENOM_MAX_LIGHTS);
}

LightManager::~LightManager()
{
    s_lightManager = nullptr;
}

const vc::Vector<Light *> & LightManager::GetLights()
{
    return s_lightManager->__lights;
}

LightManager* LightManager::Get()
{
    return s_lightManager;
}
}
}
