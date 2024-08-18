///
/// Project: Bazel_Vulkan_Metal
/// File: DLL.h
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

// Load DLL then create application
#ifdef _WIN32
#include <Windows.h>
#elif __APPLE__
#include <dlfcn.h>
#else // Linux
#include <dlfcn.h>
#endif

namespace venom
{
/// @brief Contains a shared library handle and provides a way to get symbols from it
class DLL
{
public:
    explicit DLL(const char* path);
    ~DLL();

    /// @brief Gets a function from the DLL
    /// @param name function name
    /// @return sym address
    void * GetFunction(const char * name);

    /// @brief Gets a function from the DLL and casts it to the correct type
    /// @tparam ReturnType return type of the function
    /// @tparam Args types of each args
    /// @param name function name
    /// @return sym address in the form ReturnType(*)(Args...)
    template<typename ReturnType, typename... Args>
    ReturnType(*GetFunction(const char * name))(Args...) { return reinterpret_cast<ReturnType(*)(Args...)>(GetFunction(name)); }

private:
    void * m_handle;
};
}
