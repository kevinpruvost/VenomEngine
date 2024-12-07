///
/// Project: VenomEngineWorkspace
/// @file ECS.cc
/// @date Oct, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/ECS.h>

#include <venom/common/Log.h>
#include <venom/common/ComponentManager.h>

namespace venom
{
namespace common
{
ECS * ECS::s_ecs = nullptr;

void VenomComponent::GUI()
{
    const vc::String title = _GetComponentTitle();
    if (vc::GUI::CollapsingHeader(title.c_str(), GUITreeNodeFlagsBits::GUITreeNodeFlags_DefaultOpen)) {
        _GUI();
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
    return __world.entity();
}

Entity ECS::CreateEntity(const char* name)
{
    return __world.entity(name).emplace<ComponentManager>();
}

Entity ECS::CreatePrefab(const char* name)
{
    return __world.prefab(name).emplace<ComponentManager>();
}

ECS* ECS::GetECS()
{
    return s_ecs;
}

Component* ECS::__GetComponentFromID(flecs::id id)
{

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
