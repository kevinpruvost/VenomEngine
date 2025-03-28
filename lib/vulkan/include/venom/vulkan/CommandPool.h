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
#include <venom/vulkan/plugin/graphics/Model.h>
#include <venom/vulkan/Buffer.h>
#include <venom/vulkan/Image.h>

#include <memory>

#include "plugin/graphics/Texture.h"

namespace venom
{
namespace vulkan
{
class VulkanRenderPass;
class CommandBuffer;
class SingleTimeCommandBuffer;
class VulkanShaderPipeline;
class VulkanSkybox;
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
    vc::Vector<vc::UPtr<CommandBuffer>> __commandBuffers;
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
    friend class VulkanRenderPass;

    VkCommandBuffer GetVkCommandBuffer() const;
    const VkCommandBuffer * GetVkCommandBufferPtr() const;
    operator VkCommandBuffer() const;

public:
    vc::Error BeginCommandBuffer(VkCommandBufferUsageFlags flags = 0);
    vc::Error EndCommandBuffer();
public:
    void Reset(VkCommandBufferResetFlags flags);
    /**
    * @brief Bind a pipeline to the command buffer
    * @param pipeline The pipeline to bind
    * @param bindPoint The bind point of the pipeline
    * @return true if the pipeline was already bound, false otherwise
    */
    bool BindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint);
    bool BindPipeline(const VulkanShaderPipeline * pipeline);
    void SetViewport(const VkViewport& viewport) const;
    void SetScissor(const VkRect2D& scissor) const;
    void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
    void DrawVertices(const VertexBuffer & vertexBuffer) const;
    void DrawMesh(const VulkanMesh * vulkanMesh, const int firstInstance, const VulkanShaderPipeline & pipeline) const;
    void DrawModel(const VulkanModel * vulkanModel, const int firstInstance, const VulkanShaderPipeline & pipeline) const;
    void DrawSkybox(const VulkanSkybox * vulkanSkybox, const VulkanShaderPipeline * shader);

    inline void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) const { vkCmdDispatch(_commandBuffer, groupX, groupY, groupZ); }
    inline void PipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags,
        uint32_t memoryBarrierCount, const VkMemoryBarrier * pMemoryBarriers,
        uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier * pBufferMemoryBarriers,
        uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier * pImageMemoryBarriers) const
    {
        vkCmdPipelineBarrier(_commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    }
    inline void PipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags) const
    {
        vkCmdPipelineBarrier(_commandBuffer, srcStageMask, dstStageMask, dependencyFlags, 0, nullptr, 0, nullptr, 0, nullptr);
    }

    void ClearColorImage(const vc::Texture & texture, VkClearColorValue vkClearColorValue) const;
    void CopyImage(const Image& image, const Image& getImage);
    void CopySwapChainImage(const VkImage& image, const Image& getImage);
    void ClearAttachments(uint32_t i, VkImageAspectFlags vkImageAspectFlagBits, VkClearValue vkClearValue, const VkClearRect * rect, const int rectNumber) const;
    void CopyBufferToImage(const Buffer& srcBuffer, const Image& dstImage);
    void TransitionImageLayout(Image& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void TransitionImageLayout(Image& image, VkImageLayout oldLayout, VkImageLayout newLayout);
    void ChangeImageLayout(vc::Texture & image, const VkImageLayout oldLayout, const VkImageLayout newLayout);
    void ChangeImageLayout(vc::Texture & image, const VkImageLayout newLayout);

    void PushConstants(const vc::ShaderPipeline * shaderPipeline, VkShaderStageFlags stageFlags, const void * pValues, uint32_t offset, uint32_t size) const;
    template<typename T>
    inline void PushConstants(const vc::ShaderPipeline * shaderPipeline, VkShaderStageFlags stageFlags, const T * value, uint32_t offset = 0) const {
        PushConstants(shaderPipeline, stageFlags, value, offset, sizeof(T));
    }
    void PushConstants(const VulkanShaderPipeline * shaderPipeline, VkShaderStageFlags stageFlags, const void * pValues, uint32_t offset, uint32_t size) const;
    template<typename T>
    inline void PushConstants(const VulkanShaderPipeline * shaderPipeline, VkShaderStageFlags stageFlags, const void * pValues, uint32_t offset, uint32_t size) const {
    }

    void BindDescriptorSets(VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout,
        uint32_t firstSet, VkDescriptorSet vkDescriptors) const;
    void BindDescriptorSets(VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout,
        uint32_t firstSet, uint32_t descriptSetCount, const VkDescriptorSet * vkDescriptors) const;


    void SubmitToQueue(VkFence fence = VK_NULL_HANDLE, VkSemaphore waitSemaphore = VK_NULL_HANDLE, VkPipelineStageFlags waitStage = 0,
        VkSemaphore signalSemaphore = VK_NULL_HANDLE);
    void WaitForQueue() const;
private:
    void __TransitionImageLayout(VkImageMemoryBarrier & barrier, VkImageLayout oldLayout, VkImageLayout newLayout);
protected:
    VkCommandBuffer _commandBuffer;
    const Queue * _queue;
    bool _isActive;
    VkPipeline _lastBoundPipeline;
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
