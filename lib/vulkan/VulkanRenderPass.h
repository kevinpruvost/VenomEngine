///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanRenderPass.h
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanSwapChain.h"
#include "VulkanCommandPool.h"

namespace venom
{
class VulkanRenderPass
{
public:
    VulkanRenderPass();
    ~VulkanRenderPass();
    VulkanRenderPass(const VulkanRenderPass&) = delete;
    VulkanRenderPass& operator=(const VulkanRenderPass&) = delete;
    VulkanRenderPass(VulkanRenderPass&& other);
    VulkanRenderPass& operator=(VulkanRenderPass&& other);

    Error InitRenderPass(VkDevice logicalDevice, const VulkanSwapChain * swapChain);
    Error BeginRenderPass(VulkanSwapChain * swapChain, VulkanCommandBuffer * commandBuffer, int framebufferIndex);
    Error EndRenderPass(VulkanCommandBuffer * commandBuffer);
    VkRenderPass GetRenderPass() const;

private:
    VkRenderPass __renderPass;
    VkDevice __logicalDevice;
};
}