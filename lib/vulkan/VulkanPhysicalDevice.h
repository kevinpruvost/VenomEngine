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
    VulkanPhysicalDevice(const VkPhysicalDevice & device, const VkPhysicalDeviceProperties & properties, const VkPhysicalDeviceFeatures & features);
    ~VulkanPhysicalDevice();

public:
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
};

std::vector<VulkanPhysicalDevice> GetVulkanPhysicalDevices();
}