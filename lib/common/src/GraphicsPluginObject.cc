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
GraphicsCachedResource::GraphicsCachedResource()
    : __name("None")
    , __shortName("None")
{
}

GraphicsCachedResource::GraphicsCachedResource(GraphicsCachedResourceHolder* h)
    : GraphicsCachedResource()
{
    __holder = h;
}

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

void GraphicsCachedResource::SetName(const vc::String& name)
{
    __name = name;
    __shortName = std::filesystem::path(name).filename().string();
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

static bool validPath(const vc::String& path, vc::String & res)
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

bool GraphicsPluginObject::HasCachedObject(const vc::String& path)
{
    vc::String realPath;
    if (!validPath(path, realPath))
        return false;
    return GraphicsPlugin::__GetGraphicsResourceCache()->find(realPath) != GraphicsPlugin::__GetGraphicsResourceCache()->end();
}

vc::SPtr<GraphicsCachedResource> GraphicsPluginObject::GetCachedObject(const vc::String& path)
{
    vc::String realPath;
    if (!validPath(path, realPath))
        realPath = path;
    const auto it = GraphicsPlugin::__GetGraphicsResourceCache()->find(realPath);
    return it != GraphicsPlugin::__GetGraphicsResourceCache()->end() ? it->second : vc::SPtr<GraphicsCachedResource>();
}

const vc::UMap<vc::String, vc::SPtr<GraphicsCachedResource>> & GraphicsPluginObject::GetCachedObjects()
{
    return *GraphicsPlugin::__GetGraphicsResourceCache();
}

void GraphicsPluginObject::_SetInCache(const vc::String& path, const vc::SPtr<GraphicsCachedResource> & object)
{
    // Get Relative path to working dir
    vc::String realPath;
    if (!validPath(path, realPath))
        realPath = path;
    venom_assert(GraphicsPlugin::__GetGraphicsResourceCache()->find(realPath) == GraphicsPlugin::__GetGraphicsResourceCache()->end(), "Object already in cache");
    object->SetName(realPath);
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
