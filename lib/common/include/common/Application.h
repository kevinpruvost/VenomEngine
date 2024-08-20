///
/// Project: Bazel_Vulkan_Metal
/// File: Application.h
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <common/Error.h>

namespace venom
{
class VENOM_COMMON_API ApplicationBackend
{
public:
    ApplicationBackend();
    virtual Error Run() = 0;
};

class VENOM_COMMON_API Application
{
public:
    Application();
    ~Application();

    Error Run();

private:
    ApplicationBackend * backend_;

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