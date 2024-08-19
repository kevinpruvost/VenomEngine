///
/// Project: Bazel_Vulkan_Metal
/// File: vulkan.h
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanDebug.h"

#include <common/Application.h>
#include <common/Context.h>

namespace venom
{

class VulkanApplication : public ApplicationBackend
#ifdef VENOM_DEBUG
    , public VulkanDebugApplication
#endif
{
public:
    VulkanApplication();
    ~VulkanApplication();
    Error run() override;

private:
    Error __loop();
    Error __initVulkan();

    Error __createInstance();

    VkInstance __instance;
    Context __context;
};
}

extern "C" EXPORT venom::ApplicationBackend* createApplication();