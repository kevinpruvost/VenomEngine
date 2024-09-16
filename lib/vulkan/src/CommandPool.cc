///
/// Project: VenomEngine
/// @file CommandPool.cc
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/CommandPool.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/QueueManager.h>
#include <venom/vulkan/Shader.h>

namespace venom::vulkan
{
CommandBuffer::CommandBuffer()
    : __commandBuffer(VK_NULL_HANDLE)
{
}

CommandBuffer::~CommandBuffer()
{
}

CommandBuffer::CommandBuffer(CommandBuffer&& other)
    : __commandBuffer(other.__commandBuffer)
{
    other.__commandBuffer = VK_NULL_HANDLE;
}

CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other)
{
    if (this == &other) return *this;
    __commandBuffer = other.__commandBuffer;
    other.__commandBuffer = VK_NULL_HANDLE;
    return *this;
}

VkCommandBuffer CommandBuffer::GetVkCommandBuffer() const
{
    return __commandBuffer;
}

CommandBuffer::operator VkCommandBuffer() const
{
    return __commandBuffer;
}

vc::Error CommandBuffer::BeginCommandBuffer(VkCommandBufferUsageFlags flags) const
{
    venom_assert(__commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(__commandBuffer, &beginInfo) != VK_SUCCESS) {
        vc::Log::Error("Failed to begin recording command buffer");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error CommandBuffer::EndCommandBuffer() const
{
    venom_assert(__commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    if (vkEndCommandBuffer(__commandBuffer) != VK_SUCCESS) {
        vc::Log::Error("Failed to record command buffer");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

void CommandBuffer::Reset(VkCommandBufferResetFlags flags)
{
    vkResetCommandBuffer(__commandBuffer, flags);
}

void CommandBuffer::BindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint) const
{
    venom_assert(__commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    vkCmdBindPipeline(__commandBuffer, bindPoint, pipeline);
}

void CommandBuffer::SetViewport(const VkViewport& viewport) const
{
    venom_assert(__commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    vkCmdSetViewport(__commandBuffer, 0, 1, &viewport);
}

void CommandBuffer::SetScissor(const VkRect2D& scissor) const
{
    venom_assert(__commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    vkCmdSetScissor(__commandBuffer, 0, 1, &scissor);
}

void CommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount,
    uint32_t firstVertex, uint32_t firstInstance) const
{
    venom_assert(__commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    vkCmdDraw(__commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void CommandBuffer::DrawMesh(const VulkanMesh& vulkanMesh) const
{
    venom_assert(__commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    const IndexBuffer & indexBuffer = vulkanMesh.GetIndexBuffer();
    const VkBuffer * vertexBuffers = vulkanMesh.GetVkVertexBuffers();
    VkDeviceSize offsets[] = {0, 0};
    vkCmdBindVertexBuffers(__commandBuffer, 0, vulkanMesh.GetBindingCount(), vertexBuffers, offsets);
    if (indexBuffer.GetVkBuffer() != VK_NULL_HANDLE) {
        vkCmdBindIndexBuffer(__commandBuffer, indexBuffer.GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(__commandBuffer, indexBuffer.GetVertexCount(), 1, 0, 0, 0);
    } else {
        vkCmdDraw(__commandBuffer, vulkanMesh.GetVertexCount(), 1, 0, 0);
    }
}

void CommandBuffer::PushConstants(const ShaderPipeline * shaderPipeline, VkShaderStageFlags stageFlags, uint32_t offset,
    uint32_t size, const void* pValues) const
{
    vkCmdPushConstants(__commandBuffer, shaderPipeline->GetPipelineLayout(), stageFlags, offset, size, pValues);
}

void CommandBuffer::BindDescriptorSets(VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout,
    uint32_t firstSet, uint32_t descriptSetCount, VkDescriptorSet vkDescriptors)
{
    vkCmdBindDescriptorSets(__commandBuffer, vkPipelineBindPoint, vkPipelineLayout, firstSet, descriptSetCount, &vkDescriptors, 0, nullptr);
}

void CommandBuffer::SubmitToQueue(VkFence fence, VkSemaphore waitSemaphore, VkPipelineStageFlags waitStage,
                                  VkSemaphore signalSemaphore)
{
    VkSubmitInfo submitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphore != VK_NULL_HANDLE ? 1 : 0),
        .pWaitSemaphores = &waitSemaphore,
        .pWaitDstStageMask = &waitStage,
        .commandBufferCount = 1,
        .pCommandBuffers = &__commandBuffer,
        .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphore != VK_NULL_HANDLE ? 1 : 0),
        .pSignalSemaphores = &signalSemaphore
    };
    vkQueueSubmit(__queue->GetVkQueue(), 1, &submitInfo, fence);
}

void CommandBuffer::WaitForQueue() const
{
    vkQueueWaitIdle(__queue->GetVkQueue());
}

CommandPool::CommandPool()
    : __commandPool(VK_NULL_HANDLE)
{
}

CommandPool::~CommandPool()
{
    if (__commandBuffers.size() != 0) {
        std::vector<VkCommandBuffer> commandBuffers(__commandBuffers.size());
        for (size_t i = 0; i < __commandBuffers.size(); ++i) {
            commandBuffers[i] = __commandBuffers[i]->__commandBuffer;
        }
        vkFreeCommandBuffers(LogicalDevice::GetVkDevice(), __commandPool,
            static_cast<uint32_t>(__commandBuffers.size()),
            commandBuffers.data()
        );
    }
    if (__commandPool != VK_NULL_HANDLE)
        vkDestroyCommandPool(LogicalDevice::GetVkDevice(), __commandPool, Allocator::GetVKAllocationCallbacks());
}

bool CommandPool::IsReady() const
{
    // TODO: Implement
    return true;
}

void CommandPool::SetQueue(Queue* queue)
{
    __queue = queue;
}

CommandPool::CommandPool(CommandPool&& other)
    : __commandPool(other.__commandPool)
{
    other.__commandPool = VK_NULL_HANDLE;
}

CommandPool& CommandPool::operator=(CommandPool&& other)
{
    if (this == &other) return *this;
    __commandPool = other.__commandPool;
    other.__commandPool = VK_NULL_HANDLE;
    return *this;
}

vc::Error CommandPool::Init(QueueFamilyIndex queueFamilyIndex)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    if (vkCreateCommandPool(LogicalDevice::GetVkDevice(), &poolInfo, Allocator::GetVKAllocationCallbacks(), &__commandPool) != VK_SUCCESS) {
        vc::Log::Error("Failed to create command pool with queue family index: %u", queueFamilyIndex);
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error CommandPool::CreateCommandBuffer(CommandBuffer** commandBuffer, VkCommandBufferLevel level)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = __commandPool;
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;

    std::unique_ptr<CommandBuffer> & newCommandBuffer = __commandBuffers.emplace_back(new CommandBuffer());
    if (vkAllocateCommandBuffers(LogicalDevice::GetVkDevice(), &allocInfo, &newCommandBuffer->__commandBuffer) != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate command buffer");
        return vc::Error::Failure;
    }
    newCommandBuffer->__queue = __queue;
    *commandBuffer = newCommandBuffer.get();
    return vc::Error::Success;
}
}
