///
/// Project: VenomEngine
/// @file RenderPass.h
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/RenderingPipelineType.h>
#include <venom/vulkan/SwapChain.h>
#include <venom/vulkan/CommandPool.h>

namespace venom
{
namespace vulkan
{
class RenderPass
{
public:
    RenderPass();
    ~RenderPass();
    RenderPass(const RenderPass&) = delete;
    RenderPass& operator=(const RenderPass&) = delete;
    RenderPass(RenderPass&& other);
    RenderPass& operator=(RenderPass&& other);

    void Destroy();

    void SetRenderingType(const vc::RenderingPipelineType type);
    vc::Error InitRenderPass(const SwapChain * swapChain);
    vc::Error BeginRenderPass(SwapChain * swapChain, CommandBuffer * commandBuffer, int framebufferIndex);
    vc::Error EndRenderPass(CommandBuffer * commandBuffer);
    VkRenderPass GetVkRenderPass() const;

    static RenderPass * GetRenderPass(const vc::RenderingPipelineType type);
    static vc::Vector<RenderPass *> GetRenderPasses();

private:
    vc::Error __CreateNormalRenderPass(const SwapChain * swapChain);
    vc::Error __CreateDeferredShadowRenderPass(const SwapChain * swapChain);

private:
    VkRenderPass __renderPass;
    vc::RenderingPipelineType __type;
};
}
}