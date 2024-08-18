///
/// Project: Bazel_Vulkan_Metal
/// File: DLL.cc
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "include/common/DLL.h"

#include <iostream>

namespace venom
{

DLL::DLL(const char * path)
    : m_handle(nullptr)
{
// Modify Path name to add extension considering platform
    char realPath[256] = {0};
    strcpy(realPath, path);
    strcpy(realPath + strlen(realPath),
#ifdef _WIN32
        ".dll");
    m_handle = LoadLibrary(realPath);
#elif __APPLE__
        ".dylib");
    m_handle = dlopen(realPath, RTLD_LAZY);
#else // Linux
        ".so");
    m_handle = dlopen(realPath, RTLD_LAZY);
#endif
    if (!m_handle)
    {
        std::cerr << "Failed to load DLL: " << realPath << std::endl;
    }
}

DLL::~DLL()
{
    if (m_handle)
    {
#ifdef _WIN32
        FreeLibrary((HMODULE)m_handle);
#elif __APPLE__
        dlclose(m_handle);
#else // Linux
        dlclose(m_handle);
#endif
    }
}

void * DLL::GetFunction(const char * name)
{
    if (!m_handle) return nullptr;
#ifdef _WIN32
    return (void*)GetProcAddress((HMODULE)m_handle, (LPCSTR)name);
#elif __APPLE__
    return dlsym(m_handle, name);
#else // Linux
    return dlsym(m_handle, name);
#endif
}
}