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

/// @brief Command Buffer class, only instanciable by VulkanCommandPool.
/// To use them, you need to create a VulkanCommandPool and then create/access them via VulkanCommandPool.
/// To make sure destruction and memory is handled in a correct way for command buffers, only VulkanCommandPool should
/// own them.
class VulkanCommandBuffer
{
private:
    VulkanCommandBuffer();
    ~VulkanCommandBuffer();
    VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
    VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;
    VulkanCommandBuffer(VulkanCommandBuffer&& other) = delete;
    VulkanCommandBuffer& operator=(VulkanCommandBuffer&& other) = delete;
    friend class VulkanCommandPool;

private:
    VkCommandBuffer __commandBuffer;

public:
    Error BeginCommandBuffer(VkCommandBufferUsageFlags flags = 0);
    Error EndCommandBuffer();
};

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
    Error CreateCommandBuffer(VulkanCommandBuffer ** commandBuffer, VkCommandBufferLevel level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);

private:
    VkCommandPool __commandPool;
    VkDevice __logicalDevice;
    std::vector<VulkanCommandBuffer> __commandBuffers;
};
}