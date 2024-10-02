///
/// Project: VenomEngine
/// @file Material.cc
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Material.h>

namespace venom
{
namespace common
{
Material::Material()
{
}

Material::~Material()
{
}

Material* Material::Create()
{
    return GraphicsPlugin::Get()->CreateMaterial();
}
}
}