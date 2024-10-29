///
/// Project: VenomEngine
/// @file VenomEngine.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <memory>
#include <venom/common/VenomEngine.h>
#include <venom/common/Config.h>
#include <venom/common/Log.h>
#include <venom/common/MemoryPool.h>
#include <venom/common/Resources.h>
#include <venom/common/plugin/graphics/GraphicsApplication.h>

#include <venom/common/plugin/graphics/Mesh.h>
#include <venom/common/plugin/graphics/Model.h>
#include <venom/common/plugin/graphics/Shader.h>
#include <venom/common/plugin/graphics/Skybox.h>

#include <filesystem>
#include <thread>
#include <chrono>

namespace venom
{
namespace common
{
static SceneCallback s_sceneCallback = nullptr;

VenomEngine::VenomEngine()
    : pluginManager(new PluginManager())
    , __dllCache(new DLL_Cache())
    , __ecs(new ECS())
{
    venom_assert(__dllCache, "VenomEngine::VenomEngine() : __dllCache is nullptr");
    DLL_Cache::SetCache(__dllCache.get());
    Error err;
    vc::Log::Print("Current working directory: %s", std::filesystem::current_path().string().c_str());
    if (err = pluginManager->LoadAllPlugins(); err != Error::Success) {
        Log::Error("VenomEngine::VenomEngine() : Failed to load all plugins");
        abort();
    }
    if (err = MemoryPool::CreateMemoryPool(); err != Error::Success) {
        Log::Error("VenomEngine::VenomEngine() : Failed to create memory pool");
        abort();
    }
    __LoadECS();
}

VenomEngine::~VenomEngine()
{
    // Order of destruction is important
    __ecs.reset();
    pluginManager.reset();
    __dllCache.reset();
}

static std::unique_ptr<VenomEngine> s_instance;
VenomEngine* VenomEngine::GetInstance()
{
    return s_instance.get();
}

Error VenomEngine::RunEngine(int argc, char** argv)
{
    vc::Error err = Error::Success;

    // Check if scene is set
    if (!s_sceneCallback) {
        Log::Error("VenomEngine::RunEngine() : No scene set");
        return Error::Failure;
    }

    vc::Resources::InitializeFilesystem(argc, argv);

    s_instance.reset(new VenomEngine());
    vc::GraphicsApplication * app = vc::GraphicsApplication::Create();


    if (err = app->Init(); err != vc::Error::Success) {
        vc::Log::Error("Failed to init application: %d\n", static_cast<int>(err));
    } else {
        auto test = app->GetAvailableMultisamplingOptions();
        s_sceneCallback();
        while (!app->ShouldClose())
        {
            app->Loop();
            s_instance->pluginManager->CleanPluginsObjets();
        }
    }
    s_instance.reset();
    vc::Resources::FreeFilesystem();
    return err;
}

Error VenomEngine::SetScene(const String& scenefileName)
{
    // TODO:
    return vc::Error::Failure;
}

Error VenomEngine::SetScene(const SceneCallback& sceneCallback)
{
    s_sceneCallback = sceneCallback;
    return vc::Error::Success;
}

void VenomEngine::__LoadECS()
{
    // Reserve entities

    __ecs->RegisterComponent<Shader>();
    __ecs->RegisterComponent<Model>();
    __ecs->RegisterComponent<Skybox>();
}
}
}
