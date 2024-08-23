///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanSemaphore.h
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanDebug.h"

namespace venom
{
class VulkanSemaphore
{
public:
    VulkanSemaphore();
    ~VulkanSemaphore();
    VulkanSemaphore(const VulkanSemaphore &) = delete;
    VulkanSemaphore & operator=(const VulkanSemaphore &) = delete;
    VulkanSemaphore(VulkanSemaphore && other);
    VulkanSemaphore & operator=(VulkanSemaphore && other);

    Error InitSemaphore(const VkDevice logicalDevice);

private:
    VkSemaphore __semaphore;
    VkDevice __logicalDevice;
};
}