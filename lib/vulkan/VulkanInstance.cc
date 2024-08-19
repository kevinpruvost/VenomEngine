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

VulkanInstance::VulkanInstance()
    : _vulkanInstance(VK_NULL_HANDLE)
{
}

VulkanInstance::~VulkanInstance()
{
    if (_vulkanInstance != VK_NULL_HANDLE)
        vkDestroyInstance(_vulkanInstance, nullptr);
}

}