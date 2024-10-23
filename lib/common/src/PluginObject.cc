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
PluginObjectImplWrapper::PluginObjectImplWrapper()
    : _impl(nullptr)
{
}

PluginObjectImplWrapper::PluginObjectImplWrapper(PluginObjectImpl* impl)
    : _impl(impl)
{
    _impl->IncRefCount();
}

PluginObjectImplWrapper::PluginObjectImplWrapper(const PluginObjectImplWrapper& other)
{
    _impl = other._impl;
    _impl->IncRefCount();
}

PluginObjectImplWrapper& PluginObjectImplWrapper::operator=(const PluginObjectImplWrapper& other)
{
    if (_impl != other._impl) {
        if (_impl) _impl->DecRefCount();
        _impl = other._impl;
        _impl->IncRefCount();
    }
    return *this;
}

PluginObjectImplWrapper::PluginObjectImplWrapper(PluginObjectImplWrapper&& other)
    : _impl(other._impl)
{
    other._impl = nullptr;
}

PluginObjectImplWrapper& PluginObjectImplWrapper::operator=(PluginObjectImplWrapper&& other)
{
    if (_impl != other._impl) {
        _impl = other._impl;
        other._impl = nullptr;
    }
    return *this;
}

PluginObjectImplWrapper::~PluginObjectImplWrapper()
{
    _impl->DecRefCount();
}

bool PluginObjectImplWrapper::IsImplInitialized() const
{
    return _impl != nullptr;
}

PluginObjectImpl::PluginObjectImpl()
    : __refCount(0)
{
}

void PluginObjectImpl::IncRefCount()
{
    ++__refCount;
}

void PluginObjectImpl::DecRefCount()
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
{
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
