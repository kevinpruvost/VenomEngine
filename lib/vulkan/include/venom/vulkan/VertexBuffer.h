///
/// Project: VenomEngine
/// @file VertexBuffer.h
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>
#include <venom/vulkan/Buffer.h>

namespace venom
{
namespace vulkan
{
class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&);
    VertexBuffer& operator=(VertexBuffer&&);

    vc::Error Init(const uint32_t vertexCount, const uint32_t vertexSize, const VkBufferUsageFlags flags,
        const VkSharingMode sharingMode, const VkMemoryPropertyFlags memoryProperties, const void *data);
    VkBuffer GetVkBuffer() const;
    static vc::Error CopyBuffer(const Buffer& srcBuffer, const Buffer& dstBuffer, const VkDeviceSize size);

private:
    Buffer __buffer;
};
}
}