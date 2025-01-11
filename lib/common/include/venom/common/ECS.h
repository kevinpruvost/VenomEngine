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
class GUI;

using Entity = flecs::entity;

typedef flecs::system System;

class VENOM_COMMON_API StaticVenomCanCreateComponent
{
public:
    StaticVenomCanCreateComponent(const vc::String & componentName, vc::Function<bool, Entity> canCreateFunc);
};

#define COMPONENT_CAN_ADD_DEFINITION(COMPONENT_NAME, BODY) \
    class COMPONENT_NAME##_CanAdd : public StaticVenomCanCreateComponent \
    { \
    public: \
        COMPONENT_NAME##_CanAdd() \
            : StaticVenomCanCreateComponent(#COMPONENT_NAME, [](Entity entity) { \
                BODY \
            }) \
        { \
        } \
    }; \
    static COMPONENT_NAME##_CanAdd s_##COMPONENT_NAME##_CanAdd;

class VENOM_COMMON_API VenomComponent
{
public:
    VenomComponent();
    virtual ~VenomComponent() = default;

    inline vc::String GetComponentTitle() { return _GetComponentTitle(); }
protected:
    virtual void _GUI(const Entity entity) = 0;
    virtual vc::String _GetComponentTitle() = 0;

    virtual void Init(Entity entity);
    virtual void Update(Entity entity) = 0;
    virtual bool CanRemove(Entity entity) = 0;

private:
    enum class GUIComponentAction
    {
        None,
        Remove
    };
    GUIComponentAction __GUI(const Entity entity);

private:
    void __Init(Entity entity);
    bool __initialized;

    friend class ECS;
    friend class GUI;
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
    void RegisterComponent(const vc::String & name) {
        __world.component<T>();
        vc::VenomComponent * component = new T();
        vc::String displayName = component->GetComponentTitle();
        __componentsCreateAndHasFuncs.emplace_back(MakeTuple<vc::String, vc::String, vc::Function<void, Entity>, vc::Function<bool, Entity>>(
            name,
            displayName,
            [](Entity entity) {
                entity.emplace<T>();
            },
            [](Entity entity) {
                return entity.has<T>();
            }
        ));
        delete component;
    }
#define REGISTER_COMPONENT(T) vc::ECS::GetECS()->RegisterComponent<T>(#T)

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

    static vc::String GenerateUniqueEntityName();
    static bool IsNameAvailable(const char * name);

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
    static const vc::UMap<vc::String, vc::Function<bool, Entity>> & __GetComponentCanCreateFuncs();

private:
    static Entity __currentEntity;
    flecs::world __world;
    vc::Vector<vc::Tuple<vc::String,
        vc::String,
        vc::Function<void, Entity>,
        vc::Function<bool, Entity>
    >> __componentsCreateAndHasFuncs;

    friend class GUI;
    friend class VenomComponent;
    friend class StaticVenomCanCreateComponent;
};

Entity CreatePrefab(const char* name);
Entity CreateEntity(const char * name);
Entity CreateEntity();

}
}
