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
#include <venom/common/Functional.h>

#include <venom/common/Containers.h>

namespace venom
{
namespace common
{
class ECS;

using Entity = flecs::entity;

typedef flecs::system System;

enum GUIComponentAction
{
    GUIComponentAction_None,
    GUIComponentAction_Add,
    GUIComponentAction_Remove
};
class VENOM_COMMON_API VenomComponent
{
public:
    VenomComponent();
    virtual ~VenomComponent() = default;

    void GUI(const Entity entity);
    inline vc::String GetComponentTitle() { return _GetComponentTitle(); }
protected:
    virtual void _GUI(const Entity entity) = 0;
    virtual vc::String _GetComponentTitle() = 0;

    virtual void Init(Entity entity);
    virtual void Update(Entity entity) = 0;

private:
    void __Init(Entity entity);
    bool __initialized;

    friend class ECS;
};

typedef VenomComponent Component;

template<typename T>
concept InheritsFromComponent = std::derived_from<T, vc::VenomComponent>;

/**
 * @brief Entity Component System
 * Will mainly encapsulate the fabulous flecs library (https://github.com/SanderMertens/flecs.git)
 * This class will be contained in the VenomEngine class.
 * Also manages/lists all components types for browsing.
 */
class VENOM_COMMON_API ECS
{
public:
    ECS();
    ~ECS();

    template <InheritsFromComponent T>
    void RegisterComponent() {
        __world.component<T>();
        vc::VenomComponent * component = new T();
        vc::String name = component->GetComponentTitle();
        __componentsCreateAndHasFuncs.emplace_back(MakeTuple<vc::String, vc::Function<void, Entity>, vc::Function<bool, Entity>>(
            name,
            [](Entity entity) {
                entity.emplace<T>();
            },
            [](Entity entity) {
                return entity.has<T>();
            }
        ));
        delete component;
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

    static ECS * GetECS();

    static void UpdateWorld();

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

private:
    static Entity __currentEntity;
    flecs::world __world;
    vc::Vector<vc::Tuple<vc::String,
        vc::Function<void, Entity>,
        vc::Function<bool, Entity>
    >> __componentsCreateAndHasFuncs;

    friend class GUI;
    friend class VenomComponent;
};

Entity CreatePrefab(const char* name);
Entity CreateEntity(const char * name);
Entity CreateEntity();

}
}
