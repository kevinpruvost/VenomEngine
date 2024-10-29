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

    vc::Error Init(const uint32_t vertexCount, const uint32_t vertexSize, const VkBufferUsageFlags flags, const void *data);
    VkBuffer GetVkBuffer() const;
    inline const VkBuffer * GetVkBufferPtr() const { return __buffer.GetVkBufferPtr(); }
    static vc::Error CopyBuffer(const Buffer& srcBuffer, const Buffer& dstBuffer, const VkDeviceSize size);
    uint32_t GetVertexCount() const;
    uint32_t GetVertexSize() const;
    uint32_t GetTotalSize() const;

private:
    Buffer __buffer;
    uint32_t __vertexCount;
    uint32_t __vertexSize;
};
typedef VertexBuffer IndexBuffer;
}
}