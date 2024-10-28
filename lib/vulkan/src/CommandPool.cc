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
#include <venom/vulkan/plugin/graphics/Shader.h>
#include <venom/vulkan/plugin/graphics/Material.h>

namespace venom::vulkan
{
CommandBuffer::CommandBuffer()
    : _commandBuffer(VK_NULL_HANDLE)
    , _queue(nullptr)
    , _isActive(false)
    , _lastBoundPipeline(VK_NULL_HANDLE)
{
}

CommandBuffer::~CommandBuffer()
{
}

CommandBuffer::CommandBuffer(CommandBuffer&& other)
    : _commandBuffer(other._commandBuffer)
{
    other._commandBuffer = VK_NULL_HANDLE;
}

CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other)
{
    if (this != &other) {
        _commandBuffer = other._commandBuffer;
        other._commandBuffer = VK_NULL_HANDLE;
    }
    return *this;
}

VkCommandBuffer CommandBuffer::GetVkCommandBuffer() const
{
    return _commandBuffer;
}

CommandBuffer::operator VkCommandBuffer() const
{
    return _commandBuffer;
}

vc::Error CommandBuffer::BeginCommandBuffer(VkCommandBufferUsageFlags flags)
{
    venom_assert(_isActive == false, "BeginCommandBuffer() called while already begun");
    venom_assert(_commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(_commandBuffer, &beginInfo) != VK_SUCCESS) {
        vc::Log::Error("Failed to begin recording command buffer");
        return vc::Error::Failure;
    }
    _isActive = true;
    return vc::Error::Success;
}

vc::Error CommandBuffer::EndCommandBuffer()
{
    venom_assert(_isActive == true, "EndCommandBuffer() called before BeginCommandBuffer()");
    venom_assert(_commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS) {
        vc::Log::Error("Failed to record command buffer");
        return vc::Error::Failure;
    }
    _isActive = false;
    return vc::Error::Success;
}

void CommandBuffer::Reset(VkCommandBufferResetFlags flags)
{
    vkResetCommandBuffer(_commandBuffer, flags);
    _lastBoundPipeline = VK_NULL_HANDLE;
}

bool CommandBuffer::BindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint)
{
    venom_assert(_commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    if (_lastBoundPipeline == pipeline) return true;
    vkCmdBindPipeline(_commandBuffer, bindPoint, pipeline);
    _lastBoundPipeline = pipeline;
    return false;
}

void CommandBuffer::SetViewport(const VkViewport& viewport) const
{
    venom_assert(_commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    vkCmdSetViewport(_commandBuffer, 0, 1, &viewport);
}

void CommandBuffer::SetScissor(const VkRect2D& scissor) const
{
    venom_assert(_commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    vkCmdSetScissor(_commandBuffer, 0, 1, &scissor);
}

void CommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount,
    uint32_t firstVertex, uint32_t firstInstance) const
{
    venom_assert(_commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    vkCmdDraw(_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void CommandBuffer::DrawMesh(const VulkanMesh * vulkanMesh, const int firstInstance, const VulkanShader & pipeline) const
{
    venom_assert(_commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    const IndexBuffer & indexBuffer = vulkanMesh->GetIndexBuffer();
    const auto vertexBuffers = vulkanMesh->GetVkVertexBuffers();
    const VkDeviceSize * offsets = vulkanMesh->GetOffsets();

    // Material
    if (vulkanMesh->HasMaterial()) {
        VulkanMaterial * material = vulkanMesh->GetMaterial().GetImpl()->ConstAs<VulkanMaterial>();

#ifdef VENOM_BINDLESS_TEXTURES
        // TODO: Bindless textures
#endif
        {
            // WARNING: Order is important because GetMaterialDescriptorSet() may update the uniform buffer and the textures at the same time

            // Bind material
            BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), vc::ShaderResourceTable::SetsIndex::SETS_INDEX_MATERIAL, 1, material->GetMaterialDescriptorSet().GetVkDescriptorSetPtr());

            // Bind textures (when not bindless)
            BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), vc::ShaderResourceTable::SetsIndex::SETS_INDEX_TEXTURES, 1, material->GetTextureDescriptorSet().GetVkDescriptorSetPtr());
        }
    }

    for (const auto & vertexBuffer : vertexBuffers) {
        vkCmdBindVertexBuffers(_commandBuffer, vertexBuffer.binding, 1, &vertexBuffer.buffer, offsets);
    }
    if (indexBuffer.GetVkBuffer() != VK_NULL_HANDLE) {
        vkCmdBindIndexBuffer(_commandBuffer, indexBuffer.GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(_commandBuffer, indexBuffer.GetVertexCount(), 1, 0, 0, firstInstance);
    } else {
        vkCmdDraw(_commandBuffer, vulkanMesh->GetVertexCount(), 1, 0, firstInstance);
    }
}

void CommandBuffer::DrawModel(const VulkanModel * vulkanModel, const int firstInstance, const VulkanShader & pipeline) const
{
    venom_assert(_commandBuffer != VK_NULL_HANDLE, "Command buffer not initialized");
    for (const vc::Mesh & mesh : vulkanModel->GetMeshes()) {
        DrawMesh(mesh.GetImpl()->As<VulkanMesh>(), firstInstance, pipeline);
    }
}

void CommandBuffer::PushConstants(const VulkanShader * shaderPipeline, VkShaderStageFlags stageFlags, uint32_t offset,
                                  uint32_t size, const void* pValues) const
{
    vkCmdPushConstants(_commandBuffer, shaderPipeline->GetPipelineLayout(), stageFlags, offset, size, pValues);
}

void CommandBuffer::CopyBufferToImage(const Buffer& srcBuffer, const Image& dstImage)
{
    VkBufferImageCopy region {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
        .imageOffset = {0, 0, 0},
        .imageExtent = {
            .width = dstImage.GetWidth(),
            .height = dstImage.GetHeight(),
            .depth = 1
        }
    };
    vkCmdCopyBufferToImage(_commandBuffer, srcBuffer.GetVkBuffer(), dstImage.GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void CommandBuffer::TransitionImageLayout(Image& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkImageMemoryBarrier barrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = 0,
        .dstAccessMask = 0,
        .oldLayout = oldLayout,
        .newLayout = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = image.GetVkImage(),
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
    };

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }

    VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED) {
        if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        } else if (newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        } else if (newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
            barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        } else {
            goto unsupported_layout;
        }
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    } else {
        unsupported_layout:
        venom_assert(false, "Unsupported layout transition");
    }

    vkCmdPipelineBarrier(_commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    image.__layout = newLayout;
}

void CommandBuffer::BindDescriptorSets(VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout,
                                       uint32_t firstSet, VkDescriptorSet vkDescriptors) const
{
    vkCmdBindDescriptorSets(_commandBuffer, vkPipelineBindPoint, vkPipelineLayout, firstSet, 1, &vkDescriptors, 0, nullptr);
}

void CommandBuffer::BindDescriptorSets(VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout,
                                       uint32_t firstSet, uint32_t descriptSetCount, const VkDescriptorSet * vkDescriptors) const
{
    vkCmdBindDescriptorSets(_commandBuffer, vkPipelineBindPoint, vkPipelineLayout, firstSet, descriptSetCount, vkDescriptors, 0, nullptr);
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
        .pCommandBuffers = &_commandBuffer,
        .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphore != VK_NULL_HANDLE ? 1 : 0),
        .pSignalSemaphores = &signalSemaphore
    };
    vkQueueSubmit(_queue->GetVkQueue(), 1, &submitInfo, fence);
}

void CommandBuffer::WaitForQueue() const
{
    vkQueueWaitIdle(_queue->GetVkQueue());
}

SingleTimeCommandBuffer::SingleTimeCommandBuffer()
    : CommandBuffer()
    , __commandPool(VK_NULL_HANDLE)
{
}

SingleTimeCommandBuffer::~SingleTimeCommandBuffer()
{
    if (_commandBuffer != VK_NULL_HANDLE) {
        if (_isActive) {
            EndCommandBuffer();
            SubmitToQueue();
            WaitForQueue();
        }
        // Free
        vkFreeCommandBuffers(LogicalDevice::GetVkDevice(), __commandPool, 1, &_commandBuffer);
    }
}

SingleTimeCommandBuffer::SingleTimeCommandBuffer(SingleTimeCommandBuffer&& other)
    : CommandBuffer(std::move(other))
    , __commandPool(other.__commandPool)
{
}

SingleTimeCommandBuffer& SingleTimeCommandBuffer::operator=(SingleTimeCommandBuffer&& other)
{
    if (this != &other) {
        CommandBuffer::operator=(std::move(other));
        __commandPool = other.__commandPool;
    }
    return *this;
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
            commandBuffers[i] = __commandBuffers[i]->_commandBuffer;
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
    if (vkAllocateCommandBuffers(LogicalDevice::GetVkDevice(), &allocInfo, &newCommandBuffer->_commandBuffer) != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate command buffer");
        return vc::Error::Failure;
    }
    newCommandBuffer->_queue = __queue;
    *commandBuffer = newCommandBuffer.get();
    return vc::Error::Success;
}

vc::Error CommandPool::CreateSingleTimeCommandBuffer(SingleTimeCommandBuffer & commandBuffer)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = __commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(LogicalDevice::GetVkDevice(), &allocInfo, &commandBuffer._commandBuffer) != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate command buffer");
        return vc::Error::Failure;
    }
    commandBuffer._queue = __queue;
    commandBuffer.__commandPool = __commandPool;
    commandBuffer.BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    return vc::Error::Success;
}
}
