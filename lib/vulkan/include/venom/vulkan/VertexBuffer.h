///
/// Project: Bazel_Vulkan_Metal
/// @file VertexBuffer.h
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();


    vc::Error Init(const uint32_t vertexCount, const uint32_t vertexSize, const VkBufferUsageFlags flags,
        const VkSharingMode sharingMode, const VkMemoryPropertyFlags memoryProperties, const void *data);

private:
    VkBuffer __buffer;
    VkDeviceMemory __bufferMemory;
};
}
}