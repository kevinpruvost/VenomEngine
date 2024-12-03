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
    : __valueType(MaterialComponentValueType::NONE)
    , __channels(MaterialComponentValueChannels::RGBA)
    , __texture(nullptr)
{
}

MaterialComponent::MaterialComponent(const MaterialComponent& other)
    : __valueType(other.__valueType)
    , __channels(other.__channels)
    , __value(other.__value)
    , __texture(other.__texture ? new Texture(*other.__texture) : nullptr)
{
}

MaterialComponent::MaterialComponent(MaterialComponent&& other) noexcept
    : __valueType(other.__valueType)
    , __channels(other.__channels)
    , __value(std::move(other.__value))
    , __texture(std::move(other.__texture))
{
}

MaterialComponent& MaterialComponent::operator=(const MaterialComponent& other)
{
    if (this != &other)
    {
        __valueType = other.__valueType;
        __channels = other.__channels;
        __value = other.__value;
        __texture.reset(other.__texture ? new Texture(*other.__texture) : nullptr);
    }
    return *this;
}

MaterialComponent& MaterialComponent::operator=(MaterialComponent&& other) noexcept
{
    if (this != &other)
    {
        __valueType = other.__valueType;
        __channels = other.__channels;
        __value = std::move(other.__value);
        __texture = std::move(other.__texture);
    }
    return *this;
}

MaterialComponent::~MaterialComponent()
{
}

}
}