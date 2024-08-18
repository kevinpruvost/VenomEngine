///
/// Project: Bazel_Vulkan_Metal
/// File: vulkan.h
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <vulkan/vulkan.hpp>
#include <common/Application.h>
#include <common/Context.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace venom
{

class VulkanApplication : public ApplicationBackend
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