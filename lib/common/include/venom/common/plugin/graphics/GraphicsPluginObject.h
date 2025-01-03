///
/// Project: VenomEngine
/// @file GraphicsPluginObject.h
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/PluginObject.h>

namespace venom
{
namespace common
{
class GraphicsCachedResource;
class GraphicsCachedResourceHolder;
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
    GraphicsCachedResource();
    GraphicsCachedResource(GraphicsCachedResourceHolder * h);
    virtual ~GraphicsCachedResource();
    GraphicsCachedResource(const GraphicsCachedResource &) = delete;
    GraphicsCachedResource & operator=(const GraphicsCachedResource &) = delete;
    void ReleaseFromCache();

    template<typename T> inline bool IsType() const { return dynamic_cast<const T *>(this) != nullptr; }
    template<typename T> inline T * As() { return static_cast<T *>(this); }
    template<typename T> inline const T * As() const { return static_cast<const T *>(this); }

    inline void SetHolder(GraphicsCachedResourceHolder * h) { __holder = h; }
    inline GraphicsCachedResourceHolder * GetHolder() const { return __holder; }

    void SetName(const vc::String & name);
    inline const vc::String & GetName() const { return __name; }
    inline const vc::String & GetShortName() const { return __shortName; }
private:
    GraphicsCachedResourceHolder * __holder;
    vc::String __name;
    vc::String __shortName;
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

public:
    template<typename T> T * As() { return dynamic_cast<T *>(this); }

    inline void SetResource(const GraphicsCachedResourceHolder & resource) { _resource = resource._resource; }
    inline void SetResource(const SPtr<GraphicsCachedResource> & resource) { _resource = resource; }
    inline const vc::String & GetResourceName() const { return _resource->GetName(); }
    inline const vc::String & GetResourceShortName() const { return _resource->GetShortName(); }
protected:
    inline const SPtr<GraphicsCachedResource> & _GetResourceToCache() const { return _resource; }
    inline SPtr<GraphicsCachedResource> & _GetResourceToCache() { return _resource; }
    inline void _LoadFromCache(const SPtr<GraphicsCachedResource> & cache) { _resource = cache; }
    /**
     * @brief Resets the resource, should be implemented by the final class, i.e. VulkanTextureResource
     */
    virtual void _ResetResource() = 0;

    SPtr<GraphicsCachedResource> _resource;
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
    static SPtr<GraphicsCachedResource> GetCachedObject(const std::string & path);

    /**
     * @brief Gets all cached objects
     * @return cached objects
     */
    static const std::unordered_map<std::string, std::shared_ptr<GraphicsCachedResource>> & GetCachedObjects();
protected:
    /**
     * @brief Sets an object in the cache
     * @param path
     * @warning `path` must be the original asset path relative to the `resources` folder
     * @param object
     */
    static void _SetInCache(const std::string & path, const SPtr<GraphicsCachedResource> & object);
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
