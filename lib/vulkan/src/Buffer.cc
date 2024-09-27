///
/// Project: VenomEngine
/// @file Buffer.cc
/// @date Aug, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Buffer.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

namespace venom
{
namespace vulkan
{
Buffer::Buffer()
    : __buffer(VK_NULL_HANDLE)
    , __memory(VK_NULL_HANDLE)
    , __bufferCreateInfo{}
{
}

Buffer::~Buffer()
{
    if (__buffer != VK_NULL_HANDLE)
        vkDestroyBuffer(LogicalDevice::GetVkDevice(), __buffer, Allocator::GetVKAllocationCallbacks());
    if (__memory != VK_NULL_HANDLE)
        vkFreeMemory(LogicalDevice::GetVkDevice(), __memory, Allocator::GetVKAllocationCallbacks());
}

Buffer::Buffer(Buffer&& other)
    : __buffer(other.__buffer)
    , __memory(other.__memory)
    , __bufferCreateInfo(other.__bufferCreateInfo)
{
    other.__buffer = VK_NULL_HANDLE;
    other.__memory = VK_NULL_HANDLE;
}

Buffer& Buffer::operator=(Buffer&& other)
{
    if (this != &other) {
        __buffer = other.__buffer;
        __memory = other.__memory;
        __bufferCreateInfo = other.__bufferCreateInfo;
        other.__buffer = VK_NULL_HANDLE;
        other.__memory = VK_NULL_HANDLE;
    }
    return *this;
}

uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(PhysicalDevice::GetUsedPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    return -1;
};

vc::Error Buffer::CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags flags, const VkSharingMode sharingMode,
                               const VkMemoryPropertyFlags memoryProperties)
{
    __bufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = flags,
        .sharingMode = sharingMode
    };

    if (vkCreateBuffer(LogicalDevice::GetVkDevice(), &__bufferCreateInfo, Allocator::GetVKAllocationCallbacks(), &__buffer) != VK_SUCCESS) {
        vc::Log::Error("Failed to create vertex buffer");
        return vc::Error::Failure;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(LogicalDevice::GetVkDevice(), __buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, memoryProperties);

    if (vkAllocateMemory(LogicalDevice::GetVkDevice(), &allocInfo, Allocator::GetVKAllocationCallbacks(), &__memory) != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate vertex buffer memory");
        return vc::Error::Failure;
    }

    if (auto err = __BindBufferMemory(); err != vc::Error::Success) {
        vc::Log::Error("Failed to bind buffer memory");
        return err;
    }
    return vc::Error::Success;
}

vc::Error Buffer::__BindBufferMemory()
{
    venom_assert(__buffer != VK_NULL_HANDLE && __memory != VK_NULL_HANDLE, "Buffer not created");
    if (vkBindBufferMemory(LogicalDevice::GetVkDevice(), __buffer, __memory, 0) != VK_SUCCESS) {
        vc::Log::Error("Failed to bind buffer memory");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error Buffer::WriteBuffer(const void* data)
{
    void* dataMap;
    if (auto vkErr = vkMapMemory(LogicalDevice::GetVkDevice(), __memory, 0, __bufferCreateInfo.size, 0, &dataMap); vkErr != VK_SUCCESS) {
        vc::Log::Error("Failed to map buffer memory");
        return vc::Error::Failure;
    }
    memcpy(dataMap, data, __bufferCreateInfo.size);
    vkUnmapMemory(LogicalDevice::GetVkDevice(), __memory);
    return vc::Error::Success;
}

VkBuffer Buffer::GetVkBuffer() const
{
    return __buffer;
}

const VkDeviceMemory & Buffer::GetVkDeviceMemory() const
{
    return __memory;
}

VkDeviceSize Buffer::GetSize() const
{
    return __bufferCreateInfo.size;
}
}
}