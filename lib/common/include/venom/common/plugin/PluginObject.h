///
/// Project: VenomEngine
/// @file PluginObject.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <memory>
#include <venom/common/plugin/PluginType.h>
#include <venom/common/Export.h>

namespace venom
{
namespace common
{

class VENOM_COMMON_API PluginObjectImpl
{
public:
    template<class T> inline T * As() { return static_cast<T *>(this); }
    template<class T> inline const T * As() const { return static_cast<const T *>(this); }
};

class VENOM_COMMON_API PluginObject
{
public:
    PluginObject(const PluginType type);
    virtual ~PluginObject();
    /// @brief /!\ THIS FUNCTION MUST BE CALLED FOR DESTRUCTION, DO NOT USE `delete`
    virtual void Destroy();

    template<class T> inline T * As() { return dynamic_cast<T *>(this); }
    template<class T> inline const T * As() const { return dynamic_cast<const T *>(this); }
    PluginObjectImpl * GetImpl();
    const PluginObjectImpl * GetImpl() const;
protected:
    PluginObjectImpl * _impl;
private:
    const PluginType __type;
};

}
}
