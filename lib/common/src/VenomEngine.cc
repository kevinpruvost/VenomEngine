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

namespace venom
{
namespace common
{
VenomEngine::VenomEngine()
    : pluginManager()
{
    Error err;
    if (err = pluginManager.LoadAllPlugins(); err != Error::Success) {
        Log::Error("VenomEngine::VenomEngine() : Failed to load all plugins");
        abort();
    }
}

// template<>
// VenomEngine * Singleton<VenomEngine>::GetInstance()
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
