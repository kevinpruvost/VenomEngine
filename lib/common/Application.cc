///
/// Project: Bazel_Vulkan_Metal
/// File: Application.cc
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "include/common/Application.h"
#include "include/common/DLL.h"

#include <iostream>

namespace venom
{

ApplicationBackend::ApplicationBackend()
{
}

Application::Application()
{
}

Application::~Application()
{
}

Error Application::run()
{
    return m_backend->run();
}

Error Application::LoadApi(const ApiType apiType)
{
    DLL * dll = nullptr;
    switch (apiType)
    {
    case ApiType::Vulkan:
        dll = new DLL("lib/vulkan/venom_vulkan_dll");
        break;
    default:
        std::cerr << "Unsupported API type" << std::endl;
        return Error::Failure;
    };
    if (!dll) {
        return Error::Failure;
    }
    void * data = dll->GetFunction("createApplication");
    ApplicationBackend * (*createApplicationFunc)() = dll->GetFunction<ApplicationBackend*>("createApplication");
    if (!createApplicationFunc)
    {
        std::cerr << "Failed to load 'createApplication' function" << std::endl;
        std::cerr << "data = " << data << std::endl;
        return Error::Failure;
    }
    m_backend = createApplicationFunc();
    return Error::Success;
}

}
