///
/// Project: VenomEngine
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
    : __graphicsPluginType(GraphicsPlugin::GraphicsPluginType::Vulkan)
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

GraphicsPlugin::GraphicsPluginType Config::GetGraphicsPluginType()
{
    return GetInstance()->__GetGraphicsPluginType();
}

void Config::SetGraphicsPluginType(GraphicsPlugin::GraphicsPluginType type)
{
    GetInstance()->__SetGraphicsPluginType(type);
}

GraphicsPlugin::GraphicsPluginType Config::__GetGraphicsPluginType() const
{
    return __graphicsPluginType;
}

void Config::__SetGraphicsPluginType(GraphicsPlugin::GraphicsPluginType type)
{
    __graphicsPluginType = type;
}
}
}