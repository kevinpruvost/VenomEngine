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

vc::Error VulkanMesh::__LoadMeshFromCurrentData()
{
    // Positions
    if (!_positions.empty() && AddVertexBuffer(_positions.data(), static_cast<uint32_t>(_positions.size()), sizeof(vcm::VertexPos), 0) != vc::Error::Success)
        return vc::Error::Failure;
    // Normals
    if (!_normals.empty() && AddVertexBuffer(_normals.data(), static_cast<uint32_t>(_normals.size()), sizeof(vcm::VertexNormal), 1) != vc::Error::Success)
        return vc::Error::Failure;
    // UVs
    if (!_uvs[0].empty() && AddVertexBuffer(_uvs[0].data(), static_cast<uint32_t>(_uvs[0].size()), sizeof(vcm::VertexUV), 2) != vc::Error::Success)
        return vc::Error::Failure;
    // Tangents
    if (!_tangents.empty() && AddVertexBuffer(_tangents.data(), static_cast<uint32_t>(_tangents.size()), sizeof(vcm::VertexTangent), 3) != vc::Error::Success)
        return vc::Error::Failure;
    // Bitangents
    if (!_bitangents.empty() && AddVertexBuffer(_bitangents.data(), static_cast<uint32_t>(_bitangents.size()), sizeof(vcm::VertexBitangent), 4) != vc::Error::Success)
        return vc::Error::Failure;
    // Indices
    if (!_indices.empty() && AddIndexBuffer(_indices.data(), static_cast<uint32_t>(_indices.size()), sizeof(uint32_t)) != vc::Error::Success)
        return vc::Error::Failure;

    // Materials
    if (_material) {
        const auto & material = _material->GetComponent(vc::MaterialComponentType::DIFFUSE);
        if (material.GetValueType() == vc::MaterialComponentValueType::TEXTURE) {
            const auto & texture = _material->GetComponent(vc::MaterialComponentType::DIFFUSE).GetTexture();

        }
    }
    return vc::Error::Success;
}

vc::Error VulkanMesh::AddVertexBuffer(const void* data, const uint32_t vertexCount, const uint32_t vertexSize, int binding)
{
    VertexBuffer & newVertexBuffer = __vertexBuffers.emplace_back();
    if (newVertexBuffer.Init(vertexCount, vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data) != vc::Error::Success)
        return vc::Error::Failure;
    __vkVertexBuffers.emplace_back(binding, newVertexBuffer.GetVkBuffer());
    __offsets.emplace_back(0);
    return vc::Error::Success;
}

vc::Error VulkanMesh::AddIndexBuffer(const void* data, const uint32_t indexCount, const uint32_t indexSize)
{
    if (__indexBuffer.Init(indexCount, indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

const std::vector<VulkanMesh::BoundVkBuffer> & VulkanMesh::GetVkVertexBuffers() const
{
    return __vkVertexBuffers;
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

VkDeviceSize* VulkanMesh::GetOffsets() const
{
    return const_cast<VkDeviceSize*>(__offsets.data());
}
}
}
