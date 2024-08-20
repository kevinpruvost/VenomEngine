///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanQueueFamily.h
/// Date: 8/20/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanPhysicalDevice.h"

namespace venom
{
class VulkanQueueFamily
{
public:
    VulkanQueueFamily();
    ~VulkanQueueFamily();

public:

public:
    VkQueueFamilyProperties properties;
};

std::vector<VulkanQueueFamily> getVulkanQueueFamilies(const VulkanPhysicalDevice& physicalDevice);
}