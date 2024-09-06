///
/// Project: VenomEngine
/// @file CommandPool.h
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/QueueFamily.h>
#include <venom/vulkan/plugin/graphics/Mesh.h>

#include <memory>

namespace venom
{
namespace vulkan
{
class RenderPass;
class CommandBuffer;
class Queue;

class CommandPool
{
public:
    CommandPool();
    ~CommandPool();
    bool IsReady() const;
    void SetQueue(Queue* queue);
    CommandPool(const CommandPool&) = delete;
    CommandPool& operator=(const CommandPool&) = delete;
    CommandPool(CommandPool&& other);
    CommandPool& operator=(CommandPool&& other);

    vc::Error Init(QueueFamilyIndex queueFamilyIndex);
    vc::Error CreateCommandBuffer(CommandBuffer ** commandBuffer, VkCommandBufferLevel level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);

private:
    VkCommandPool __commandPool;
    std::vector<std::unique_ptr<CommandBuffer>> __commandBuffers;
    Queue * __queue;
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

    VkCommandBuffer GetVkCommandBuffer() const;
    operator VkCommandBuffer() const;

public:
    vc::Error BeginCommandBuffer(VkCommandBufferUsageFlags flags = 0) const;
    vc::Error EndCommandBuffer() const;
public:
    void Reset(VkCommandBufferResetFlags flags);
    void BindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint) const;
    void SetViewport(const VkViewport& viewport) const;
    void SetScissor(const VkRect2D& scissor) const;
    void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
    void DrawMesh(const VulkanMesh& vulkanMesh) const;

    void SubmitToQueue(VkFence fence = VK_NULL_HANDLE, VkSemaphore waitSemaphore = VK_NULL_HANDLE, VkPipelineStageFlags waitStage = 0,
        VkSemaphore signalSemaphore = VK_NULL_HANDLE);
    void WaitForQueue() const;
private:
    VkCommandBuffer __commandBuffer;
    const Queue * __queue;
};
}
}
