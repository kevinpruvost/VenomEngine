///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanRenderPass.h
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanSwapChain.h"

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
    VkRenderPass GetRenderPass() const;

private:
    VkRenderPass __renderPass;
    VkDevice __logicalDevice;
};
}