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
#include <queue>
#include <map>
#include <unordered_map>

namespace venom
{
namespace common
{
template<typename T>
using Vector = std::vector<T>;
template<typename T>
using Stack = std::stack<T>;
template<typename T>
using Queue = std::queue<T>;
template<typename T, size_t N>
using Array = std::array<T, N>;

template<typename T, size_t N1, size_t N2>
using Array2D = std::array<std::array<T, N2>, N1>;

template<typename T, typename U>
using Map = std::map<T, U>;
template<typename T, typename U>
using UMap = std::unordered_map<T, U>;

template<typename... Args>
using Tuple = std::tuple<Args...>;

template<class... Args>
using MakeTuple = decltype(std::make_tuple(std::declval<Args>()...));
}
}