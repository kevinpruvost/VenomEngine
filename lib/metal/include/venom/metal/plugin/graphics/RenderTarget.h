///
/// Project: VenomEngineWorkspace
/// @file RenderTarget.h
/// @date Dec, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/RenderTarget.h>
#include <venom/vulkan/Framebuffer.h>
#include <venom/vulkan/plugin/graphics/Texture.h>

namespace venom
{
namespace vulkan
{
class VulkanRenderTarget : public vc::RenderTargetImpl
{
public:
    VulkanRenderTarget();

public:
    vc::Error __PrepareRenderTarget() override;

    //Framebuffer __framebuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
};
}
}