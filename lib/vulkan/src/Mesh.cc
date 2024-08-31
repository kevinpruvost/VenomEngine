///
/// Project: VenomEngine
/// @file Mesh.cc
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#include <venom/vulkan/plugin/graphics/Mesh.h>

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

vc::Error VulkanMesh::AddVertexBuffer(const uint32_t vertexCount, const uint32_t vertexSize,
    const VkBufferUsageFlags flags, const VkSharingMode sharingMode, const VkMemoryPropertyFlags memoryProperties,
    const void* data)
{
    VertexBuffer & newVertexBuffer = __vertexBuffers.emplace_back();
    if (newVertexBuffer.Init(vertexCount, vertexSize, flags, sharingMode, memoryProperties, data) != vc::Error::Success)
        return vc::Error::Failure;
    __vkVertexBuffers.emplace_back(newVertexBuffer.GetVkBuffer());
    return vc::Error::Success;
}

const VkBuffer* VulkanMesh::GetVkVertexBuffers() const
{
    return __vkVertexBuffers.data();
}
}
}