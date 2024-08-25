///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanInstance.cc
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Instance.h>

namespace venom::vulkan
{

static VkInstance vulkanInstance = VK_NULL_HANDLE;

Instance::Instance()
{
}

Instance::~Instance()
{
    if (vulkanInstance != VK_NULL_HANDLE)
        vkDestroyInstance(vulkanInstance, nullptr);
}

VkInstance & Instance::GetInstance()
{
    return vulkanInstance;
}
}
