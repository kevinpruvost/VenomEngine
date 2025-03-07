///
/// Project: VenomEngineWorkspace
/// @file Light.cc
/// @date Jan, 12 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/Light.h>

#include <venom/metal/MetalApplication.h>

namespace venom
{
namespace metal
{
MetalLight::MetalLight()
{
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        __lastCascades[i] = -1;
    }
}

MetalLight::~MetalLight()
{
}

vc::Error MetalLight::_SetType(const vc::LightType type)
{
    MetalApplication * app = vc::GraphicsApplication::Get()->DAs<MetalApplication>();

    return vc::Error::Success;
}

void MetalLight::_SetDescriptorsFromCascade(const int cascadeIndex)
{
    MetalApplication * app = vc::GraphicsApplication::Get()->DAs<MetalApplication>();

    if (__lastCascades[vc::GraphicsApplication::GetCurrentFrameInFlight()] != cascadeIndex)
    {
        __lastCascades[vc::GraphicsApplication::GetCurrentFrameInFlight()] = cascadeIndex;
        switch (GetLightType())
        {
            case vc::LightType::Directional:
            {
                break;
            }
            case vc::LightType::Spot:
            {
                break;
            }
            case vc::LightType::Point:
            {
                break;
            }
            default: break;
        }
    }
}

}
}
