///
/// Project: VenomEngine
/// @file Config.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

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
private:
    GraphicsPlugin::GraphicsPluginType __GetGraphicsPluginType() const;
    void __SetGraphicsPluginType(GraphicsPlugin::GraphicsPluginType type);

private:
    GraphicsPlugin::GraphicsPluginType __graphicsPluginType;
};
}
}
