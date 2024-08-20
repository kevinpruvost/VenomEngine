///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanQueueFamily.cc
/// Date: 8/20/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanQueueFamily.h"

namespace venom
{
VulkanQueueFamily::VulkanQueueFamily()
{
}

VulkanQueueFamily::~VulkanQueueFamily()
{
}

std::vector<VulkanQueueFamily> getVulkanQueueFamilies(const VulkanPhysicalDevice& physicalDevice)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.physicalDevice, &queueFamilyCount, queueFamilies.data());

    std::vector<VulkanQueueFamily> families(queueFamilyCount);
    for (size_t i = 0; i < queueFamilyCount; i++) {
        families[i].properties = queueFamilies[i];
    }
    return families;
}
}
