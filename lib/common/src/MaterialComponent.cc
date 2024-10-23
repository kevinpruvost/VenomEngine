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

MaterialComponent::~MaterialComponent()
{
}

void MaterialComponent::SetValue(const vcm::Vec3& value)
{
    __valueType = MaterialComponentValueType::COLOR3D;
    __value.emplace<vcm::Vec3>(value);
}

void MaterialComponent::SetValue(const vcm::Vec4& value)
{
    __valueType = MaterialComponentValueType::COLOR4D;
    __value.emplace<vcm::Vec4>(value);
}

void MaterialComponent::SetValue(const float value)
{
    __valueType = MaterialComponentValueType::VALUE;
    __value.emplace<float>(value);
}

void MaterialComponent::SetValue(const Texture & texture)
{
    __value.emplace<Texture>(texture);
    __valueType = MaterialComponentValueType::TEXTURE;
}

const Texture & MaterialComponent::GetTexture() const
{
    return std::get<Texture>(__value);
}
}
}