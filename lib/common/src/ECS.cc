///
/// Project: VenomEngineWorkspace
/// @file ECS.cc
/// @date Oct, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/ECS.h>

#include <venom/common/Log.h>

#include "venom/common/Transform3D.h"

namespace venom
{
namespace common
{
ECS * ECS::s_ecs = nullptr;

static vc::UMap<vc::String, vc::Function<bool, Entity>> s_componentsCanCreateFuncs;

StaticVenomCanCreateComponent::StaticVenomCanCreateComponent(const vc::String& componentName,
    vc::Function<bool, Entity> canCreateFunc)
{
    s_componentsCanCreateFuncs[componentName] = canCreateFunc;
}

VenomComponent::VenomComponent()
    : __initialized(false)
{
}

vc::VenomComponent::GUIComponentAction VenomComponent::__GUI(const Entity entity)
{
    const vc::String title = _GetComponentTitle();
    vc::VenomComponent::GUIComponentAction action = GUIComponentAction::None;
    if (vc::GUI::CollapsingHeader(title.c_str(), GUITreeNodeFlagsBits::GUITreeNodeFlags_DefaultOpen)) {
        if (vc::GUI::BeginPopupContextItem(nullptr, GUIPopupFlagsBits::GUIPopupFlags_MouseButtonRight)) {
            if (CanRemove(entity)) {
                if (vc::GUI::MenuItem(ICON_MS_DELETE " Delete Component")) {
                    action = GUIComponentAction::Remove;
                }
            } else {
                vc::GUI::Text(ICON_MS_WARNING " Unable to remove this component!");
            }
            vc::GUI::EndPopup();
        }
        _GUI(entity);
    }
    return action;
}

void VenomComponent::Init(Entity entity)
{
}

void VenomComponent::__Init(Entity entity)
{
    if (!__initialized) {
        Init(entity);
        __initialized = true;
    }
}

ECS::ECS()
    : __world()
{
    venom_assert(s_ecs == nullptr, "ECS already exists");
    s_ecs = this;
}

ECS::~ECS()
{
    s_ecs = nullptr;
}

Entity ECS::CreateEntity()
{
    vc::String name = GenerateUniqueEntityName();
    return __world.entity(name.c_str()).emplace<Transform3D>();
}

Entity ECS::CreateEntity(const char* name)
{
    return __world.entity(name).emplace<Transform3D>();
}

Entity ECS::CreatePrefab(const char* name)
{
    return __world.prefab(name).emplace<Transform3D>();
}

vc::String ECS::GenerateUniqueEntityName()
{
    vc::String ret = "Entity" + std::to_string(rand());
    while ((s_ecs->__world.lookup(ret.c_str())).is_valid()) {
        ret = "Entity" + std::to_string(rand());
    }
    return ret;
}

bool ECS::IsNameAvailable(const char* name)
{
    return !s_ecs->__world.lookup(name).is_valid();
}

ECS* ECS::GetECS()
{
    return s_ecs;
}

void ECS::UpdateWorld()
{
    ECS::ForEach<Transform3D>([&](Entity entity, Transform3D & cm) {
        entity.each([&](flecs::id componentID) {
            Component * component = reinterpret_cast<Component *>(entity.get_mut(componentID));
            component->__Init(entity);
        });
    });
    // In case Init removes a component
    ECS::ForEach<Transform3D>([&](Entity entity, Transform3D & cm) {
    entity.each([&](flecs::id componentID) {
        Component * component = reinterpret_cast<Component *>(entity.get_mut(componentID));
        component->Update(entity);
    });
});
}

const vc::UMap<vc::String, vc::Function<bool, Entity>>& ECS::__GetComponentCanCreateFuncs()
{
    return s_componentsCanCreateFuncs;
}

Entity CreatePrefab(const char* name)
{
    return ECS::GetECS()->CreatePrefab(name);
}

Entity CreateEntity(const char* name)
{
    return ECS::GetECS()->CreateEntity(name);
}

Entity CreateEntity()
{
    return ECS::GetECS()->CreateEntity();
}
}
}
