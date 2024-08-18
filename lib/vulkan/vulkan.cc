///
/// Project: Bazel_Vulkan_Metal
/// File: vulkan.cc
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "vulkan.h"

venom::Error VulkanApplication::run()
{
    printf("Hello, Vulkan!\n");
    return venom::Error::Success;
}

extern "C" venom::ApplicationBackend* createApplication()
{
    return new VulkanApplication();
}