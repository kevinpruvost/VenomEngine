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
size_t Light::__countOfLights = 0;
Light::Light()
    : __type(LightType::Directional)
    , __color(vcm::Vec3(1.0f, 1.0f, 1.0f))
    , __intensity(1.0f)
{
    ++__countOfLights;
}

Light::~Light()
{
    --__countOfLights;
}

void Light::Init(Entity entity)
{
}

void Light::Update(Entity entity)
{
    __transform = entity.get_mut<Transform3D>();
}

void Light::_GUI(const Entity entity)
{
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
    if (__type == LightType::Spot) {
        vc::GUI::SliderFloat("Angle", &__angle, 0.0f, 180.0f);
    }
    vc::GUI::ColorEdit3("Color", &__color.x);
}

vc::String Light::_GetComponentTitle()
{
    return  ICON_MS_EMOJI_OBJECTS" Light";
}

bool Light::CanRemove(Entity entity)
{
    return true;
}
}
}
