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

void Light::_GUI()
{
    Transform3D::_GUI();

    // Select type of light
    const vc::Array<vc::String, 3> lightTypes = {"Directional", "Point", "Spot"};
    int type = static_cast<int>(__type);
    if (vc::GUI::BeginCombo("Type", lightTypes[type].c_str())) {
        for (int i = 0; i < lightTypes.size(); i++) {
            bool isSelected = (type == i);
            if (vc::GUI::Selectable(lightTypes[i].c_str(), isSelected)) {
                type = i;
                __type = static_cast<LightType>(i);
            }
            if (isSelected) {
                vc::GUI::SetItemDefaultFocus();
            }
        }
        vc::GUI::EndCombo();
    }

    // Other properties
    vc::GUI::InputFloat("Intensity", &__intensity);
    if (__type != LightType::Directional)
        vc::GUI::InputFloat("Radius", &__radius);
    vc::GUI::ColorEdit3("Color", &__color.x);
}

vc::String Light::_GetComponentTitle()
{
    return "Light";
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
