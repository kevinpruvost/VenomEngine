///
/// Project: VenomEngine
/// @file DLL.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/DLL.h>
#include <venom/common/Log.h>

#include <iostream>

namespace venom::common
{

DLL::DLL()
    : __handle(nullptr)
{
}

DLL * DLL::Create()
{
    return new DLL();
}

DLL* DLL::GetFromCache(const std::string& path)
{
    return DLL_Cache::GetCache()->GetFromCache(path);
}

DLL::~DLL()
{
    if (__handle) {
#ifdef _WIN32
        FreeLibrary((HMODULE)__handle);
#elif __APPLE__
        dlclose(m_handle);
#else // Linux
        dlclose(m_handle);
#endif
    }
}

void * DLL::GetFunction(const char * name)
{
    venom_assert(__handle, "DLL not loaded");
#ifdef _WIN32
    return (void*)GetProcAddress((HMODULE)__handle, (LPCSTR)name);
#elif __APPLE__
    return dlsym(m_handle, name);
#else // Linux
    return dlsym(m_handle, name);
#endif
}

Error DLL::LoadDLL(const char* path)
{
    if (__handle) {
        Log::Error("DLL already loaded: %s\n", path);
        return Error::Failure;
    }

    // Modify Path name to add extension considering platform
    char realPath[256] = {0};
    strcpy(realPath, path);
    strcpy(realPath + strlen(realPath),
#ifdef _WIN32
        ".dll");
    __handle = LoadLibrary(realPath);
#elif __APPLE__
    ".dylib");
    m_handle = dlopen(realPath, RTLD_LAZY);
#else // Linux
    ".so");
    m_handle = dlopen(realPath, RTLD_LAZY);
#endif
    if (!__handle) {
        Log::Error("Failed to load DLL: %s\n", realPath);
        return Error::Failure;
    }
    __path = path;
    DLL_Cache::GetCache()->StoreInCache(__path, this);
    return Error::Success;
}

void DLL::UnloadDLL() const
{
    venom_assert(__handle, "DLL not loaded");
    DLL_Cache::GetCache()->UnloadFromCache(__path);
}

DLL_Cache::DLL_Cache()
{
}

DLL_Cache::~DLL_Cache()
{
}

void DLL_Cache::StoreInCache(const std::string& name, DLL* dll)
{
    __dlls[name] = std::shared_ptr<DLL>(dll);
}

void DLL_Cache::UnloadFromCache(const std::string& name)
{
    venom_assert(__dlls.erase(name) == 1, "DLL not found in cache");
}

DLL* DLL_Cache::GetFromCache(const std::string& name)
{
    if (auto it = __dlls.find(name); it != __dlls.end())
        return it->second.get();
    return nullptr;
}

static DLL_Cache * s_cache = nullptr;
void DLL_Cache::SetCache(DLL_Cache* cache)
{
    venom_assert(s_cache == nullptr, "DLL_Cache already set");
    s_cache = cache;
}

DLL_Cache * DLL_Cache::GetCache()
{
    return s_cache;
}
}
