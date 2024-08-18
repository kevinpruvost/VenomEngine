///
/// Project: Bazel_Vulkan_Metal
/// File: Application.cc
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <common/Application.h>
#include <common/DLL.h>

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
    return backend_->run();
}

Error Application::LoadApi(const ApiType apiType)
{
    DLL * dll = nullptr;
    switch (apiType)
    {
    case ApiType::Vulkan:
        dll = new DLL("VenomVulkan");
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
    backend_ = createApplicationFunc();
    return Error::Success;
}

}
