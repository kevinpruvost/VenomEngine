///
/// Project: Bazel_Vulkan_Metal
/// File: DLL.cc
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <common/DLL.h>
#include <common/Log.h>

#include <iostream>

namespace venom
{

DLL::DLL(const char * path)
    : handle_(nullptr)
{
// Modify Path name to add extension considering platform
    char realPath[256] = {0};
    strcpy(realPath, path);
    strcpy(realPath + strlen(realPath),
#ifdef _WIN32
        ".dll");
    handle_ = LoadLibrary(realPath);
#elif __APPLE__
        ".dylib");
    m_handle = dlopen(realPath, RTLD_LAZY);
#else // Linux
        ".so");
    m_handle = dlopen(realPath, RTLD_LAZY);
#endif
    if (!handle_)
    {
        Log::Error("Failed to load DLL: %s\n", realPath);
    }
}

DLL::~DLL()
{
    if (handle_)
    {
#ifdef _WIN32
        FreeLibrary((HMODULE)handle_);
#elif __APPLE__
        dlclose(m_handle);
#else // Linux
        dlclose(m_handle);
#endif
    }
}

void * DLL::GetFunction(const char * name)
{
    if (!handle_) return nullptr;
#ifdef _WIN32
    return (void*)GetProcAddress((HMODULE)handle_, (LPCSTR)name);
#elif __APPLE__
    return dlsym(m_handle, name);
#else // Linux
    return dlsym(m_handle, name);
#endif
}
}