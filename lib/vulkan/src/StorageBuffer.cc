///
/// Project: VenomEngineWorkspace
/// @file StorageBuffer.cc
/// @date Oct, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/StorageBuffer.h>

#include <venom/vulkan/QueueManager.h>
#include <venom/vulkan/LogicalDevice.h>

namespace venom
{
namespace vulkan
{
StorageBuffer::StorageBuffer()
    : __mappedData(nullptr)
{
}

StorageBuffer::~StorageBuffer()
{
}

StorageBuffer::StorageBuffer(StorageBuffer&& other)
    : __buffer(std::move(other.__buffer))
    , __mappedData(std::move(other.__mappedData))
{
}

StorageBuffer& StorageBuffer::operator=(StorageBuffer&& other)
{
    if (this != &other) {
        __buffer = std::move(other.__buffer);
        __mappedData = std::move(other.__mappedData);
    }
    return *this;
}

vc::Error StorageBuffer::Init(const VkDeviceSize size)
{
    vc::Error err = __buffer.CreateBuffer(size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
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

const VkDeviceMemory& StorageBuffer::GetVkDeviceMemory() const
{
    return __buffer.GetVkDeviceMemory();
}

void* StorageBuffer::GetMappedData() const
{
    return __mappedData;
}

VkBuffer StorageBuffer::GetVkBuffer() const
{
    return __buffer.GetVkBuffer();
}

VkDeviceSize StorageBuffer::GetSize() const
{
    return __buffer.GetSize();
}
}
}