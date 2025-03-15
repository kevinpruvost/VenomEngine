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
#if !defined(VENOM_DISABLE_GLFW)
    , __contextType(Context::ContextType::GLFW)
#elif defined(VENOM_PLATFORM_APPLE)
    , __contextType(Context::ContextType::Apple)
#else
#error "No context type defined"
#endif
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

void Config::SetGraphicsPluginType(GraphicsPlugin::GraphicsPluginType type)
{
    GetInstance()->__SetGraphicsPluginType(type);
}

void Config::SetContextType(vc::Context::ContextType type)
{
    GetInstance()->__SetContextType(type);
}

GraphicsPlugin::GraphicsPluginType Config::__GetGraphicsPluginType() const
{
    return __graphicsPluginType;
}

void Config::__SetGraphicsPluginType(GraphicsPlugin::GraphicsPluginType type)
{
    __graphicsPluginType = type;
}

Context::ContextType Config::__GetContextType() const
{
    return __contextType;
}

void Config::__SetContextType(Context::ContextType type)
{
    __contextType = type;
}
}
}
