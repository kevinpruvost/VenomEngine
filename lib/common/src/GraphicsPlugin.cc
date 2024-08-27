///
/// Project: Bazel_Vulkan_Metal
/// @file GraphicsPlugin.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GraphicsPlugin.h>

#include "venom/common/Log.h"
#include "venom/common/VenomEngine.h"

namespace venom
{
namespace common
{
GraphicsPluginObject::GraphicsPluginObject()
    : PluginObject(Plugin::PluginType::Graphics)
{
}

GraphicsPluginObject::~GraphicsPluginObject()
{
    Log::Print("Graphics Plugin Object Destruction...");
}

GraphicsPlugin::GraphicsPlugin()
    : Plugin(Plugin::PluginType::Graphics)
{
}

GraphicsPlugin::~GraphicsPlugin()
{
}

GraphicsPlugin* GraphicsPlugin::Get()
{
    return VenomEngine::GetInstance()->pluginManager.GetGraphicsPlugin();
}
}
}
