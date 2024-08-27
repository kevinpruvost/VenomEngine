///
/// Project: Bazel_Vulkan_Metal
/// @file VertexBuffer.cc
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VertexBuffer.h>

#include <venom/vulkan/LogicalDevice.h>

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
        vkDestroyBuffer(LogicalDevice::GetVkDevice(), __buffer, nullptr);
}

vc::Error VertexBuffer::Init(const uint32_t vertexCount, const uint32_t vertexSize, const VkBufferUsageFlags flags,
    const VkSharingMode sharingMode, const VkMemoryPropertyFlags memoryProperties, const void* data)
{

    return vc::Error::Success;
}
}
}
