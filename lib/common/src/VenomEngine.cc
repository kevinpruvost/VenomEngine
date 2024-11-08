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

#include <venom/common/plugin/graphics/Camera.h>
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
static vc::Vector<LoopCallback> s_loopCallbacks;

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

    // Init Context
    s_instance->__context.reset(new Context());
    if (err = s_instance->__context->InitContext(); err != vc::Error::Success)
    {
        vc::Log::Error("Failed to initialize context: %d", err);
        return vc::Error::InitializationFailed;
    }

    vc::GraphicsApplication * app = vc::GraphicsApplication::Create();

    if (err = app->Init(); err != vc::Error::Success) {
        vc::Log::Error("Failed to init application: %d\n", static_cast<int>(err));
    } else {
        auto test = app->GetAvailableMultisamplingOptions();
        s_sceneCallback();
        vc::Timer::ResetLoopTimer();
        while (!app->ShouldClose())
        {
            app->Loop();
            s_instance->pluginManager->CleanPluginsObjets();

            // Camera controls
            const float speed = 5.0f;
            if (Context::Get()->IsKeyPressed(vc::KeyboardInput::KeyboardW))
            {
                vc::Camera::GetMainCamera()->MoveForward(speed * vc::Timer::GetLambdaSeconds());
            }
            if (Context::Get()->IsKeyPressed(vc::KeyboardInput::KeyboardS))
            {
                vc::Camera::GetMainCamera()->MoveForward(-speed * vc::Timer::GetLambdaSeconds());
            }
            if (Context::Get()->IsKeyPressed(vc::KeyboardInput::KeyboardA))
            {
                vc::Camera::GetMainCamera()->MoveRight(-speed * vc::Timer::GetLambdaSeconds());
            }
            if (Context::Get()->IsKeyPressed(vc::KeyboardInput::KeyboardD))
            {
                vc::Camera::GetMainCamera()->MoveRight(speed * vc::Timer::GetLambdaSeconds());
            }
            if (Context::Get()->IsKeyPressed(vc::KeyboardInput::KeyboardQ))
            {
                vc::Camera::GetMainCamera()->MoveUp(-speed * vc::Timer::GetLambdaSeconds());
            }
            if (Context::Get()->IsKeyPressed(vc::KeyboardInput::KeyboardE))
            {
                vc::Camera::GetMainCamera()->MoveUp(speed * vc::Timer::GetLambdaSeconds());
            }

            vcm::Vec2 mouseMov = Context::Get()->GetMouseMove();
            vc::Camera::GetMainCamera()->RotateYaw( -mouseMov.x * 0.0025f);
            vc::Camera::GetMainCamera()->RotatePitch(-mouseMov.y * 0.0025f);

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

void VenomEngine::__LoadECS()
{
    // Reserve entities

    __ecs->RegisterComponent<Shader>();
    __ecs->RegisterComponent<Model>();
    __ecs->RegisterComponent<Skybox>();
}
}
}
