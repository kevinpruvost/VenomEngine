///
/// Project: VenomEngineWorkspace
/// @file ComponentManager.cc
/// @date Dec, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/ComponentManager.h>

#include <venom/common/ECS.h>

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

void ComponentManager::Update(Entity entity)
{
}

void ComponentManager::_GUI(const Entity entity)
{
    if (vc::GUI::Button("+ Add Component", vcm::Vec2(-1, 0)))
    {
        // Show all components
        vc::GUI::OpenPopup("add_component_popup");
    }
    if (vc::GUI::BeginPopup("add_component_popup"))
    {
        vc::GUI::SeparatorText("Components");

        const auto & componentCreateFuncs = ECS::s_ecs->__componentsCreateAndHasFuncs;
        for (const auto & [name, funcCreate, funcHas] : componentCreateFuncs)
        {
            if (funcHas(entity))
                continue;
            if (vc::GUI::Button(name.c_str(), vcm::Vec2(-1, 0)))
            {
                funcCreate(entity);
                vc::GUI::CloseCurrentPopup();
            }
        }

        vc::GUI::EndPopup();
    }
}

vc::String ComponentManager::_GetComponentTitle()
{
    return "Components Manager";
}
}
}