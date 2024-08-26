///
/// Project: Bazel_Vulkan_Metal
/// @file Singleton.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <cstdlib>
#include <filesystem>

#include "Log.h"

namespace venom
{
namespace common
{
template <typename T>
class Singleton
{
public:
    Singleton() = default;
    virtual ~Singleton() = default;

    static T* GetInstance()
    {
#if __cplusplus >= 201703L
        static T instance;
        return &instance;
#else
        std::filesystem::path path = std::filesystem::current_path();
        Log::Error("version is: %d", __cplusplus);
        abort();
        return nullptr;
#endif
    }
};
}
}
