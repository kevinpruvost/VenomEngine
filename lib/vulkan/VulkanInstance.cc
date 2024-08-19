///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanInstance.cc
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanInstance.h"

namespace venom
{

static VkInstance vulkanInstance = VK_NULL_HANDLE;

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
    if (vulkanInstance != VK_NULL_HANDLE)
        vkDestroyInstance(vulkanInstance, nullptr);
}

VkInstance & VulkanInstance::GetInstance()
{
    return vulkanInstance;
}
}
