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
#include <venom/common/Timer.h>
#include <venom/common/Light.h>

#include <venom/common/plugin/graphics/Camera.h>
#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/plugin/graphics/Mesh.h>
#include <venom/common/plugin/graphics/Model.h>
#include <venom/common/plugin/graphics/RenderingPipeline.h>
#include <venom/common/plugin/graphics/ShaderPipeline.h>
#include <venom/common/plugin/graphics/Skybox.h>

#include <venom/common/ComponentManager.h>

#include <filesystem>
#include <thread>
#include <chrono>

#include <venom/common/plugin/graphics/GUI.h>


namespace venom
{
namespace common
{
static SceneCallback s_sceneCallback = nullptr;
static vc::Vector<LoopCallback> s_loopCallbacks;
static vc::Vector<InputCallback> s_inputCallbacks;

VenomEngine::VenomEngine()
    : pluginManager(new PluginManager())
    , __dllCache(new DLL_Cache())
    , __ecs(new ECS())
    , __sceneSettings(new SceneSettings())
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

    // Init Context
    s_instance->__context.reset(new Context());
    if (err = s_instance->__context->InitContext(); err != vc::Error::Success)
    {
        vc::Log::Error("Failed to initialize context: %d", err);
        return vc::Error::InitializationFailed;
    }
    vc::GraphicsApplication * app = vc::GraphicsApplication::Create();

    vc::GraphicsSettings::SetWindowResolution(s_instance->__context->GetWindowWidth(), s_instance->__context->GetWindowWidth());

    if (err = app->Init(); err != vc::Error::Success) {
        vc::Log::Error("Failed to init application: %d\n", static_cast<int>(err));
    } else {
        s_instance->__LoadECS();
        s_sceneCallback();
        vc::Timer::ResetLoopTimer();
        while (!app->ShouldClose())
        {
            vc::GUI::Get()->__PreUpdate();
            ECS::UpdateWorld();
            app->Loop();
            s_instance->pluginManager->CleanPluginsObjets();

            for (const auto& loopCallback : s_loopCallbacks) {
                loopCallback();
            }
            for (const auto& inputCallback : s_inputCallbacks) {
                inputCallback(Context::Get());
            }
            // Reset timer at the end
            vc::Timer::__PassFrame();
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

void VenomEngine::AddLoopCallback(const LoopCallback& loopCallback)
{
    s_loopCallbacks.emplace_back(loopCallback);
}

void VenomEngine::AddInputCallback(const InputCallback& inputCallback)
{
    s_inputCallbacks.emplace_back(inputCallback);
}

void VenomEngine::__LoadECS()
{
    // Reserve entities

    __ecs->RegisterComponent<ComponentManager>();
    __ecs->RegisterComponent<Transform3D>();
    __ecs->RegisterComponent<RenderingPipeline>();
    __ecs->RegisterComponent<Model>();
    __ecs->RegisterComponent<Skybox>();
    __ecs->RegisterComponent<Light>();

    // Need to clean ECS objects that were instantiated
    pluginManager->CleanPluginsObjets();
}
}
}
