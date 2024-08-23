///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanFence.h
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanDebug.h"

namespace venom
{
class VulkanFence
{
public:
    VulkanFence();
    ~VulkanFence();
    VulkanFence(const VulkanFence &) = delete;
    VulkanFence & operator=(const VulkanFence &) = delete;
    VulkanFence(VulkanFence && other);
    VulkanFence & operator=(VulkanFence && other);

    Error InitFence(const VkDevice logicalDevice);

private:
    VkFence __fence;
    VkDevice __logicalDevice;
};
}