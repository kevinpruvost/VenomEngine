///
/// Project: VenomEngine
/// @file Plugin.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/Plugin.h>
#include <venom/common/plugin/PluginObject.h>

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

void Plugin::AddPluginObject(IPluginObject* object)
{
    __objects.emplace_back(object);
}

void Plugin::RemovePluginObject(IPluginObject* object)
{
    // We need to delay the destruction as this function might be called from the object to delete itself
    auto ite = std::find_if(__objects.begin(), __objects.end(), [object](const std::unique_ptr<IPluginObject>& obj) { return obj.get() == object; });
    if (ite != __objects.end())
    {
        // Move to remove list
        __objectsToRemove.push_back(std::move(*ite));
        __objects.erase(ite);
    }
}

void Plugin::CleanPluginObjects()
{
    __objectsToRemove.clear();
}

Plugin::~Plugin()
{
    // Cleaning objects in reverse order
    // Very important as some objects might depend on others (first object for instance will surely be the Application)
    for (auto ite = __objects.rbegin(); ite != __objects.rend(); ++ite) {
        ite->reset();
    }
}

const PluginType Plugin::GetType() const
{
    return __type;
}
}
}