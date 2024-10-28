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

    template<class T> inline T * DAs() { return dynamic_cast<T *>(this); }
    template<class T> inline const T * DAs() const { return dynamic_cast<const T *>(this); }
private:
    const PluginType __type;
};

/**
 * @brief Interface for the PluginObject different implementations
 */
class VENOM_COMMON_API PluginObjectImpl : virtual public IPluginObject
{
public:
    PluginObjectImpl();
    template<class T> inline T * As() { return static_cast<T *>(this); }
    template<class T> inline const T * As() const { return static_cast<const T *>(this); }
    template<class T> inline T * ConstAs() const { return const_cast<T *>(static_cast<const T *>(this)); }
    template<class T> inline T * RAs() { return reinterpret_cast<T *>(this); }
    template<class T> inline const T * RAs() const { return reinterpret_cast<const T *>(this); }
    void IncRefCount();
    void DecRefCount();

private:
    int __refCount;
};

/**
 * @brief Wrapper for the PluginObjectImpl, inherited by classes like Model or Mesh
 * to make them usable as components (for ECS) and hold the Graphics API specific implementation
 */
class VENOM_COMMON_API PluginObjectImplWrapper
{
public:
    PluginObjectImplWrapper();
    PluginObjectImplWrapper(PluginObjectImpl * impl);
    PluginObjectImplWrapper(const PluginObjectImplWrapper & other);
    PluginObjectImplWrapper & operator=(const PluginObjectImplWrapper & other);
    PluginObjectImplWrapper(PluginObjectImplWrapper && other);
    PluginObjectImplWrapper & operator=(PluginObjectImplWrapper && other);
    ~PluginObjectImplWrapper();

    inline PluginObjectImpl* GetImpl()
    {
        venom_assert(_impl != nullptr, "PluginObjectImplWrapper::GetImpl() : _impl is nullptr");
        return _impl;
    }
    inline const PluginObjectImpl* GetImpl() const
    {
        venom_assert(_impl != nullptr, "PluginObjectImplWrapper::GetImpl() : _impl is nullptr");
        return _impl;
    }
    inline const PluginObjectImpl * GetConstImpl() const { return GetImpl(); }
    bool IsImplInitialized() const;
    inline void Destroy() { _impl->Destroy(); }
protected:
    PluginObjectImpl * _impl;
};

/**
 * @brief Container for the PluginObjectImplWrapper
 * Important to avoid it from loading the PluginObjectImpl if not needed
 * e.g. if the object is optional
 */

template<class T, typename = std::enable_if_t<std::is_base_of_v<PluginObjectImplWrapper, T>>>
class VENOM_COMMON_API PluginObjectContainer : public std::optional<T>
{
public:
    PluginObjectContainer() = default;
};

}
}
