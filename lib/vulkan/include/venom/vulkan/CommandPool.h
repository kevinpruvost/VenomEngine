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
#include <venom/vulkan/Buffer.h>
#include <venom/vulkan/Image.h>

#include <memory>

namespace venom
{
namespace vulkan
{
class RenderPass;
class CommandBuffer;
class SingleTimeCommandBuffer;
class ShaderPipeline;
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
    vc::Error CreateSingleTimeCommandBuffer(SingleTimeCommandBuffer & commandBuffer);

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
protected:
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
    vc::Error BeginCommandBuffer(VkCommandBufferUsageFlags flags = 0);
    vc::Error EndCommandBuffer();
public:
    void Reset(VkCommandBufferResetFlags flags);
    void BindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint) const;
    void SetViewport(const VkViewport& viewport) const;
    void SetScissor(const VkRect2D& scissor) const;
    void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
    void DrawMesh(const VulkanMesh& vulkanMesh) const;
    void PushConstants(const ShaderPipeline * shaderPipeline, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void * pValues) const;
    void CopyBuffer(const Buffer& srcBuffer, const Buffer& dstBuffer);
    void CopyBufferToImage(const Buffer& srcBuffer, const Image& dstImage);
    void TransitionImageLayout(Image& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void BindDescriptorSets(VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout,
        uint32_t firstSet, uint32_t descriptSetCount, VkDescriptorSet vkDescriptors);

    void SubmitToQueue(VkFence fence = VK_NULL_HANDLE, VkSemaphore waitSemaphore = VK_NULL_HANDLE, VkPipelineStageFlags waitStage = 0,
        VkSemaphore signalSemaphore = VK_NULL_HANDLE);
    void WaitForQueue() const;
protected:
    VkCommandBuffer _commandBuffer;
    const Queue * _queue;
    bool _isActive;
};

class SingleTimeCommandBuffer : public CommandBuffer
{
public:
    friend class CommandPool;
    SingleTimeCommandBuffer();
    ~SingleTimeCommandBuffer();
    SingleTimeCommandBuffer(const SingleTimeCommandBuffer&) = delete;
    SingleTimeCommandBuffer& operator=(const SingleTimeCommandBuffer&) = delete;
    SingleTimeCommandBuffer(SingleTimeCommandBuffer&& other);
    SingleTimeCommandBuffer& operator=(SingleTimeCommandBuffer&& other);

private:
    VkCommandPool __commandPool;
};
}
}
