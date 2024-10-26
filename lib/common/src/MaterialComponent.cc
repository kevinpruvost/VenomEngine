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
{
}

MaterialComponent::~MaterialComponent()
{
}

}
}