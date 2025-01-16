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

Light::Light(const Light& other)
    : PluginObjectImplWrapper(other)
{
    __lights.emplace_back(this);
}

Light& Light::operator=(const Light& other)
{
    PluginObjectImplWrapper::operator=(other);
    __lights.emplace_back(this);
}

Light::Light(Light&& other) noexcept
    : PluginObjectImplWrapper(std::move(other))
{
    __lights.emplace_back(this);
}

Light& Light::operator=(Light&& other) noexcept
{
    if (this != &other) {
        PluginObjectImplWrapper::operator=(std::move(other));
        __lights.emplace_back(this);
    }
    return *this;
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
    return  ICON_MS_EMOJI_OBJECTS" Light";
}

bool Light::CanRemove(Entity entity)
{
    return true;
}

LightCascadedShadowMapConstantsStruct LightImpl::GetShadowMapConstantsStruct(const int cascadeIndex, const int faceIndex, Camera * const camera) const
{
    LightCascadedShadowMapConstantsStruct ret;
    ret.cascadeIndex = cascadeIndex;
    ret.lightType = static_cast<int>(__lightType);

    const CameraCascadedShadowMapData & csmCameraData = camera->GetImpl()->As<CameraImpl>()->GetCascadedShadowMapData();
    switch (__lightType)
    {
        case LightType::Directional: {
            vcm::Vec3 lightPos = csmCameraData.cascadeFrustumsCenters[cascadeIndex] + __transform->GetForwardVector() * csmCameraData.cascadeFrustumsRadius[cascadeIndex];
            vcm::Mat4 lightViewMatrix = vcm::LookAt(lightPos, csmCameraData.cascadeFrustumsCenters[cascadeIndex], vcm::Vec3(0.0f, 1.0f, 0.0f));
            vcm::Mat4 lightProjMatrix = vcm::Orthographic(-csmCameraData.cascadeFrustumsRadius[cascadeIndex], csmCameraData.cascadeFrustumsRadius[cascadeIndex], -csmCameraData.cascadeFrustumsRadius[cascadeIndex], csmCameraData.cascadeFrustumsRadius[cascadeIndex], 0.0f, csmCameraData.cascadeFrustumsRadius[cascadeIndex] * 2.0f);
            ret.lightSpaceMatrix = lightProjMatrix * lightViewMatrix;
            break;
        }
        case LightType::Point:
            break;
        case LightType::Spot:
            break;
        default: {
            vc::Log::Error("Light type not supported");
            break;
        }
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

    inline int AllocateLightIndex(const LightType type)
    {
        venom_assert(!__lightIndexPerType[static_cast<int>(type)].empty(), "No more light index available");
        int ret = __lightIndexPerType[static_cast<int>(type)].top();
        __lightIndexPerType[static_cast<int>(type)].pop();
        return ret;
    }

    inline void DeallocateLightIndex(const LightType type, const int index)
    {
        __lightIndexPerType[static_cast<int>(type)].push(index);
    }

    inline const size_t GetCountOfLightsOfType(const LightType type)
    {
        switch (type)
        {
            case LightType::Directional:
                return VENOM_CSM_MAX_DIRECTIONAL_LIGHTS - __lightIndexPerType[static_cast<int>(type)].size();
                break;
            case LightType::Point:
                return VENOM_CSM_MAX_POINT_LIGHTS - __lightIndexPerType[static_cast<int>(type)].size();
                break;
            case LightType::Spot:
                return VENOM_CSM_MAX_SPOT_LIGHTS - __lightIndexPerType[static_cast<int>(type)].size();
                break;
            default:
                break;
        }
        return __lightIndexPerType[static_cast<int>(type)].size();
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

const size_t Light::GetCountOfLightsOfType(const LightType type)
{
    return s_lightIndexAllocator.GetCountOfLightsOfType(type);
}
}
}
