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

Mesh* Mesh::Create()
{
    return GraphicsPlugin::Get()->CreateMesh();
}

Mesh::~Mesh()
{
}
}
}