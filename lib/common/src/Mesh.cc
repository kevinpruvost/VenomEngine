///
/// Project: VenomEngine
/// @file Mesh.cc
/// @date Oct, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Mesh.h>

namespace venom
{
namespace common
{
Mesh::Mesh()
    : GraphicsPluginObject()
{
}

Mesh * Mesh::Create()
{
    Mesh * mesh = new Mesh();
    mesh->_impl = GraphicsPlugin::Get()->CreateMesh();
    return mesh;
}

Mesh::~Mesh()
{
}

MeshImpl::MeshImpl()
    : _material(nullptr)
{
}

void MeshImpl::SetMaterial(Material* material)
{
    _material = material;
}

const Material* MeshImpl::GetMaterial() const
{
    return _material;
}
}
}