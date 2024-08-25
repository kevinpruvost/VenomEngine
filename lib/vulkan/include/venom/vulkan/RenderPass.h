///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanRenderPass.h
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

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

    Error InitRenderPass(const VkDevice logicalDevice, const SwapChain * swapChain);
    Error BeginRenderPass(SwapChain * swapChain, CommandBuffer * commandBuffer, int framebufferIndex);
    Error EndRenderPass(CommandBuffer * commandBuffer);
    VkRenderPass GetRenderPass() const;

private:
    VkRenderPass __renderPass;
    VkDevice __logicalDevice;
};
}
}