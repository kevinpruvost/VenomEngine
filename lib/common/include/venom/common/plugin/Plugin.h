///
/// Project: VenomEngine
/// @file Plugin.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <memory>
#include <vector>
#include <venom/common/Error.h>
#include <venom/common/DLL.h>
#include <venom/common/plugin/PluginObject.h>

namespace venom
{
namespace common
{
class PluginManager;
class VENOM_COMMON_API Plugin
{
public:
    virtual ~Plugin();
    Plugin(const Plugin&) = delete;
    Plugin& operator=(const Plugin&) = delete;

    friend class PluginManager;

    const PluginType GetType() const;

protected:
    Plugin(const PluginType type);

private:
    void AddPluginObject(PluginObject * object);
    void RemovePluginObject(PluginObject * object);
    void CleanPluginObjects();

private:
    const PluginType __type;
    std::vector<std::unique_ptr<PluginObject>> __objects;
    std::vector<std::unique_ptr<PluginObject>> __objectsToRemove;
};
}
}