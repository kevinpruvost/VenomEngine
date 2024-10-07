///
/// Project: VenomEngine
/// @file GraphicsPluginObject.cc
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GraphicsPluginObject.h>

#include <venom/common/Log.h>

#include <unordered_map>

#include <venom/common/Resources.h>
#include <filesystem>

namespace venom
{
namespace common
{
GraphicsPluginObject::GraphicsPluginObject()
    : PluginObject(PluginType::Graphics)
{
}

GraphicsPluginObject::~GraphicsPluginObject()
{
}

static std::unordered_map<std::string, GraphicsPluginObject *> s_cache;
void GraphicsPluginObject::Destroy()
{
    // Remove from cache
    // TODO: Optimize destroying process
    for (auto it = s_cache.begin(); it != s_cache.end(); ++it) {
        if (it->second == this) {
            s_cache.erase(it);
            break;
        }
    }
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

GraphicsPluginObject* GraphicsPluginObject::GetCachedObject(const std::string& path)
{
    std::string realPath;
    if (!validPath(path, realPath))
        return nullptr;
    const auto it = s_cache.find(realPath);
    return it != s_cache.end() ? it->second : nullptr;
}

void GraphicsPluginObject::_SetInCache(const std::string& path, GraphicsPluginObject* object)
{
    // Get Relative path to working dir
    std::string realPath;
    if (!validPath(path, realPath))
        return;
    venom_assert(s_cache.find(realPath) == s_cache.end(), "Object already in cache");
    s_cache[realPath] = object;
}

void GraphicsPluginObject::_SetCacheSize(size_t size)
{
    s_cache.reserve(size);
}

void GraphicsPluginObject::_AddCacheSize(size_t size)
{
    s_cache.reserve(s_cache.size() + size);
}
}
}
