///
/// Project: VenomEngine
/// @file Mesh.cc
/// @date Oct, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Mesh.h>
#include <venom/common/Log.h>

namespace venom
{
namespace common
{
Mesh::Mesh()
    : PluginObjectWrapper(GraphicsPlugin::Get()->CreateMesh())
{
}

Mesh::~Mesh()
{
}

MeshImpl::MeshImpl()
    : _material()
{
}

void MeshImpl::SetMaterial(const Material & material)
{
    _material.emplace(material);
}

bool MeshImpl::HasMaterial() const
{
    return _material.has_value();
}

const Material & MeshImpl::GetMaterial() const
{
    venom_assert(HasMaterial(), "MeshImpl::GetMaterial() : _material does not have value, call HasMaterial() before to check if there is a material to get");
    return _material.value();
}
}
}
