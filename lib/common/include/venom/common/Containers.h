///
/// Project: VenomEngineWorkspace
/// @file Containers.h
/// @date Oct, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <vector>
#include <stack>

namespace venom
{
namespace common
{
template<typename T>
using Vector = std::vector<T>;
template<typename T>
using Stack = std::stack<T>;
}
}