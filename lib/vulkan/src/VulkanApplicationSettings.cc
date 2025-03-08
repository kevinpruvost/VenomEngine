///
/// Project: VenomEngineWorkspace
/// @file VulkanApplicationSettings.cc
/// @date Nov, 03 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <thread>
#include <venom/vulkan/VulkanApplication.h>

#include <venom/common/ECS.h>

#include "venom/common/plugin/graphics/GUI.h"

namespace venom
{
namespace vulkan
{

vc::Error VulkanApplication::_OnGfxSettingsChange()
{
    vc::Error err;

    if (__RecreateSwapChain() != vc::Error::Success) return vc::Error::Failure;
    // If the multisampling is dirty, we need to recreate the swap chain, render pass and shaders

    if (_multisamplingDirty || _hdrDirty)
    {
        static vc::ShaderPipeline vkShader;
            for (const auto & [key, shader] : vc::ShaderPipelineImpl::GetCachedObjects()) {
                if (!shader->IsType<VulkanShaderResource>()) continue;
                shader->GetHolder()->As<VulkanShaderPipeline>()->SetMultiSamplingCount(GetActiveSamplesMultisampling());
                shader->GetHolder()->As<VulkanShaderPipeline>()->LoadShaders();
            }
        if (err = vc::GUI::Get()->Reset(); err != vc::Error::Success)
            return err;
        _multisamplingDirty = _hdrDirty = false;
    }

    // Reset Render Targets
    for (auto & rt : vc::RenderTargetImpl::GetAllRenderTargets())
        if (err = rt->Reset(); err != vc::Error::Success)
            return err;
    return err;
}

vc::Error VulkanApplication::_OnGfxConstantsChange()
{
    __graphicsSettingsBuffer.WriteToBuffer(vc::GraphicsSettings::GetGfxSettingsDataPtr(), sizeof(vc::GraphicsSettingsData));
    _windowSizeDirty = false;
    return vc::Error::Success;
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
    return vc::Error::Success;
}

}
}
