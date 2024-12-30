///
/// Project: VenomEngineWorkspace
/// @file Functional.h
/// @date Dec, 30 2024
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
using Function = std::function<Return(Args...)>;
}
}