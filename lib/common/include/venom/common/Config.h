///
/// Project: VenomEngine
/// @file Config.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPlugin.h>
#include <venom/common/plugin/context/Context.h>

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
    static GraphicsPlugin::GraphicsPluginType GetGraphicsPluginType();
    static void SetGraphicsPluginType(GraphicsPlugin::GraphicsPluginType type);
    static Context::ContextType GetContextType();
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
