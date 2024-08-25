///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanCommandPool.h
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/QueueFamily.h>

#include <memory>

namespace venom
{
namespace vulkan
{
class RenderPass;
class CommandBuffer;

class CommandPool
{
public:
    CommandPool();
    ~CommandPool();
    CommandPool(const CommandPool&) = delete;
    CommandPool& operator=(const CommandPool&) = delete;
    CommandPool(CommandPool&& other);
    CommandPool& operator=(CommandPool&& other);

    Error InitCommandPool(const VkDevice logicalDevice, QueueFamilyIndex queueFamilyIndex);
    Error CreateCommandBuffer(CommandBuffer ** commandBuffer, VkCommandBufferLevel level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);

private:
    VkCommandPool __commandPool;
    VkDevice __logicalDevice;
    std::vector<std::unique_ptr<CommandBuffer>> __commandBuffers;
};

/// @brief Command Buffer class, only instanciable by VulkanCommandPool.
/// To use them, you need to create a VulkanCommandPool and then create/access them via VulkanCommandPool.
/// To make sure destruction and memory is handled in a correct way for command buffers, only VulkanCommandPool should
/// own them.
class CommandBuffer
{
private:
    CommandBuffer();
public:
    ~CommandBuffer();
    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer& operator=(const CommandBuffer&) = delete;
    CommandBuffer(CommandBuffer&& other);
    CommandBuffer& operator=(CommandBuffer&& other);
    friend class CommandPool;
    friend class RenderPass;

private:
    VkCommandBuffer __commandBuffer;

public:
    Error BeginCommandBuffer(VkCommandBufferUsageFlags flags = 0) const;
    Error EndCommandBuffer() const;
public:
    void Reset(VkCommandBufferResetFlags flags);
    void BindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint) const;
    void SetViewport(const VkViewport& viewport) const;
    void SetScissor(const VkRect2D& scissor) const;
    void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
};
}
}