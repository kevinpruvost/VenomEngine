///
/// Project: VenomEngine
/// @file PluginManager.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/Plugin.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace venom
{
namespace common
{

// Plugins list
class GraphicsPlugin;
class ContextPlugin;

class VenomEngine;
class VENOM_COMMON_API PluginManager
{
    friend class VenomEngine;
public:
    ~PluginManager();

    GraphicsPlugin * GetGraphicsPlugin();
    ContextPlugin * GetContextPlugin();

    void AddPluginObject(const PluginType type, IPluginObject * object);
    void RemovePluginObject(const PluginType type, IPluginObject * object);
    void UnloadPlugins();
private:
    PluginManager();

    Error LoadAllPlugins();

    Error LoadGraphicsPlugin();
    Error LoadContextPlugin();

    void CleanPluginsObjets();

    vc::UPtr<GraphicsPlugin> __graphicsPlugin;
    vc::UPtr<ContextPlugin> __contextPlugin;
};

}
}