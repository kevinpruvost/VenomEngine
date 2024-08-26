///
/// Project: Bazel_Vulkan_Metal
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

class VenomEngine;
class VENOM_COMMON_API PluginManager
{
    friend class VenomEngine;
public:
    ~PluginManager();

    GraphicsPlugin * GetGraphicsPlugin();

    void AddPluginObject(const Plugin::PluginType type, PluginObject * object);
    void RemovePluginObject(const Plugin::PluginType type, PluginObject * object);
private:
    PluginManager();

    Error LoadAllPlugins();

    Error LoadGraphicsPlugin();

    void CleanPluginsObjets();

    std::unique_ptr<GraphicsPlugin> __graphicsPlugin;
};

}
}