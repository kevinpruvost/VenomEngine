///
/// Project: VenomEngine
/// @file Mesh.cc
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#include <venom/metal/plugin/graphics/Mesh.h>

namespace venom
{
namespace metal
{
MetalMesh::MetalMesh()
{
}

MetalMesh::~MetalMesh()
{
}

void MetalMesh::Draw()
{
    common::Log::Print("Mesh draw");
}

vc::Error MetalMesh::__LoadMeshFromCurrentData()
{
//    // Positions
//    if (!_positions.empty() && AddVertexBuffer(_positions.data(), static_cast<uint32_t>(_positions.size()), sizeof(vcm::VertexPos), 0) != vc::Error::Success)
//        return vc::Error::Failure;
//    // Normals
//    if (!_normals.empty() && AddVertexBuffer(_normals.data(), static_cast<uint32_t>(_normals.size()), sizeof(vcm::VertexNormal), 1) != vc::Error::Success)
//        return vc::Error::Failure;
//    // UVs
//    if (!_uvs[0].empty() && AddVertexBuffer(_uvs[0].data(), static_cast<uint32_t>(_uvs[0].size()), sizeof(vcm::VertexUV), 2) != vc::Error::Success)
//        return vc::Error::Failure;
//    // Tangents
//    if (!_tangents.empty() && AddVertexBuffer(_tangents.data(), static_cast<uint32_t>(_tangents.size()), sizeof(vcm::VertexTangent), 3) != vc::Error::Success)
//        return vc::Error::Failure;
//    // Bitangents
//    if (!_bitangents.empty() && AddVertexBuffer(_bitangents.data(), static_cast<uint32_t>(_bitangents.size()), sizeof(vcm::VertexBitangent), 4) != vc::Error::Success)
//        return vc::Error::Failure;
//    // Indices
//    if (!_indices.empty() && AddIndexBuffer(_indices.data(), static_cast<uint32_t>(_indices.size()), sizeof(uint32_t)) != vc::Error::Success)
//        return vc::Error::Failure;

//    // Materials
//    if (_material) {
//        const auto & material = _material->GetComponent(vc::MaterialComponentType::DIFFUSE);
//        if (material.GetValueType() & vc::MaterialComponentValueType::TEXTURE) {
//            const auto * texture = _material->GetComponent(vc::MaterialComponentType::DIFFUSE).GetTexture();
//        }
//    }
    return vc::Error::Success;
}

}
}
