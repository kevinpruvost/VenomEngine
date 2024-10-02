///
/// Project: VenomEngine
/// @file GraphicsPluginObject.h
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/PluginObject.h>

#include <string>

namespace venom
{
namespace common
{
class VENOM_COMMON_API GraphicsPluginObject : public PluginObject
{
public:
    GraphicsPluginObject();
    ~GraphicsPluginObject() override;


    /**
     * @brief Gets a cached object from the graphics cache
     * @param path (to the original asset)
     * @warning `path` must be the original asset path relative to the `resources` folder
     * @return nullptr if the object is not in cache, the object otherwise
     */
    static GraphicsPluginObject * GetCachedObject(const std::string & path);
protected:
    /**
     * @brief Sets an object in the cache
     * @param path
     * @warning `path` must be the original asset path relative to the `resources` folder
     * @param object
     */
    static void _SetInCache(const std::string & path, GraphicsPluginObject * object);
    /**
     * @brief Adds cache size to avoid unnecessary additional allocations
     * @param size
     */
    static void _SetCacheSize(size_t size);
    /**
     * @brief Adds cache size to avoid unnecessary additional allocations
     * @param size
     */
    static void _AddCacheSize(size_t size);
};
}
}