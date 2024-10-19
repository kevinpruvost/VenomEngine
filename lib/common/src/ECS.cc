///
/// Project: VenomEngineWorkspace
/// @file ECS.cc
/// @date Oct, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <../include/venom/common/ECS.h>

#include <../include/venom/common/Log.h>

namespace venom
{
namespace common
{
ECS * ECS::s_ecs = nullptr;

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

Entity ECS::CreateEntity(const char* name)
{
    return __world.entity(name);
}

ECS* ECS::GetECS()
{
    return s_ecs;
}

Entity CreateEntity(const char* name)
{
    return ECS::GetECS()->CreateEntity(name);
}
}
}
