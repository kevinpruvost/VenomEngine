///
/// Project: VenomEngine
/// @file Material.cc
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Material.h>

#include <venom/common/Log.h>

namespace venom
{
namespace common
{
Material::Material()
    : __components{
        MaterialComponentType::AMBIENT,
        MaterialComponentType::DIFFUSE,
        MaterialComponentType::SPECULAR,
        MaterialComponentType::EMISSIVE,
        MaterialComponentType::SHININESS,
        MaterialComponentType::OPACITY,
        MaterialComponentType::NORMAL,
        MaterialComponentType::HEIGHT,
        MaterialComponentType::REFLECTION,
        MaterialComponentType::REFLECTIVITY,
        MaterialComponentType::REFRACTION,
        MaterialComponentType::TRANSPARENT,
        MaterialComponentType::ANISOTROPY,
        MaterialComponentType::BASE_COLOR,
        MaterialComponentType::METALLIC,
        MaterialComponentType::ROUGHNESS,
        MaterialComponentType::AMBIENT_OCCLUSION,
        MaterialComponentType::EMISSION_COLOR,
        MaterialComponentType::TRANSMISSION,
        MaterialComponentType::SHEEN,
        MaterialComponentType::CLEARCOAT
    }
{
}

Material::~Material()
{
}

Material* Material::Create()
{
    return GraphicsPlugin::Get()->CreateMaterial();
}

void Material::SetComponent(const MaterialComponentType type, const vcm::Vec3& value)
{
    __components[type].SetValue(value);
}

void Material::SetComponent(const MaterialComponentType type, const vcm::Vec4& value)
{
    __components[type].SetValue(value);
}

void Material::SetComponent(const MaterialComponentType type, const float value)
{
    __components[type].SetValue(value);
}

void Material::SetComponent(const MaterialComponentType type, const Texture* texture)
{
    __components[type].SetValue(texture);
}

const MaterialComponent& Material::GetComponent(const MaterialComponentType type) const
{
    venom_assert(type < MaterialComponentType::MAX_COMPONENT, "MaterialComponentType out of range");
    return __components[type];
}

const std::string& Material::GetName() const
{
    return __name;
}

void Material::SetName(const std::string& name)
{
    __name = name;
}
}
}
