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
    : PluginObjectWrapper(GraphicsPlugin::Get()->CreateLight())
{
    __lights.emplace_back(this);
}

Light::~Light()
{
    __lights.erase(std::remove(__lights.begin(), __lights.end(), this), __lights.end());
}

Light::Light(const Light& other)
    : PluginObjectWrapper(other)
{
    __lights.emplace_back(this);
}

Light& Light::operator=(const Light& other)
{
    PluginObjectWrapper::operator=(other);
    __lights.emplace_back(this);
    return *this;
}

Light::Light(Light&& other) noexcept
    : PluginObjectWrapper(std::move(other))
{
    __lights.erase(std::remove(__lights.begin(), __lights.end(), &other), __lights.end());
    if (std::find(__lights.begin(), __lights.end(), this) == __lights.end())
        __lights.emplace_back(this);
}

Light& Light::operator=(Light&& other) noexcept
{
    PluginObjectWrapper::operator=(std::move(other));
    if (this != &other) {
        __lights.erase(std::remove(__lights.begin(), __lights.end(), &other), __lights.end());
        if (std::find(__lights.begin(), __lights.end(), this) == __lights.end())
            __lights.emplace_back(this);
    }
    return *this;
}

void Light::Init(Entity entity)
{
    if (GetImpl()->As<LightImpl>()->_lightIndexPerType == -1) {
        SetType(LightType::Directional);
    }
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
    vc::GUI::SliderFloat("Intensity", &GetImpl()->As<LightImpl>()->__intensity, 0.0f, 100.0f);
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

LightCascadedShadowMapConstantsStruct LightImpl::GetShadowMapConstantsStruct(const int cascadeIndex, const int faceIndex, Camera * const camera, vcm::Vec3 * lightPos) const
{
    LightCascadedShadowMapConstantsStruct ret;
    ret.cascadeIndex = cascadeIndex;
    ret.lightType = static_cast<int>(__lightType);

    const CameraCascadedShadowMapData & csmCameraData = camera->GetImpl()->As<CameraImpl>()->GetCascadedShadowMapData();
    switch (__lightType)
    {
        case LightType::Directional: {
            vcm::Vec3 lightDir = GetDirection();
            vcm::Vec3 upVec(0.0f, 1.0f, 0.0f);
            if (fabs(vcm::DotProduct(lightDir, upVec)) > 0.99f) {
                upVec = vcm::Vec3(1.0f, 0.0f, 0.0f);
            }
            *lightPos = csmCameraData.cascadeFrustumsCenters[cascadeIndex] + (lightDir * csmCameraData.cascadeFrustumsRadius[cascadeIndex]);
            vcm::Mat4 lightViewMatrix = vcm::LookAt(*lightPos, csmCameraData.cascadeFrustumsCenters[cascadeIndex], upVec);
            vcm::Mat4 lightProjMatrix = vcm::Orthographic(
                -csmCameraData.cascadeFrustumsRadius[cascadeIndex],
                csmCameraData.cascadeFrustumsRadius[cascadeIndex],
                -csmCameraData.cascadeFrustumsRadius[cascadeIndex],
                csmCameraData.cascadeFrustumsRadius[cascadeIndex],
                0.0f, csmCameraData.cascadeFrustumsRadius[cascadeIndex] * 2.0f
            );
            ret.lightSpaceMatrix = lightProjMatrix * lightViewMatrix;
            break;
        }
        case LightType::Point: {
            const float lightRadius = sqrt(__intensity / POINTLIGHT_THRESHHOLD);
            const vcm::Vec3 lightDirs[6] = {
                {1.0f, 0.0f, 0.0f}, // Right
                {-1.0f, 0.0f, 0.0f}, // Left
                {0.0f, 1.0f, 0.0f}, // Top
                {0.0f, -1.0f, 0.0f}, // Bottom
                {0.0f, 0.0f, 1.0f}, // Front
                {0.0f, 0.0f, -1.0f} // Back
            };
            vcm::Vec3 upVec(0.0f, 1.0f, 0.0f);
            if (fabs(vcm::DotProduct(lightDirs[faceIndex], upVec)) > 0.99f) {
                upVec = vcm::Vec3(1.0f, 0.0f, 0.0f);
            }
            *lightPos = __transform->GetPosition();
            vcm::Mat4 lightViewMatrix = vcm::LookAt(*lightPos, *lightPos + lightDirs[faceIndex], upVec);
            vcm::Mat4 lightProjMatrix = vcm::Perspective(vcm::Radians(90.0f), 1.0f, 0.01f, lightRadius * 2.0f);
            ret.lightSpaceMatrix = lightProjMatrix * lightViewMatrix;
            break;
        }
        case LightType::Spot: {
            const float lightRadius = sqrt(__intensity / SPOTLIGHT_THRESHHOLD);
            *lightPos = __transform->GetPosition();
            const vcm::Vec3 direction = GetDirection();
            const vcm::Vec3 focusPoint = *lightPos - direction;
            vcm::Vec3 upVector = vcm::Vec3(0.0f, 1.0f, 0.0f);
            if (fabs(vcm::DotProduct(direction, upVector)) > 0.99f) {
                upVector = vcm::Vec3(1.0f, 0.0f, 0.0f);
            }
            vcm::Mat4 lightViewMatrix = vcm::LookAt(*lightPos, focusPoint, upVector);
            vcm::Mat4 lightProjMatrix = vcm::Perspective(vcm::Radians(__angle), 1.0f, 0.01f, lightRadius * 2.0f);
            ret.lightSpaceMatrix = lightProjMatrix * lightViewMatrix;
            break;
        }
        default: {
            vc::Log::Error("Light type not supported");
            break;
        }
    }
    return ret;
}

int LightImpl::GetCascadeIndex(Camera* const camera)
{
    const CameraCascadedShadowMapData & csmCameraData = camera->GetImpl()->As<CameraImpl>()->GetCascadedShadowMapData();

    int cascadeIndex = -1;
    switch (__lightType) {
        case LightType::Point: {
            const vcm::Vec3 & lightCenter = __transform->GetPosition();
            const float lightRadius = sqrt(__intensity / POINTLIGHT_THRESHHOLD);
            for (int i = 0; i < VENOM_CSM_TOTAL_CASCADES; ++i) {
                if (vcm::Distance(csmCameraData.cascadeFrustumsCenters[i], lightCenter) - lightRadius < csmCameraData.cascadeFrustumsRadius[i]) {
                    cascadeIndex = i;
                    break;
                }
            }
            break;
        }
        case LightType::Spot: {
            vcm::Vec3 lightCenter = __transform->GetPosition();
            const float lightRadius = sqrt(__intensity / SPOTLIGHT_THRESHHOLD);
            lightCenter += GetDirection() * lightRadius;
            for (int i = 0; i < VENOM_CSM_TOTAL_CASCADES; ++i) {
                if (vcm::Distance(csmCameraData.cascadeFrustumsCenters[i], lightCenter) - lightRadius < csmCameraData.cascadeFrustumsRadius[i]) {
                    cascadeIndex = i;
                    break;
                }
            }
            break;
        }
        default:
            cascadeIndex = -1;
    }
    if (cascadeIndex != -1) {
        _SetDescriptorsFromCascade(cascadeIndex);
    }
    return cascadeIndex;
}

vcm::Vec3 SpotAndDirectionalDirection(const vcm::Vec3& direction)
{
    // Rotate the default light direction (0, -1, 0) using the light's direction as Euler angles
    float cx = cos(direction.x);
    float sx = sin(direction.x);
    float cy = cos(direction.y);
    float sy = sin(direction.y);
    float cz = cos(direction.z);
    float sz = sin(direction.z);

    // Direct computation of the rotated direction
    vcm::Vec3 dir;
    dir.x = -cy * sz;
    dir.y = -sx * sy * sz - cx * cz;
    dir.z = -cx * sy * sz + sx * cz;

    vcm::Normalize(dir);
    return dir;
}

vcm::Vec3 LightImpl::GetDirection() const
{
    const vcm::Vec3 & dir = __transform->GetRotation();
    switch (__lightType) {
        case LightType::Directional:
        case LightType::Spot:
            // Default dir is going towards bottom (0, -1.0, 0), so apply rotation
            return SpotAndDirectionalDirection(dir);
        case LightType::Point:
        default:
            return dir;
    }
}

class LightIndexAllocator
{
public:
    LightIndexAllocator()
    {
        for (int i = 0; i < VENOM_CSM_MAX_DIRECTIONAL_LIGHTS; ++i) {
            __lightIndexPerType[static_cast<int>(LightType::Directional)].emplace_back(i);
        }
        for (int i = 0; i < VENOM_CSM_MAX_POINT_LIGHTS; ++i) {
            __lightIndexPerType[static_cast<int>(LightType::Point)].emplace_back(i);
        }
        for (int i = 0; i < VENOM_CSM_MAX_SPOT_LIGHTS; ++i) {
            __lightIndexPerType[static_cast<int>(LightType::Spot)].emplace_back(i);
        }
    }

    inline int AllocateLightIndex(const LightType type)
    {
        venom_assert(!__lightIndexPerType[static_cast<int>(type)].empty(), "No more light index available");
        int ret = __lightIndexPerType[static_cast<int>(type)].front();
        __lightIndexPerType[static_cast<int>(type)].erase(__lightIndexPerType[static_cast<int>(type)].begin());
        return ret;
    }

    inline void DeallocateLightIndex(const LightType type, const int index)
    {
        __lightIndexPerType[static_cast<int>(type)].push_back(index);
        // Sort to always have the smallest index at the end
        std::sort(__lightIndexPerType[static_cast<int>(type)].begin(), __lightIndexPerType[static_cast<int>(type)].end());
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
    // Stacks aren't sortable
    vc::Vector<int> __lightIndexPerType[static_cast<size_t>(LightType::Count)];
};

static LightIndexAllocator s_lightIndexAllocator;
void LightImpl::__AllocateLightType()
{
    _lightIndexPerType = s_lightIndexAllocator.AllocateLightIndex(__lightType);
}

void LightImpl::__DeallocateLightType()
{
    if (_lightIndexPerType != -1) {
        s_lightIndexAllocator.DeallocateLightIndex(__lightType, _lightIndexPerType);
        _lightIndexPerType = -1;
    }
}

const size_t Light::GetCountOfLightsOfType(const LightType type)
{
    return s_lightIndexAllocator.GetCountOfLightsOfType(type);
}
}
}
