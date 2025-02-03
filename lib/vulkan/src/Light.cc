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
    : __shadowMapFramebuffers{nullptr}
    , __shadowMapDescriptorSet(DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_LightIndividual).AllocateSet())
{
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        __lastCascades[i] = -1;
    }
}

VulkanLight::~VulkanLight()
{
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_LightIndividual).FreeSet(__shadowMapDescriptorSet);
}

const DescriptorSet& VulkanLight::GetShadowMapDescriptorSet()
{
    return __shadowMapDescriptorSet->GetCurrentSet();
}

vc::Error VulkanLight::_SetType(const vc::LightType type)
{
    VulkanApplication * app = vc::GraphicsApplication::Get()->DAs<VulkanApplication>();

    RenderPass & csmRenderPass = app->__shadowMapRenderPass;
    __shadowMapFramebuffers.Reset(new vc::Array2D<vc::Vector<Framebuffer>, VENOM_MAX_FRAMES_IN_FLIGHT, VENOM_CSM_TOTAL_CASCADES>());
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i)
    {
        __lastCascades[i] = -1;
        for (int j = 0; j < VENOM_CSM_TOTAL_CASCADES; ++j)
        {
            switch (type)
            {
                case vc::LightType::Directional: {
                    VkExtent2D extent = {static_cast<uint32_t>(VENOM_CSM_DIRECTIONAL_DIMENSION) >> j, static_cast<uint32_t>(VENOM_CSM_DIRECTIONAL_DIMENSION) >> j};
                    Framebuffer & fb = (*__shadowMapFramebuffers)[i][j].emplace_back();
                    if (__CreateFramebuffer(fb, csmRenderPass, *app->__shadowMapDirectionalImageViews[i][j][_lightIndexPerType], extent) != vc::Error::Success)
                        return vc::Error::Failure;
                    // Update descriptor set
                    __shadowMapDescriptorSet->GroupUpdateImageViewPerFrame(i, *app->__shadowMapDirectionalImageViews[i][j][_lightIndexPerType], 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, j);
                    break;
                }
                case vc::LightType::Spot: {
                    VkExtent2D extent = {static_cast<uint32_t>(VENOM_CSM_SPOT_DIMENSION) >> j, static_cast<uint32_t>(VENOM_CSM_SPOT_DIMENSION) >> j};
                    Framebuffer & fb = (*__shadowMapFramebuffers)[i][j].emplace_back();
                    if (__CreateFramebuffer(fb, csmRenderPass, *app->__shadowMapSpotImageViews[i][j][_lightIndexPerType], extent) != vc::Error::Success)
                        return vc::Error::Failure;
                    // Update descriptor set

                    break;
                }
                case vc::LightType::Point: {
                    VkExtent2D extent = {static_cast<uint32_t>(VENOM_CSM_POINT_DIMENSION) >> j, static_cast<uint32_t>(VENOM_CSM_POINT_DIMENSION) >> j};
                    for (int k = 0; k < 6; ++k)
                    {
                        Framebuffer & fb = (*__shadowMapFramebuffers)[i][j].emplace_back();
                        if (__CreateFramebuffer(fb, csmRenderPass, *app->__shadowMapPointImageViews[i][j][_lightIndexPerType * 6 + k], extent) != vc::Error::Success)
                            return vc::Error::Failure;
                        // Update descriptor set

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

void VulkanLight::_SetDescriptorsFromCascade(const int cascadeIndex)
{
    VulkanApplication * app = vc::GraphicsApplication::Get()->DAs<VulkanApplication>();
    DescriptorSet & set =  __shadowMapDescriptorSet->GetCurrentSet();

    if (__lastCascades[vc::GraphicsApplication::GetCurrentFrameInFlight()] != cascadeIndex)
    {
        __lastCascades[vc::GraphicsApplication::GetCurrentFrameInFlight()] = cascadeIndex;
        switch (GetLightType())
        {
            case vc::LightType::Directional:
            {
                for (int i = 0; i < VENOM_CSM_TOTAL_CASCADES; ++i)
                    set.UpdateImageView(*app->__shadowMapDirectionalImageViews[vc::GraphicsApplication::GetCurrentFrameInFlight()][i][_lightIndexPerType], 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, i);
                break;
            }
            case vc::LightType::Spot:
            {
                set.UpdateImageView(*app->__shadowMapSpotImageViews[vc::GraphicsApplication::GetCurrentFrameInFlight()][cascadeIndex][_lightIndexPerType], 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, cascadeIndex);
                break;
            }
            case vc::LightType::Point:
            {
                for (int i = 0; i < 6; ++i)
                    set.UpdateImageView(*app->__shadowMapPointImageViews[vc::GraphicsApplication::GetCurrentFrameInFlight()][cascadeIndex][_lightIndexPerType * 6 + i], 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, i);
                break;
            }
            default: break;
        }
    }
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