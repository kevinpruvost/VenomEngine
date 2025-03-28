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
#include <set>

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
template<class T, size_t N>
using Array = std::array<T, N>;
template<typename T, typename Compare = std::less<T>>
using Set = std::set<T, Compare>;

template<typename T, size_t N1, size_t N2>
using Array2D = std::array<std::array<T, N2>, N1>;

template<typename T, typename U, typename Compare = std::less<T>>
using Map = std::map<T, U, Compare>;
template<typename T, typename U>
using UMap = std::unordered_map<T, U>;

template<typename... Args>
using Tuple = std::tuple<Args...>;

template<class... Args>
using MakeTuple = decltype(std::make_tuple(std::declval<Args>()...));

template<typename T>
class FastVector {
public:
    FastVector(size_t size)
        : _size(size), _data(static_cast<T*>(::operator new(size * sizeof(T)))) {}

    FastVector()
        : _size(0), _data(nullptr) {}

    ~FastVector() {
        ::operator delete(_data);
    }

    inline T& operator[](size_t index) {
        if (index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    inline const T& operator[](size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    inline size_t size() const {
        return _size;
    }

    inline void resize(size_t size) {
        _size = size;
        if (_data) ::operator delete(_data);
        _data = static_cast<T*>(::operator new(size * sizeof(T)));
    }

    T * data() {
        return _data;
    }

    inline void clear() {
        if (_data) ::operator delete(_data);
        _data = nullptr;
        _size = 0;
    }

private:
    size_t _size;
    T* _data;
};
}
}