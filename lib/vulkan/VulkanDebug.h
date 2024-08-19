///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanDebug.h
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <common/Log.h>

namespace venom
{
class VulkanDebugApplication
{
public:
    VulkanDebugApplication();
    ~VulkanDebugApplication();

    Error initDebug();
    void destroyDebug();

private:
    Error __initValidationLayers();
    VkInstance __instance;
};
}