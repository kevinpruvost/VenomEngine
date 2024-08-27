///
/// Project: Bazel_Vulkan_Metal
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

namespace venom
{
namespace common
{
VenomEngine::VenomEngine()
    : pluginManager()
    , __dllCache(new DLL_Cache())
{
    venom_assert(__dllCache, "VenomEngine::VenomEngine() : __dllCache is nullptr");
    DLL_Cache::SetCache(__dllCache.get());
    Error err;
    if (err = pluginManager.LoadAllPlugins(); err != Error::Success) {
        Log::Error("VenomEngine::VenomEngine() : Failed to load all plugins");
        abort();
    }
    if (err = MemoryPool::CreateMemoryPool(); err != Error::Success) {
        Log::Error("VenomEngine::VenomEngine() : Failed to create memory pool");
        abort();
    }
}

// template<>
// VenomEngine * Singleton<VenomEngine>::GetCache()
// {
//     static VenomEngine instance;
//     return &instance;
// }

VenomEngine::~VenomEngine()
{
}

VenomEngine* VenomEngine::GetInstance()
{
    static VenomEngine instance;
    return &instance;
}
}
}
