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
class VulkanRenderPass;
class VulkanCommandBuffer;

class VulkanCommandPool
{
public:
    VulkanCommandPool();
    ~VulkanCommandPool();
    VulkanCommandPool(const VulkanCommandPool&) = delete;
    VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;
    VulkanCommandPool(VulkanCommandPool&& other);
    VulkanCommandPool& operator=(VulkanCommandPool&& other);

    Error InitCommandPool(const VkDevice logicalDevice, VulkanQueueFamilyIndex queueFamilyIndex);
    Error CreateCommandBuffer(VulkanCommandBuffer ** commandBuffer, VkCommandBufferLevel level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);

private:
    VkCommandPool __commandPool;
    VkDevice __logicalDevice;
    std::vector<VulkanCommandBuffer> __commandBuffers;
};

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
    VulkanCommandBuffer(VulkanCommandBuffer&& other);
    VulkanCommandBuffer& operator=(VulkanCommandBuffer&& other);
    friend class VulkanCommandPool;
    friend class VulkanRenderPass;

private:
    VkCommandBuffer __commandBuffer;

public:
    Error BeginCommandBuffer(VkCommandBufferUsageFlags flags = 0) const;
    Error EndCommandBuffer() const;
public:
    void BindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint) const;
    void SetViewport(const VkViewport& viewport) const;
    void SetScissor(const VkRect2D& scissor) const;
    void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
};
}
