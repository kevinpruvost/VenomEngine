///
/// Project: Bazel_Vulkan_Metal
/// @file VertexBuffer.cc
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VertexBuffer.h>

#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

namespace venom
{
namespace vulkan
{
VertexBuffer::VertexBuffer()
    : __buffer(VK_NULL_HANDLE)
    , __bufferMemory(VK_NULL_HANDLE)
{
}

VertexBuffer::~VertexBuffer()
{
    if (__buffer != VK_NULL_HANDLE)
        vkDestroyBuffer(LogicalDevice::GetVkDevice(), __buffer, Allocator::GetVKAllocationCallbacks());
    if (__bufferMemory != VK_NULL_HANDLE)
        vkFreeMemory(LogicalDevice::GetVkDevice(), __bufferMemory, Allocator::GetVKAllocationCallbacks());
}

vc::Error VertexBuffer::Init(const uint32_t vertexCount, const uint32_t vertexSize, const VkBufferUsageFlags flags,
    const VkSharingMode sharingMode, const VkMemoryPropertyFlags memoryProperties, const void* data)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = vertexCount * vertexSize;
    bufferInfo.usage = flags;
    bufferInfo.sharingMode = sharingMode;

    if (vkCreateBuffer(LogicalDevice::GetVkDevice(), &bufferInfo, Allocator::GetVKAllocationCallbacks(), &__buffer) != VK_SUCCESS) {
        vc::Log::Error("Failed to create vertex buffer");
        return vc::Error::Failure;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(LogicalDevice::GetVkDevice(), __buffer, &memRequirements);

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(PhysicalDevice::GetUsedPhysicalDevice(), &memProperties);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    const auto findMemoryType = [&](uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t {
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        return -1;
    };
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(LogicalDevice::GetVkDevice(), &allocInfo, Allocator::GetVKAllocationCallbacks(), &__bufferMemory) != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate vertex buffer memory");
        return vc::Error::Failure;
    }

    vkBindBufferMemory(LogicalDevice::GetVkDevice(), __buffer, __bufferMemory, 0);

    void* dataMap;
    vkMapMemory(LogicalDevice::GetVkDevice(), __bufferMemory, 0, bufferInfo.size, 0, &dataMap);
    memcpy(dataMap, data, bufferInfo.size);
    vkUnmapMemory(LogicalDevice::GetVkDevice(), __bufferMemory);
    return vc::Error::Success;
}
}
}
