///
/// Project: VenomEngine
/// @file PluginManager.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/PluginManager.h>

#include <venom/common/plugin/graphics/GraphicsPlugin.h>
#include <venom/common/VenomEngine.h>
#include <venom/common/Log.h>
#include <venom/common/Config.h>

#include <venom/common/Resources.h>

namespace venom
{
namespace common
{
PluginManager::PluginManager()
{
}

void PluginManager::CleanPluginsObjets()
{
    __graphicsPlugin->CleanPluginObjects();
}

PluginManager::~PluginManager()
{
}

GraphicsPlugin* PluginManager::GetGraphicsPlugin()
{
    venom_assert(__graphicsPlugin, "GraphicsPlugin is nullptr");
    return __graphicsPlugin.get();
}

Error PluginManager::LoadAllPlugins()
{
    Error err;
    if (err = LoadGraphicsPlugin(); err != Error::Success)
        return err;
    return err;
}

template<typename T>
T * LoadPluginFromNameAndType(const std::string & libName, const char * functionName)
{
    DLL * dll = DLL::GetFromCache(libName);
    if (!dll) {
        dll = DLL::Create();
        Error err;
#if defined(__APPLE__) && defined(VENOM_PACKAGE)
        // Try to load from the bundle (/Frameworks)
        std::string newLibPath = Resources::GetResourcePath("../Frameworks/" + libName);
        err = dll->LoadDLL(newLibPath.c_str());
        if (err != Error::Success)
#endif
        {
            err = dll->LoadDLL(libName.c_str());
        }
        if (err != Error::Success)
        {
            Log::Error("Failed to load %s", libName.c_str());
            return nullptr;
        }
    }
    void * function = dll->GetFunction(functionName);
    if (!function)
    {
        Log::Error("Failed to load '%s' function from %s", functionName, libName.c_str());
        return nullptr;
    }
    auto *createPluginFunc = reinterpret_cast<T * (*)()>(function);
    if (!createPluginFunc)
    {
        Log::Error("Failed to convert '%s' ptr to function from %s", functionName, libName.c_str());
        return nullptr;
    }
    return createPluginFunc();
}

Error PluginManager::LoadGraphicsPlugin()
{
    Error err;
    const GraphicsPlugin::GraphicsPluginType type = Config::GetGraphicsPluginType();
    std::string libName;
    switch (type)
    {
    case GraphicsPlugin::GraphicsPluginType::Vulkan:
        libName = "VenomVulkan";
        break;
#ifdef __APPLE__
    case GraphicsPlugin::GraphicsPluginType::Metal:
        libName = "VenomMetal";
        break;
#endif
    default:
        Log::Error("Unknown GraphicsPluginType");
        return Error::Failure;
    }
    __graphicsPlugin.reset(LoadPluginFromNameAndType<GraphicsPlugin>(libName, "createGraphicsPlugin"));
    if (!__graphicsPlugin)
    {
        Log::Error("Failed to create GraphicsPlugin from %s", libName.c_str());
        return Error::Failure;
    }
    return Error::Success;
}

void PluginManager::AddPluginObject(const PluginType type, IPluginObject* object)
{
    switch (type)
    {
    case PluginType::Graphics:
        __graphicsPlugin->AddPluginObject(object);
        break;
    default:
        Log::Error("PluginManager::AddPluginObject: Unknown plugin type");
        break;
    }
}

void PluginManager::RemovePluginObject(const PluginType type, IPluginObject* object)
{
    switch (type)
    {
    case PluginType::Graphics:
        __graphicsPlugin->RemovePluginObject(object);
        break;
    default:
        Log::Error("PluginManager::RemovePluginObject: Unknown plugin type");
        break;
    }
}

void PluginManager::UnloadPlugins()
{
    __graphicsPlugin.reset();
}
}
}
