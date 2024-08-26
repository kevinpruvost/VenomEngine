///
/// Project: VenomEngine
/// @file DLL.h
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
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

#include <memory>
#include <venom/common/Export.h>
#include <venom/common/Error.h>

#include <unordered_map>

namespace venom
{
namespace common
{
/// @brief Contains a shared library handle and provides a way to get symbols from it, can only be created through DLL::Create()
class VENOM_COMMON_API DLL
{
private:
    explicit DLL();
public:
    /// @brief Forces user to use dynamic allocation
    /// @return new DLL
    static DLL * Create();
    /// @brief Searched for DLL in cache, if not found creates a new one
    /// @return existing or new DLL
    static DLL * GetFromCache(const std::string & path);
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

    Error LoadDLL(const char * path);
    inline Error ReloadDLL(const char * path) { return LoadDLL(path); }
    void UnloadDLL() const;

private:
    void * __handle;
    std::string __path;
};

/// @brief Holds all DLLs loaded in memory unless explicitly unloaded/deleted.
class VENOM_COMMON_API DLL_Cache
{
public:
    DLL_Cache();
    ~DLL_Cache();

    void StoreInCache(const std::string & name, DLL * dll);
    void UnloadFromCache(const std::string & name);
    DLL * GetFromCache(const std::string & name);

    static DLL_Cache * GetInstance();

private:
    std::unordered_map<std::string, std::shared_ptr<DLL>> __dlls;
};
}
}
