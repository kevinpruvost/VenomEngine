///
/// Project: VenomEngineWorkspace
/// @file RenderTarget.cc
/// @date Dec, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/RenderPass.h>
#include <venom/vulkan/plugin/graphics/RenderTarget.h>

#include "venom/common/plugin/graphics/GraphicsSettings.h"

namespace venom
{
namespace vulkan
{
VulkanRenderTarget::VulkanRenderTarget()
{
}

vc::Error VulkanRenderTarget::__PrepareRenderTarget()
{
    // Prepare framebuffers
    // for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i)
    // {
    //     __framebuffers[i].SetRenderPass(RenderPass::GetRenderPass(GetRenderingPipelineType()));
    //     __framebuffers[i].SetExtent(VkExtent2D{static_cast<unsigned int>(GetWidth()), static_cast<unsigned int>(GetHeight())});
    //     __framebuffers[i].SetLayers(1);
    //
    //     // Prepare attachments
    //     int samples = vc::GraphicsSettings::GetActiveSamplesMultisampling();
    //
    // }
    return vc::Error::Success;
}
}
}
