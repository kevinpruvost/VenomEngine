///
/// Project: VenomEngine
/// @file Mesh.cc
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#include <venom/vulkan/plugin/graphics/Mesh.h>

#include "venom/vulkan/QueueManager.h"

namespace venom
{
namespace vulkan
{
VulkanMesh::VulkanMesh()
{
}

VulkanMesh::~VulkanMesh()
{
}

void VulkanMesh::Draw()
{
    common::Log::Print("Mesh draw");
}

vc::Error VulkanMesh::AddVertexBuffer(const void* data, const uint32_t vertexCount, const uint32_t vertexSize)
{
    VertexBuffer & newVertexBuffer = __vertexBuffers.emplace_back();
    if (newVertexBuffer.Init(vertexCount, vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data) != vc::Error::Success)
        return vc::Error::Failure;
    __vkVertexBuffers.emplace_back(newVertexBuffer.GetVkBuffer());
    return vc::Error::Success;
}

vc::Error VulkanMesh::AddIndexBuffer(const void* data, const uint32_t indexCount, const uint32_t indexSize)
{
    if (__indexBuffer.Init(indexCount, indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

const VkBuffer* VulkanMesh::GetVkVertexBuffers() const
{
    return __vkVertexBuffers.data();
}

VkBuffer VulkanMesh::GetVkIndexBuffer() const
{
    return __indexBuffer.GetVkBuffer();
}

const IndexBuffer& VulkanMesh::GetIndexBuffer() const
{
    return __indexBuffer;
}

uint32_t VulkanMesh::GetBindingCount() const
{
    return static_cast<uint32_t>(__vertexBuffers.size());
}

uint32_t VulkanMesh::GetVertexCount() const
{
    venom_assert(__vertexBuffers.size() > 0, "No vertex buffer");
    return __vertexBuffers[0].GetVertexCount();
}
}
}
