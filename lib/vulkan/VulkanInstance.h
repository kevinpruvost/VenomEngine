///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanInstance.h
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <common/Log.h>

namespace venom
{
class VulkanInstance
{
public:
    VulkanInstance();
    ~VulkanInstance();

protected:
    VkInstance _vulkanInstance;
};
}
