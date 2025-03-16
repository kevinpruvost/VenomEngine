///
/// Project: VenomEngineWorkspace
/// @file Ptr.h
/// @date Oct, 23 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <memory>
#include <venom/common/plugin/PluginObject.h>


namespace venom
{
namespace common
{
class PluginObject;
// Helper template to block types inheriting from PluginObject
// This is to avoid delete issues when using shared/unique pointers
template <typename T>
concept NotPluginObject = !std::is_convertible_v<T *, PluginObject *>;

// Modified Weak pointer alias
template <NotPluginObject T>
using WPtr = std::weak_ptr<T>;

// Modified Shared pointer alias
template <NotPluginObject T>
using SPtr = std::shared_ptr<T>;

// Modified Unique pointer alias
template <NotPluginObject T>
using UPtr = std::unique_ptr<T>;

}
}
