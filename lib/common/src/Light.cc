///
/// Project: VenomEngineWorkspace
/// @file Light.cc
/// @date Nov, 12 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Light.h>
#include <venom/common/plugin/graphics/Camera.h>

namespace venom
{
namespace common
{
vc::Vector<Light *> Light::__lights;
LightImpl::LightImpl()
    : __lightType(LightType::Directional)
    , _lightIndexPerType(-1)
    , __color(vcm::Vec3(1.0f, 1.0f, 1.0f))
    , __intensity(1.0f)
{
    __AllocateLightType();
}

LightImpl::~LightImpl()
{
    __DeallocateLightType();
}

vc::Error LightImpl::SetType(const LightType type)
{
    __DeallocateLightType();

    __lightType = type;
    __AllocateLightType();

    return _SetType(type);
}

Light::Light()
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateLight())
{
    __lights.emplace_back(this);
}

Light::~Light()
{
    __lights.erase(std::remove(__lights.begin(), __lights.end(), this), __lights.end());
}

void Light::Init(Entity entity)
{
}

void Light::Update(Entity entity)
{
    GetImpl()->As<LightImpl>()->__transform = entity.get_mut<Transform3D>();
}

void Light::_GUI(const Entity entity)
{
    // Select type of light
    const vc::Array<vc::String, 3> lightTypes = {"Directional", "Point", "Spot"};
    int type = static_cast<int>(GetLightType());
    if (vc::GUI::BeginCombo("Type", lightTypes[type].c_str())) {
        for (int i = 0; i < lightTypes.size(); i++) {
            bool isSelected = (type == i);
            if (vc::GUI::Selectable(lightTypes[i].c_str(), isSelected)) {
                type = i;
                SetType(static_cast<LightType>(i));
            }
            if (isSelected) {
                vc::GUI::SetItemDefaultFocus();
            }
        }
        vc::GUI::EndCombo();
    }

    // Other properties
    vc::GUI::InputFloat("Intensity", &GetImpl()->As<LightImpl>()->__intensity);
    if (GetLightType() == LightType::Spot) {
        vc::GUI::SliderFloat("Angle", &GetImpl()->As<LightImpl>()->__angle, 0.0f, 180.0f);
    }
    vc::GUI::ColorEdit3("Color", &GetImpl()->As<LightImpl>()->__color.x);
}

vc::String Light::_GetComponentTitle()
{
    return  ICON_MS_EMOJI_OBJECTS" LightImpl";
}

bool Light::CanRemove(Entity entity)
{
    return true;
}

LightCascadedShadowMapConstantsStruct LightImpl::GetShadowMapConstantsStruct(const int shadowMapIndex, const Camera * const camera) const
{
    LightCascadedShadowMapConstantsStruct ret;
    ret.shadowMapIndex = shadowMapIndex;
    ret.lightType = static_cast<int>(__lightType);

    switch (__lightType)
    {
        case LightType::Directional:
            break;
        case LightType::Point:
            break;
        case LightType::Spot:
            break;
    }
    return ret;
}

class LightIndexAllocator
{
public:
    LightIndexAllocator()
    {
        for (int i = VENOM_CSM_MAX_DIRECTIONAL_LIGHTS - 1; i >= 0; --i) {
            __lightIndexPerType[static_cast<int>(LightType::Directional)].push(i);
        }
        for (int i = VENOM_CSM_MAX_POINT_LIGHTS - 1; i >= 0; --i) {
            __lightIndexPerType[static_cast<int>(LightType::Point)].push(i);
        }
        for (int i = VENOM_CSM_MAX_SPOT_LIGHTS - 1; i >= 0; --i) {
            __lightIndexPerType[static_cast<int>(LightType::Spot)].push(i);
        }
    }

    int AllocateLightIndex(const LightType type)
    {
        venom_assert(!__lightIndexPerType[static_cast<int>(type)].empty(), "No more light index available");
        int ret = __lightIndexPerType[static_cast<int>(type)].top();
        __lightIndexPerType[static_cast<int>(type)].pop();
        return ret;
    }

    void DeallocateLightIndex(const LightType type, const int index)
    {
        __lightIndexPerType[static_cast<int>(type)].push(index);
    }

private:
    vc::Stack<int> __lightIndexPerType[static_cast<size_t>(LightType::Count)];
};

static LightIndexAllocator s_lightIndexAllocator;
void LightImpl::__AllocateLightType()
{
    _lightIndexPerType = s_lightIndexAllocator.AllocateLightIndex(__lightType);
}

void LightImpl::__DeallocateLightType()
{
    s_lightIndexAllocator.DeallocateLightIndex(__lightType, _lightIndexPerType);
    _lightIndexPerType = -1;
}
}
}
