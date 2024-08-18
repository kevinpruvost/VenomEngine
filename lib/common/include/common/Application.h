///
/// Project: Bazel_Vulkan_Metal
/// File: Application.h
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <common/Error.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif __APPLE__
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#endif

#ifdef VENOM_COMMON_EXPORTS
#define APPLICATION_API EXPORT
#else
#define APPLICATION_API IMPORT
#endif

namespace venom
{
class APPLICATION_API ApplicationBackend
{
public:
    ApplicationBackend();
    virtual Error run() = 0;
};

class APPLICATION_API Application
{
public:
    Application();
    ~Application();

    Error run();

private:
    ApplicationBackend * m_backend;

public:
    enum class ApiType
    {
        Vulkan,
        Metal
    };
    Error LoadApi(const ApiType apiType);
    inline Error ReloadApi(const ApiType apiType) { return LoadApi(apiType); }
};
}