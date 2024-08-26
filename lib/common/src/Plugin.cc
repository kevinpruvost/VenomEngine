///
/// Project: Bazel_Vulkan_Metal
/// @file Plugin.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/Plugin.h>

#include <iterator>

namespace venom
{
namespace common
{
Plugin::Plugin(const PluginType type)
    : __type(type)
    , __objects()
    , __objectsToRemove()
{
}

void Plugin::AddPluginObject(PluginObject* object)
{
    __objects.emplace_back(object);
}

void Plugin::RemovePluginObject(PluginObject* object)
{
    auto ite = std::find_if(__objects.begin(), __objects.end(), [object](const std::shared_ptr<PluginObject>& obj) { return obj.get() == object; });
    if (ite != __objects.end()) {
        std::move(ite, ite + 1, std::back_inserter(__objectsToRemove));
    }
}

void Plugin::CleanPluginObjects()
{
    __objectsToRemove.clear();
}

Plugin::~Plugin()
{
}

const Plugin::PluginType Plugin::GetType() const
{
    return __type;
}
}
}