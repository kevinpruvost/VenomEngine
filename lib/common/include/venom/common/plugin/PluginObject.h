///
/// Project: VenomEngine
/// @file PluginObject.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/PluginType.h>
#include <venom/common/Export.h>
#include <venom/common/Log.h>
#include <venom/common/Ptr.h>
#include <venom/common/Containers.h>

#include <memory>
#include <optional>

namespace venom
{
namespace common
{
class VENOM_COMMON_API IPluginObject
{
public:
    virtual ~IPluginObject() = default;

    virtual void Destroy() = 0;
    virtual PluginType GetType() const = 0;
};

/**
 * @brief Base class for all the plugin objects
 * Objects that will be loaded from the plugins as their implementations lie there
 * As a user of VenomEngine, you should not care about that, unless you plan
 * to create your own plugin
 */
class VENOM_COMMON_API PluginObject : virtual public IPluginObject
{
public:
    PluginObject(const PluginType type);

    virtual ~PluginObject();
    /// @brief /!\ THIS FUNCTION MUST BE CALLED FOR DESTRUCTION, DO NOT USE `delete`
    virtual void Destroy() override;
    PluginType GetType() const override;

    template<class T> inline T * As() { return static_cast<T *>(this); }
    template<class T> inline const T * As() const { return static_cast<const T *>(this); }
    template<class T> inline T * ConstAs() const { return const_cast<T *>(static_cast<const T *>(this)); }
    template<class T> inline T * RAs() { return reinterpret_cast<T *>(this); }
    template<class T> inline const T * RAs() const { return reinterpret_cast<const T *>(this); }
    template<class T> inline T * DAs() { return dynamic_cast<T *>(this); }
    template<class T> inline const T * DAs() const { return dynamic_cast<const T *>(this); }
    void IncRefCount();
    void DecRefCount();
private:
    const PluginType __type;
    int __refCount;
};

/**
 * @brief Wrapper for the PluginObjectImpl, inherited by classes like Model or Mesh
 * to make them usable as components (for ECS) and hold the Graphics API specific implementation
 */
class VENOM_COMMON_API PluginObjectWrapper
{
public:
    PluginObjectWrapper();
    PluginObjectWrapper(PluginObject * impl);
    PluginObjectWrapper(const PluginObjectWrapper & other);
    PluginObjectWrapper & operator=(const PluginObjectWrapper & other);
    PluginObjectWrapper(PluginObjectWrapper && other);
    PluginObjectWrapper & operator=(PluginObjectWrapper && other);
    virtual ~PluginObjectWrapper();

    inline PluginObject* GetImpl()
    {
        venom_assert(_impl != nullptr, "PluginObjectWrapper::GetImpl() : _impl is nullptr");
        return _impl;
    }
    inline const PluginObject* GetImpl() const
    {
        venom_assert(_impl != nullptr, "PluginObjectWrapper::GetImpl() : _impl is nullptr");
        return _impl;
    }
    inline const PluginObject * GetConstImpl() const { return GetImpl(); }
    bool IsImplInitialized() const;
    inline void Destroy() { _impl->Destroy(); }
protected:
    PluginObject * _impl;
};

/**
 * @brief Container for the PluginObjectWrapper
 * Important to avoid it from loading the PluginObject if not needed
 * e.g. if the object is optional
 */

template<class T, typename = std::enable_if_t<std::is_base_of_v<PluginObjectWrapper, T>>>
class VENOM_COMMON_API PluginObjectOptional : public std::optional<T>
{
public:
    PluginObjectOptional() = default;
};

}
}
