///
/// Project: VenomEngine
/// @file CommandPool.cc
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/CommandPool.h>

namespace venom::vulkan
{
CommandBuffer::CommandBuffer()
    : __commandBuffer(VK_NULL_HANDLE)
{
    // Important to check because of allocation & free of command buffers
    static_assert(sizeof(CommandBuffer) == sizeof(VkCommandBuffer), "Unexpected Command Buffer size!");
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

CommandPool::CommandPool()
    : __commandPool(VK_NULL_HANDLE)
    , __logicalDevice(VK_NULL_HANDLE)
{
}

CommandPool::~CommandPool()
{
    if (__commandBuffers.size() != 0) {
        std::vector<VkCommandBuffer> commandBuffers(__commandBuffers.size());
        for (size_t i = 0; i < __commandBuffers.size(); ++i) {
            commandBuffers[i] = __commandBuffers[i]->__commandBuffer;
        }
        vkFreeCommandBuffers(__logicalDevice, __commandPool,
            static_cast<uint32_t>(__commandBuffers.size()),
            commandBuffers.data()
        );
    }
    if (__commandPool != VK_NULL_HANDLE)
        vkDestroyCommandPool(__logicalDevice, __commandPool, nullptr);
}

CommandPool::CommandPool(CommandPool&& other)
    : __commandPool(other.__commandPool)
    , __logicalDevice(other.__logicalDevice)
{
    other.__commandPool = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
}

CommandPool& CommandPool::operator=(CommandPool&& other)
{
    if (this == &other) return *this;
    __commandPool = other.__commandPool;
    __logicalDevice = other.__logicalDevice;
    other.__commandPool = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
    return *this;
}

vc::Error CommandPool::InitCommandPool(const VkDevice logicalDevice, QueueFamilyIndex queueFamilyIndex)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &__commandPool) != VK_SUCCESS) {
        vc::Log::Error("Failed to create command pool with queue family index: %u", queueFamilyIndex);
        return vc::Error::Failure;
    }
    __logicalDevice = logicalDevice;
    return vc::Error::Success;
}

vc::Error CommandPool::CreateCommandBuffer(CommandBuffer** commandBuffer, VkCommandBufferLevel level)
{
    if (__logicalDevice == VK_NULL_HANDLE) {
        vc::Log::Error("Command pool not initialized");
        return vc::Error::InvalidUse;
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = __commandPool;
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;

    __commandBuffers.emplace_back(new CommandBuffer());
    auto & newCommandBuffer = __commandBuffers.back();

    if (vkAllocateCommandBuffers(__logicalDevice, &allocInfo, &newCommandBuffer->__commandBuffer) != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate command buffer");
        return vc::Error::Failure;
    }
    *commandBuffer = newCommandBuffer.get();
    return vc::Error::Success;
}
}
