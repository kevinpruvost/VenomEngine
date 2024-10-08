///
/// Project: VenomEngine
/// @file MaterialComponent.cc
/// @date Oct, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/MaterialComponent.h>

namespace venom
{
namespace common
{
MaterialComponent::MaterialComponent(const MaterialComponentType type)
    : __type(type)
    , __valueType(MaterialComponentValueType::NONE)
{
}

void MaterialComponent::SetValue(const vcm::Vec3& value)
{
    __valueType = MaterialComponentValueType::COLOR3D;
    __color3D = value;
}

void MaterialComponent::SetValue(const vcm::Vec4& value)
{
    __valueType = MaterialComponentValueType::COLOR4D;
    __color4D = value;
}

void MaterialComponent::SetValue(const float value)
{
    __valueType = MaterialComponentValueType::VALUE;
    __value = value;
}

void MaterialComponent::SetValue(const Texture* texture)
{
    __valueType = MaterialComponentValueType::TEXTURE;
    __texture = texture;
}

const Texture* MaterialComponent::GetTexture() const
{
    return __texture;
}
}
}