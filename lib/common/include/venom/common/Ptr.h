///
/// Project: VenomEngineWorkspace
/// @file Ptr.h
/// @date Oct, 23 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <memory>

namespace venom
{
namespace common
{
// Weak pointer alias
template <typename T> using WPtr = std::weak_ptr<T>;
// Shared pointer alias
template <typename T> using SPtr = std::shared_ptr<T>;
// Unique pointer alias
template <typename T> using UPtr = std::unique_ptr<T>;
}
}