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

VenomComponent::VenomComponent()
    : __initialized(false)
{
}

void VenomComponent::GUI(const Entity entity)
{
    const vc::String title = _GetComponentTitle();
    if (vc::GUI::CollapsingHeader(title.c_str(), GUITreeNodeFlagsBits::GUITreeNodeFlags_DefaultOpen)) {
        _GUI(entity);
    }
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
    return __world.entity().emplace<Transform3D>();
}

Entity ECS::CreateEntity(const char* name)
{
    return __world.entity(name).emplace<Transform3D>();
}

Entity ECS::CreatePrefab(const char* name)
{
    return __world.prefab(name).emplace<Transform3D>();
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
