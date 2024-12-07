///
/// Project: VenomEngineWorkspace
/// @file ComponentManager.cc
/// @date Dec, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/ComponentManager.h>

namespace venom
{
namespace common
{
ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::_GUI()
{
    if (vc::GUI::Button("+ Add Component", vcm::Vec2(-1, 0)))
    {
        // TODO: Add component system
    }
}

vc::String ComponentManager::_GetComponentTitle()
{
    return "Components Manager";
}
}
}