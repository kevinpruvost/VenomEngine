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
    , __radius(10.0f)
{
}

Light::~Light()
{
}

void Light::Update(Entity entity)
{
}

void Light::_GUI(const Entity entity)
{
    Transform3D::_GUI(entity);

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
    if (__type == LightType::Point || __type == LightType::Spot) {
        vc::GUI::InputFloat("Radius", &__radius);
    }
    if (__type == LightType::Directional || __type == LightType::Spot) {
        vc::GUI::InputFloat3("Direction", &__direction.x);
    }
    vc::GUI::ColorEdit3("Color", &__color.x);
}

vc::String Light::_GetComponentTitle()
{
    return  ICON_MS_EMOJI_OBJECTS" Light";
}
}
}
