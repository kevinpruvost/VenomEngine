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

namespace venom
{
namespace common
{
VenomEngine::VenomEngine()
    : pluginManager(new PluginManager())
    , __dllCache(new DLL_Cache())
{
    venom_assert(__dllCache, "VenomEngine::VenomEngine() : __dllCache is nullptr");
    DLL_Cache::SetCache(__dllCache.get());
    Error err;
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
    pluginManager.reset();
    __dllCache.reset();
}

static std::unique_ptr<VenomEngine> s_instance;
VenomEngine* VenomEngine::GetInstance()
{
    return s_instance.get();
}

Error VenomEngine::RunEngine(char** argv)
{
    vc::Error err = Error::Success;

    vc::Resources::InitializeFilesystem(argv);

    s_instance.reset(new VenomEngine());
    vc::GraphicsApplication * app = vc::GraphicsApplication::Create();

    if (err = app->Init(); err != vc::Error::Success) {
        printf("Failed to init application: %d\n", static_cast<int>(err));
    }
    while (!app->ShouldClose())
    {
        app->Loop();
        s_instance->pluginManager->CleanPluginsObjets();
    }
    s_instance.reset();
    vc::Resources::FreeFilesystem();
    return err;
}
}
}
