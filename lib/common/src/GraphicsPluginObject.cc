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

GraphicsPluginObject* GraphicsPluginObject::GetCachedObject(const std::string& path)
{
    const auto it = s_cache.find(path);
    return it != s_cache.end() ? it->second : nullptr;
}

void GraphicsPluginObject::_SetInCache(const std::string& path, GraphicsPluginObject* object)
{
    venom_assert(s_cache.find(path) == s_cache.end(), "Object already in cache");
    s_cache[path] = object;
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