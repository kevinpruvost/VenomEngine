///
/// Project: VenomEngineWorkspace
/// @file Callback.h
/// @date Feb, 05 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <functional>

namespace venom
{
namespace common
{
template<typename Return, typename... Args>
using Callback = std::function<Return(Args...)>;
}
}