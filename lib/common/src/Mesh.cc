///
/// Project: Bazel_Vulkan_Metal
/// @file Mesh.cc
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Mesh.h>

#include "venom/common/VenomEngine.h"

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
