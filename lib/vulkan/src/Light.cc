///
/// Project: VenomEngineWorkspace
/// @file Light.cc
/// @date Jan, 12 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/Light.h>

#include <venom/vulkan/VulkanApplication.h>

namespace venom
{
namespace vulkan
{
VulkanLight::VulkanLight()
{
}

VulkanLight::~VulkanLight()
{
}

vc::Error VulkanLight::_SetType(const vc::LightType type)
{
    VulkanApplication * app = vc::GraphicsApplication::Get()->DAs<VulkanApplication>();

    RenderPass & csmRenderPass = app->__shadowMapRenderPass;
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i)
    {
        for (int j = 0; j < VENOM_CSM_TOTAL_CASCADES; ++j)
        {
            __shadowMapFramebuffers[i][j].clear();
            switch (type)
            {
                case vc::LightType::Directional: {
                    VkExtent2D extent = {VENOM_CSM_DIRECTIONAL_DIMENSION, VENOM_CSM_DIRECTIONAL_DIMENSION};
                    Framebuffer & fb = __shadowMapFramebuffers[i][j].emplace_back();
                    if (__CreateFramebuffer(fb, csmRenderPass, *app->__shadowMapDirectionalImageViews[i][j][_lightIndexPerType], extent) != vc::Error::Success)
                        return vc::Error::Failure;
                    break;
                }
                case vc::LightType::Spot: {
                    VkExtent2D extent = {VENOM_CSM_SPOT_DIMENSION, VENOM_CSM_SPOT_DIMENSION};
                    Framebuffer & fb = __shadowMapFramebuffers[i][j].emplace_back();
                    if (__CreateFramebuffer(fb, csmRenderPass, *app->__shadowMapSpotImageViews[i][j][_lightIndexPerType], extent) != vc::Error::Success)
                        return vc::Error::Failure;
                    break;
                }
                case vc::LightType::Point: {
                    VkExtent2D extent = {VENOM_CSM_POINT_DIMENSION, VENOM_CSM_POINT_DIMENSION};
                    for (int k = 0; k < 6; ++k)
                    {
                        Framebuffer & fb = __shadowMapFramebuffers[i][j].emplace_back();
                        if (__CreateFramebuffer(fb, csmRenderPass, *app->__shadowMapPointImageViews[i][j][_lightIndexPerType * 6 + k], extent) != vc::Error::Success)
                            return vc::Error::Failure;
                    }
                    break;
                }
                default: {
                    vc::Log::Error("Light type not supported");
                    return vc::Error::Failure;
                }
            }
        }
    }
    return vc::Error::Success;
}

vc::Error VulkanLight::__CreateFramebuffer(Framebuffer& framebuffer, RenderPass& csmRenderPass, ImageView& imageView,
    VkExtent2D& extent)
{
    framebuffer.SetAttachment(0, *imageView.GetImage(), imageView);
    framebuffer.SetExtent(extent);
    framebuffer.SetLayers(1);
    framebuffer.SetRenderPass(&csmRenderPass);
    if (framebuffer.Init() != vc::Error::Success) {
        vc::Log::Error("Failed to create framebuffer");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

}
}