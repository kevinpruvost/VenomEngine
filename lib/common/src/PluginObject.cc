///
/// Project: VenomEngine
/// @file PluginObject.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/PluginObject.h>

#include <venom/common/VenomEngine.h>
#include <venom/common/Debug.h>

#include "venom/common/Log.h"

namespace venom
{
namespace common
{
PluginObjectWrapper::PluginObjectWrapper()
    : _impl(nullptr)
{
}

PluginObjectWrapper::PluginObjectWrapper(PluginObject* impl)
    : _impl(impl)
{
    _impl->IncRefCount();
}

PluginObjectWrapper::PluginObjectWrapper(const PluginObjectWrapper& other)
{
    _impl = other._impl;
    _impl->IncRefCount();
}

PluginObjectWrapper& PluginObjectWrapper::operator=(const PluginObjectWrapper& other)
{
    if (_impl != other._impl) {
        if (_impl) _impl->DecRefCount();
        _impl = other._impl;
        _impl->IncRefCount();
    }
    return *this;
}

PluginObjectWrapper::PluginObjectWrapper(PluginObjectWrapper&& other)
    : _impl(other._impl)
{
    _impl->IncRefCount();
}

PluginObjectWrapper& PluginObjectWrapper::operator=(PluginObjectWrapper&& other)
{
    if (_impl) _impl->DecRefCount();
    _impl = other._impl;
    _impl->IncRefCount();
    return *this;
}

PluginObjectWrapper::~PluginObjectWrapper()
{
    if (_impl) _impl->DecRefCount();
}

bool PluginObjectWrapper::IsImplInitialized() const
{
    return _impl != nullptr;
}

void PluginObject::IncRefCount()
{
    ++__refCount;
}

void PluginObject::DecRefCount()
{
    // If the ref count is 0, we need to remove the object from the plugin manager
    // If the engine is terminating, it will free everything anyway
    if (VenomEngine::GetInstance()) {
        --__refCount;
        if (__refCount == 0) {
            VenomEngine::GetInstance()->pluginManager->RemovePluginObject(GetType(), this);
        }
    }
}

PluginObject::PluginObject(const PluginType type)
    : __type(type)
    , __refCount(0)
{
    auto test = VenomEngine::GetInstance();
    VenomEngine::GetInstance()->pluginManager->AddPluginObject(__type, this);
}

PluginObject::~PluginObject()
{
}

void PluginObject::Destroy()
{
    // Not checking for GetInstance as it is a manually called method
    VenomEngine::GetInstance()->pluginManager->RemovePluginObject(__type, this);
}

PluginType PluginObject::GetType() const
{
    return __type;
}
}
}
