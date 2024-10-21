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
class GraphicsCachedResource;
class GraphicsPluginObject;

// Must not inherit GraphicsCachedResource and GraphicsPluginObject at the same time
template<class T>
concept ValidGraphicsCachedResource = std::is_base_of<GraphicsCachedResource, T>::value && !std::is_base_of<GraphicsPluginObject, T>::value;

/**
 * @brief Base class for cached resources
 * @warning DO NOT INHERIT with GraphicsPluginObject
 */
class VENOM_COMMON_API GraphicsCachedResource
{
public:
    GraphicsCachedResource() {}
    virtual ~GraphicsCachedResource();
    void ReleaseFromCache();

    template<typename T> inline T * As() { return static_cast<T *>(this); }
    template<typename T> inline const T * As() const { return static_cast<const T *>(this); }
};

// Must not inherit GraphicsCachedResource and GraphicsPluginObject at the same time
template<class T>
concept ValidGraphicsCachedResourceHolder = !std::is_base_of<GraphicsCachedResource, T>::value && std::is_base_of<GraphicsPluginObject, T>::value;

/**
 * @brief Base class for cached resource holders
 * @warning DO NOT INHERIT with GraphicsCachedResource
 */
class VENOM_COMMON_API GraphicsCachedResourceHolder
{
public:
    GraphicsCachedResourceHolder();
    virtual ~GraphicsCachedResourceHolder() = default;

protected:
    inline const std::shared_ptr<GraphicsCachedResource> & _GetTextureToCache() { return _resource; }
    inline void _LoadFromCache(const std::shared_ptr<GraphicsCachedResource> & cache) { _resource = cache; }

    std::shared_ptr<GraphicsCachedResource> _resource;
};

/**
 * @brief Base class for graphics plugin objects
 * @warning DO NOT INHERIT with GraphicsCachedResource
 */
class VENOM_COMMON_API GraphicsPluginObject : public PluginObject
{
public:
    GraphicsPluginObject();
    ~GraphicsPluginObject() override;

    virtual void Destroy() override;

    /**
     * @brief Checks if an object is in the cache
     * @param path relative path to the asset
     * @return true if found in the cache
     */
    static bool HasCachedObject(const std::string & path);

    /**
     * @brief Gets a cached object from the graphics cache
     * @param path (to the original asset)
     * @warning `path` must be the original asset path relative to the `resources` folder
     * @return nullptr if the object is not in cache, the object otherwise
     */
    static std::shared_ptr<GraphicsCachedResource> GetCachedObject(const std::string & path);
protected:
    /**
     * @brief Sets an object in the cache
     * @param path
     * @warning `path` must be the original asset path relative to the `resources` folder
     * @param object
     */
    static void _SetInCache(const std::string & path, const std::shared_ptr<GraphicsCachedResource> & object);
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