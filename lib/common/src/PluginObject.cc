///
/// Project: Bazel_Vulkan_Metal
/// @file PluginObject.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/PluginObject.h>

#include <venom/common/VenomEngine.h>
#include <venom/common/Debug.h>

#include "venom/common/Log.h"

namespace venom
{
namespace common
{
PluginObject::PluginObject(const Plugin::PluginType type)
    : _impl(nullptr)
    , __type(type)
{
    VenomEngine::GetInstance()->pluginManager.AddPluginObject(__type, this);
}

PluginObject::~PluginObject()
{
    Log::Print("Plugin Object destruction...");
}

void PluginObject::Destroy()
{
    VenomEngine::GetInstance()->pluginManager.RemovePluginObject(__type, this);
}

}
}
