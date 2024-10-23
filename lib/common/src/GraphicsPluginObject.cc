///
/// Project: VenomEngine
/// @file GraphicsPluginObject.cc
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GraphicsPluginObject.h>
#include <venom/common/plugin/graphics/GraphicsPlugin.h>

#include <venom/common/Log.h>

#include <unordered_map>

#include <venom/common/Resources.h>
#include <filesystem>

namespace venom
{
namespace common
{
GraphicsCachedResource::~GraphicsCachedResource()
{
}

void GraphicsCachedResource::ReleaseFromCache()
{
    for (auto it = GraphicsPlugin::__GetGraphicsResourceCache()->begin(); it != GraphicsPlugin::__GetGraphicsResourceCache()->end(); ++it) {
        if (it->second.get() == this) {
            GraphicsPlugin::__GetGraphicsResourceCache()->erase(it);
            break;
        }
    }
}

GraphicsCachedResourceHolder::GraphicsCachedResourceHolder()
{
}

GraphicsPluginObject::GraphicsPluginObject()
    : PluginObject(PluginType::Graphics)
{
}

GraphicsPluginObject::~GraphicsPluginObject()
{
}

void GraphicsPluginObject::Destroy()
{
    // // Remove from cache
    // // TODO: Optimize destroying process
    // for (auto it = s_cache.begin(); it != s_cache.end(); ++it) {
    //     if (it->second.get() == this) {
    //         s_cache.erase(it);
    //         break;
    //     }
    // }
    PluginObject::Destroy();
}

static bool validPath(const std::string& path, std::string & res)
{
    std::error_code ec;
    auto realPath = std::filesystem::canonical(path, ec);
    if (realPath.empty() || ec) {
        Log::Error("Failed to find relative path when looking cache for: %s", path.c_str());
        return false;
    }
    realPath = std::filesystem::relative(realPath, ec);
    if (realPath.empty() || ec) {
        Log::Error("Failed to find relative path when looking cache for: %s", path.c_str());
        return false;
    }
    res = realPath;
    return true;
}

bool GraphicsPluginObject::HasCachedObject(const std::string& path)
{
    std::string realPath;
    if (!validPath(path, realPath))
        return false;
    return GraphicsPlugin::__GetGraphicsResourceCache()->find(realPath) != GraphicsPlugin::__GetGraphicsResourceCache()->end();
}

std::shared_ptr<GraphicsCachedResource> GraphicsPluginObject::GetCachedObject(const std::string& path)
{
    std::string realPath;
    if (!validPath(path, realPath))
        return nullptr;
    const auto it = GraphicsPlugin::__GetGraphicsResourceCache()->find(realPath);
    return it != GraphicsPlugin::__GetGraphicsResourceCache()->end() ? it->second : std::shared_ptr<GraphicsCachedResource>();
}

void GraphicsPluginObject::_SetInCache(const std::string& path, const std::shared_ptr<GraphicsCachedResource> & object)
{
    // Get Relative path to working dir
    std::string realPath;
    if (!validPath(path, realPath))
        return;
    venom_assert(GraphicsPlugin::__GetGraphicsResourceCache()->find(realPath) == GraphicsPlugin::__GetGraphicsResourceCache()->end(), "Object already in cache");
    GraphicsPlugin::__GetGraphicsResourceCache()->operator[](realPath) = object;
}

void GraphicsPluginObject::_SetCacheSize(size_t size)
{
    GraphicsPlugin::__GetGraphicsResourceCache()->reserve(size);
}

void GraphicsPluginObject::_AddCacheSize(size_t size)
{
    GraphicsPlugin::__GetGraphicsResourceCache()->reserve(GraphicsPlugin::__GetGraphicsResourceCache()->size() + size);
}
}
}
