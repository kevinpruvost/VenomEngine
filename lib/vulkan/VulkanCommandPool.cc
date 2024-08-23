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
VulkanCommandBuffer::VulkanCommandBuffer()
    : __commandBuffer(VK_NULL_HANDLE)
{
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
}

Error VulkanCommandBuffer::BeginCommandBuffer(VkCommandBufferUsageFlags flags)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(__commandBuffer, &beginInfo) != VK_SUCCESS) {
        Log::Error("Failed to begin recording command buffer");
        return Error::Failure;
    }
    return Error::Success;
}

Error VulkanCommandBuffer::EndCommandBuffer()
{
    if (vkEndCommandBuffer(__commandBuffer) != VK_SUCCESS) {
        Log::Error("Failed to record command buffer");
        return Error::Failure;
    }
    return Error::Success;
}

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

Error VulkanCommandPool::CreateCommandBuffer(VulkanCommandBuffer** commandBuffer, VkCommandBufferLevel level)
{
    if (__logicalDevice == VK_NULL_HANDLE) {
        Log::Error("Command pool not initialized");
        return Error::InvalidUse;
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = __commandPool;
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;

    auto & newCommandBuffer = __commandBuffers.emplace_back();

    if (vkAllocateCommandBuffers(__logicalDevice, &allocInfo, &newCommandBuffer.__commandBuffer) != VK_SUCCESS) {
        Log::Error("Failed to allocate command buffer");
        return Error::Failure;
    }
    *commandBuffer = &newCommandBuffer;
    return Error::Success;
}
}