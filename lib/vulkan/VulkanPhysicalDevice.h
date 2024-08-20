///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanPhysicalDevice.h
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanInstance.h"

namespace venom
{
class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice();
    ~VulkanPhysicalDevice();

    uint64_t GetDeviceLocalVRAM() const;

public:
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;
};

std::vector<VulkanPhysicalDevice> GetVulkanPhysicalDevices();
}