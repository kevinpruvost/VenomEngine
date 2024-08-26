///
/// Project: Bazel_Vulkan_Metal
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

namespace venom
{
namespace common
{
class PluginManager;
class PluginObject;
class VENOM_COMMON_API Plugin
{
public:
    virtual ~Plugin();

    friend class PluginManager;
    enum class PluginType
    {
        Graphics = 0,
        TotalCount
    };

    const PluginType GetType() const;

protected:
    Plugin(const PluginType type);

private:
    void AddPluginObject(PluginObject * object);
    void RemovePluginObject(PluginObject * object);
    void CleanPluginObjects();

private:
    const PluginType __type;
    std::vector<std::shared_ptr<PluginObject>> __objects;
    std::vector<std::shared_ptr<PluginObject>> __objectsToRemove;
};
}
}