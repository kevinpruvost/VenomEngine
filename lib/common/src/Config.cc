///
/// Project: Bazel_Vulkan_Metal
/// @file Config.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Config.h>

namespace venom
{
namespace common
{
Config::Config()
{
}

Config::~Config()
{
}

Config * Config::GetInstance()
{
    static Config instance;
    return &instance;
}

GraphicsPlugin::GraphicsPluginType Config::GetGraphicsPluginType() const
{
    return GraphicsPlugin::GraphicsPluginType::Vulkan;
}
}
}