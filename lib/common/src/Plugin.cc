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
static vc::Vector<Plugin *> s_plugins;

Plugin::Plugin(const PluginType type)
    : __type(type)
    , __objects()
    , __objectsToRemove()
    , __objectsToRemoveLocked(false)
    , __objectsToRemoveNext()
{
    s_plugins.emplace_back(this);
}

vc::Vector<Plugin *> Plugin::GetAllPlugins()
{
    return s_plugins;
}

void Plugin::AddPluginObject(IPluginObject* object)
{
    __objects.emplace_back(object);
}

void Plugin::RemovePluginObject(IPluginObject* object)
{
    // We need to delay the destruction as this function might be called from the object to delete itself
    auto ite = std::find_if(__objects.begin(), __objects.end(), [object](const vc::UPtr<IPluginObject>& obj) { return obj.get() == object; });
    if (ite != __objects.end())
    {
        // Move to remove list
        if (__objectsToRemoveLocked)
            __objectsToRemoveNext.emplace_back(std::move(*ite));
        else
            __objectsToRemove.emplace_back(std::move(*ite));
        __objects.erase(ite);
    }
}

void Plugin::CleanPluginObjects()
{
    while (__objectsToRemove.size() != 0) {
        __objectsToRemoveLocked = true;
        __objectsToRemove.clear();
        __objectsToRemoveLocked = false;
        __objectsToRemove = std::move(__objectsToRemoveNext);
    }
}

Plugin::~Plugin()
{
    TerminatePluginObjects();
    // Erase itself from the list
    std::erase(s_plugins, this);
}

void Plugin::TerminatePluginObjects()
{
    // Cleaning objects in reverse order
    // Very important as some objects might depend on others (first object for instance will surely be the Application)
    int i = __objects.size() - 1;
    for (auto ite = __objects.rbegin(); ite != __objects.rend(); ++ite) {
        ite->reset();
        CleanPluginObjects();
        --i;
    }
//    for (int i = static_cast<int>(__objects.size()) - 1; i >= 0; --i) {
//        __objects[i].reset();
//        CleanPluginObjects();
//    }
    __objects.clear();
}

const PluginType Plugin::GetType() const
{
    return __type;
}
}
}
