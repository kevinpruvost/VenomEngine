///
/// Project: VenomEngineWorkspace
/// @file ECS.h
/// @date Oct, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/DLL.h>
#include <venom/common/plugin/graphics/GUI.h>
#include <flecs.h>
#include <functional>

#include <venom/common/Containers.h>

namespace venom
{
namespace common
{
typedef flecs::entity Entity;

typedef flecs::system System;

class VenomComponent
{
public:
    VenomComponent() = default;
    virtual ~VenomComponent() = default;

    void GUI();
protected:
    virtual void _GUI() = 0;
    virtual vc::String _GetComponentTitle() = 0;
};

typedef VenomComponent Component;

/**
 * @brief Entity Component System
 * Will mainly encapsulate the fabulous flecs library (https://github.com/SanderMertens/flecs.git)
 * This class will be contained in the VenomEngine class.
 */
class VENOM_COMMON_API ECS
{
public:
    ECS();
    ~ECS();

    template <typename T>
    void RegisterComponent() {
        __world.component<T>();
    }

    Entity CreateEntity();
    Entity CreateEntity(const char * name);
    Entity CreatePrefab(const char * name);
    static inline Entity GetEntity(const char * name) {
        return ECS::GetECS()->__GetEntity(name);
    }

    template <typename... Comps, typename... Args>
    System CreateSystem(const char * name) {
        return __world.system<Comps...>(name);
    }

    template <typename... Args>
    static inline void ForEach(auto && func) {
        ECS::GetECS()->__ForEach<Args...>(func);
    }

    static inline void Each(auto && func) {
        ECS::GetECS()->__Each(func);
    }

    static inline Component * GetComponentFromID(flecs::id id) {
        return ECS::GetECS()->__GetComponentFromID(id);
    }

    static ECS * GetECS();

private:
    template <typename... Args>
    void __ForEach(auto && func) {
        flecs::query<Args...> q = __world.query<Args...>();
        q.each(func);
    }

    void __Each(auto && func) {
        auto query = __world.query_builder<>().build();
        query.each(func);
    }

    static ECS * s_ecs;

    Entity __GetEntity(const char * name) {
        return __world.lookup(name);
    }

    Component * __GetComponentFromID(flecs::id id);

private:
    flecs::world __world;
};

Entity CreatePrefab(const char* name);
Entity CreateEntity(const char * name);
Entity CreateEntity();

}
}
