///
/// Project: VenomEngine
/// @file UniformBuffer.cc
/// @date Sep, 15 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/UniformBuffer.h>

#include <venom/vulkan/QueueManager.h>
#include <venom/vulkan/LogicalDevice.h>

namespace venom
{
namespace vulkan
{
UniformBuffer::UniformBuffer()
    : __buffer()
    , __mappedData(nullptr)
{
}

UniformBuffer::~UniformBuffer()
{
}

UniformBuffer::UniformBuffer(UniformBuffer&& other)
    : __buffer(std::move(other.__buffer))
    , __mappedData(other.__mappedData)
{
}

UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other)
{
    if (this == &other) return *this;
    __buffer = std::move(other.__buffer);
    __mappedData = other.__mappedData;
    return *this;
}

vc::Error UniformBuffer::Init(const VkDeviceSize size)
{
    vc::Error err = __buffer.CreateBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        QueueManager::GetGraphicsTransferSharingMode(),
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to create uniform buffer");
        return err;
    }
    vkMapMemory(LogicalDevice::GetVkDevice(), __buffer.GetVkDeviceMemory(), 0, size, 0, &__mappedData);
    return err;
}

const VkDeviceMemory & UniformBuffer::GetVkDeviceMemory() const
{
    return __buffer.GetVkDeviceMemory();
}

void* UniformBuffer::GetMappedData() const
{
    return __mappedData;
}

VkBuffer UniformBuffer::GetVkBuffer() const
{
    return __buffer.GetVkBuffer();
}
}
}