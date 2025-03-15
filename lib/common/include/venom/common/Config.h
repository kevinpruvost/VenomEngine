///
/// Project: VenomEngine
/// @file Config.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPlugin.h>
#include <venom/common/Context.h>

#define IS_CONTEXT_TYPE(type) (vc::Config::GetContextType() == vc::Context::ContextType::type)

namespace venom
{
namespace common
{

class VenomEngine;
class VENOM_COMMON_API Config
{
private:
    Config();
public:
    ~Config();
    static Config * GetInstance();
    inline static GraphicsPlugin::GraphicsPluginType GetGraphicsPluginType() { return GetInstance()->__GetGraphicsPluginType(); }
    static void SetGraphicsPluginType(GraphicsPlugin::GraphicsPluginType type);
    inline static Context::ContextType GetContextType() { return GetInstance()->__GetContextType(); }
    static void SetContextType(Context::ContextType type);
private:
    GraphicsPlugin::GraphicsPluginType __GetGraphicsPluginType() const;
    void __SetGraphicsPluginType(GraphicsPlugin::GraphicsPluginType type);

    Context::ContextType __GetContextType() const;
    void __SetContextType(Context::ContextType type);

private:
    GraphicsPlugin::GraphicsPluginType __graphicsPluginType;
    Context::ContextType __contextType;
};
}
}
