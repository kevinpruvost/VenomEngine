///
/// Project: VenomEngine
/// @file VenomEngine.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/PluginManager.h>
#include <venom/common/ECS.h>
#include <venom/common/VenomSettings.h>

#include <venom/common/Context.h>
#include <venom/common/plugin/graphics/Light.h>
#include <venom/common/SceneSettings.h>

#include <venom/common/DeferredTrash.h>

namespace venom
{
namespace common
{
enum class ScenePhase {
    Initialization,  // First phase: Only entity creation and adding components
    Activation,      // Second phase: Entity and component methods can be used
    Update,          // Third phase: Update during loops
    Destruction      // Third phase: Cleanup and removal of entities/components
};
// Scenes are callbacks that will be called by the engine to load a scene
// Can either be a file or a function
typedef void (*SceneCallback)(const ScenePhase);
typedef void (*LoopCallback)();
typedef void (*InputCallback)(Context *);

/**
 * @brief Main class of the engine
 * This class will be the main entry point of the engine.
 * It will contain all the main components of the engine, as singleton are harder to manage,
 * especially for unit tests and memory management.
 */
class VENOM_COMMON_API VenomEngine
{
private:
    VenomEngine();
public:
    ~VenomEngine();

    /**
     * @brief Gets instance of VenomEngine
     * @return VenomEngine *
     */
    static VenomEngine * GetInstance();

    /**
     * @brief Runs the engine
     * @param argc
     * @param argv
     * @return error if any, refer to Error enum for more information
     */
    static Error RunEngine(int argc, const char* argv[]);

    /**
     * @brief Loads a scene from a file
     * @param scenefileName
     * @return error
     */
    static Error SetScene(const String & scenefileName);

    /**
     * @brief Loads a scene from a callback
     * @param sceneCallback
     * @return error
     */
    static Error SetScene(const SceneCallback & sceneCallback);

    /**
     * @brief Adds a loop callback
     * @param loopCallback
     */
    static void AddLoopCallback(const LoopCallback & loopCallback);

    /**
     * @brief Adds a loop callback
     * @param inputCallback
     */
    static void AddInputCallback(const InputCallback & inputCallback);


private:
    void __LoadECS();

public:
    UPtr<PluginManager> pluginManager;

private:
    UPtr<Context> __context;
    UPtr<DLL_Cache> __dllCache;
    UPtr<ECS> __ecs;
    UPtr<VenomSettings> __settings;
    UPtr<SceneSettings> __sceneSettings;
    UPtr<DeferredTrashBin> __deferredTrash;
};
}
}
