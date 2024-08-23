///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanCommandPool.cc
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanCommandPool.h"

namespace venom
{
VulkanCommandPool::VulkanCommandPool()
    : __commandPool(VK_NULL_HANDLE)
    , __logicalDevice(VK_NULL_HANDLE)
{
}

VulkanCommandPool::~VulkanCommandPool()
{
    if (__commandPool != VK_NULL_HANDLE)
        vkDestroyCommandPool(__logicalDevice, __commandPool, nullptr);
}

VulkanCommandPool::VulkanCommandPool(VulkanCommandPool&& other)
    : __commandPool(other.__commandPool)
    , __logicalDevice(other.__logicalDevice)
{
    other.__commandPool = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
}

VulkanCommandPool& VulkanCommandPool::operator=(VulkanCommandPool&& other)
{
    if (this == &other) return *this;
    __commandPool = other.__commandPool;
    __logicalDevice = other.__logicalDevice;
    other.__commandPool = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
    return *this;
}

Error VulkanCommandPool::InitCommandPool(VkDevice logicalDevice, VulkanQueueFamilyIndex queueFamilyIndex)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &__commandPool) != VK_SUCCESS) {
        Log::Error("Failed to create command pool with queue family index: %u", queueFamilyIndex);
        return Error::Failure;
    }
    __logicalDevice = logicalDevice;
    return Error::Success;
}
}
