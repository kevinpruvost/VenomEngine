///
/// Project: VenomEngine
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
{
}

VertexBuffer::~VertexBuffer()
{
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
    : __buffer(std::move(other.__buffer))
{
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer && other)
{
    if (this == &other) return *this;
    __buffer = std::move(__buffer);
    return *this;
}

vc::Error VertexBuffer::Init(const uint32_t vertexCount, const uint32_t vertexSize, const VkBufferUsageFlags flags,
                             const VkSharingMode sharingMode, const VkMemoryPropertyFlags memoryProperties, const void* data)
{
    Buffer stagingBuffer;
    stagingBuffer.CreateBuffer(vertexCount * vertexSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    stagingBuffer.WriteBuffer(data);

    __buffer.CreateBuffer(vertexCount * vertexSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | flags,
        sharingMode,
        memoryProperties
    );

    return vc::Error::Success;
}

VkBuffer VertexBuffer::GetVkBuffer() const
{
    return __buffer.GetVkBuffer();
}
}
}
