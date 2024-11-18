///
/// Project: VenomEngineWorkspace
/// @file ECS.h
/// @date Oct, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/DLL.h>
#include <flecs.h>
#include <functional>

namespace venom
{
namespace common
{
typedef flecs::entity Entity;

template <typename T>
using Component = flecs::component<T>;

typedef flecs::system System;

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

    template<typename T>
    inline Component<T> GetComponent() {
        return __world.component<T>();
    }

    Entity CreateEntity();
    Entity CreateEntity(const char * name);
    static inline Entity GetEntity(const char * name) {
        return ECS::GetECS()->__GetEntity(name);
    }

    template <typename... Comps, typename... Args>
    System CreateSystem(const char * name) {
        return __world.system<Comps...>(name);
    }

    template <typename... Args>
    void ForEach(auto && func) {
        flecs::query<Args...> q = __world.query<Args...>();
        q.each(func);
    }

    static ECS * GetECS();

private:
    static ECS * s_ecs;

    Entity __GetEntity(const char * name) {
        return __world.lookup(name);
    }

private:
    flecs::world __world;
};

Entity CreateEntity(const char * name);
Entity CreateEntity();

template<typename T>
inline Component<T> GetComponent() {
    return ECS::GetECS()->GetComponent<T>();
}

}
}
