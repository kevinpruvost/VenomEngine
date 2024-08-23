///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanCommandPool.h
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanQueueFamily.h"

namespace venom
{
class VulkanCommandPool
{
public:
    VulkanCommandPool();
    ~VulkanCommandPool();
    VulkanCommandPool(const VulkanCommandPool&) = delete;
    VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;
    VulkanCommandPool(VulkanCommandPool&& other);
    VulkanCommandPool& operator=(VulkanCommandPool&& other);

    Error InitCommandPool(VkDevice logicalDevice, VulkanQueueFamilyIndex queueFamilyIndex);

private:
    VkCommandPool __commandPool;
    VkDevice __logicalDevice;
};
}
