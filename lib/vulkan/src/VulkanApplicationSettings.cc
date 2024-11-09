///
/// Project: VenomEngineWorkspace
/// @file VulkanApplicationSettings.cc
/// @date Nov, 03 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>

#include <venom/common/ECS.h>

#include "venom/common/plugin/graphics/GUI.h"

namespace venom
{
namespace vulkan
{

vc::Error VulkanApplication::_LoadGfxSettings()
{
    vc::Error err;

    // If the multisampling is dirty, we need to recreate the swap chain, render pass and shaders
    if (_multisamplingDirty)
    {
        vkDeviceWaitIdle(LogicalDevice::GetVkDevice());
        __swapChain.CleanSwapChain();
        // Create Surface
        __surface.CreateSurface(vc::Context::Get());
        // ReCreate Render Pass
        if (err = __swapChain.InitSwapChainSettings(&__surface); err != vc::Error::Success)
            return err;
        if (err = __swapChain.InitSwapChain(); err != vc::Error::Success)
            return err;
        if (err = __renderPass.InitRenderPass(&__swapChain); err != vc::Error::Success)
            return err;
        if (err = __swapChain.InitSwapChainFramebuffers(&__renderPass); err != vc::Error::Success)
            return err;

        // We also need to reset the last used semaphore
        err = __imageAvailableSemaphores[_currentFrame].InitSemaphore();
        if (err != vc::Error::Success)
            return err;

        vc::ECS::GetECS()->ForEach<vc::Shader>([&](vc::Entity entity, vc::Shader & shader)
        {
            shader.GetImpl()->As<VulkanShader>()->SetMultiSamplingCount(_samples);
            shader.GetImpl()->As<VulkanShader>()->LoadShaders();
        });
    }
    return err;
}

vc::Error VulkanApplication::_SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples)
{
    int iSamples = static_cast<int>(samples);

    // Default is MSAA
    switch (mode) {
        case MultiSamplingModeOption::None:
            __swapChain.SetSamples(1);
        break;
        case MultiSamplingModeOption::MSAA:
            __swapChain.SetSamples(iSamples);
        break;
        default:
            venom_assert(false, "Invalid MultiSamplingModeOption");
        return vc::Error::Failure;
    };
    __RecreateSwapChain();
    return vc::Error::Success;
}

vc::Vector<vc::GraphicsSettings::MultiSamplingCountOption> VulkanApplication::_GetAvailableMultisamplingOptions()
{
    vc::Vector<vc::GraphicsSettings::MultiSamplingCountOption> options;

    const auto & props = PhysicalDevice::GetUsedPhysicalDevice().GetProperties();
    for (int i = 1; i <= props.limits.framebufferColorSampleCounts; i <<= 1)
        options.push_back(static_cast<vc::GraphicsSettings::MultiSamplingCountOption>(i));
    return options;
}

vc::Error VulkanApplication::_SetHDR(bool enable)
{
    if (enable) {

    } else {

    }
    return vc::Error::Success;
}

}
}
