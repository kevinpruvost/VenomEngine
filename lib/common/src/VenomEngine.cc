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
#include <venom/common/plugin/graphics/Light.h>

#include <venom/common/plugin/graphics/Camera.h>
#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/plugin/graphics/Mesh.h>
#include <venom/common/plugin/graphics/Model.h>
#include <venom/common/plugin/graphics/RenderingPipeline.h>
#include <venom/common/plugin/graphics/ShaderPipeline.h>
#include <venom/common/plugin/graphics/Skybox.h>

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
    , __deferredTrash(new DeferredTrashBin())
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
    __deferredTrash.reset();
    pluginManager.reset();
    __dllCache.reset();
}

static std::unique_ptr<VenomEngine> s_instance;
VenomEngine* VenomEngine::GetInstance()
{
    return s_instance.get();
}

Error VenomEngine::RunEngine(int argc, const char* argv[])
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
    vc::GraphicsApplication * graphicsApp = vc::GraphicsApplication::Create(argc, argv);

    vc::GraphicsSettings::SetWindowResolution(s_instance->__context->GetWindowWidth(), s_instance->__context->GetWindowWidth());

    if (err = graphicsApp->Init(); err != vc::Error::Success) {
        vc::Log::Error("Failed to init application: %d\n", static_cast<int>(err));
    } else {
        s_instance->__LoadECS();
        s_sceneCallback(vc::ScenePhase::Initialization);
        ECS::UpdateWorld();
        s_sceneCallback(vc::ScenePhase::Activation);
        vc::Timer::ResetLoopTimer();
        while (!graphicsApp->ShouldClose())
        {
            vc::GUI::Get()->__PreUpdate();
            ECS::UpdateWorld();
            s_sceneCallback(vc::ScenePhase::Update);
            s_instance->__deferredTrash->EmptyDeferredTrash();
            graphicsApp->Loop();
            s_instance->pluginManager->CleanPluginsObjets();

            // Reset timer at the end
            vc::Timer::__PassFrame();
            for (const auto& loopCallback : s_loopCallbacks) {
                loopCallback();
            }
            for (const auto& inputCallback : s_inputCallbacks) {
                inputCallback(Context::Get());
            }

            // Waits for draws to finish then calls the callbacks
            if (graphicsApp->HasCallbacksAfterDraws()) {
                graphicsApp->WaitForDraws();
                graphicsApp->LaunchCallbacksAfterDraws();
            }
        }
    }
    s_sceneCallback(vc::ScenePhase::Destruction);
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

    REGISTER_COMPONENT(Transform3D);
    REGISTER_COMPONENT(Model);
    REGISTER_COMPONENT(Skybox);
    REGISTER_COMPONENT(Light);

    // Need to clean ECS objects that were instantiated
    pluginManager->CleanPluginsObjets();
}
}
}
