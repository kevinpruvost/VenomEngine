///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanPhysicalDevice.cc
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanPhysicalDevice.h"

namespace venom
{
VulkanPhysicalDevice::VulkanPhysicalDevice()
    : physicalDevice(VK_NULL_HANDLE), properties(), features()
{
}

VulkanPhysicalDevice::VulkanPhysicalDevice(const VkPhysicalDevice& device,
                                           const VkPhysicalDeviceProperties& properties,
                                           const VkPhysicalDeviceFeatures& features)
        : physicalDevice(device), properties(properties), features(features)
{
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
}

std::vector<VulkanPhysicalDevice> GetVulkanPhysicalDevices()
{
    uint32_t deviceCount = 0;
    std::vector<VulkanPhysicalDevice> physicalDevices;

    vkEnumeratePhysicalDevices(VulkanInstance::GetInstance(), &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        Log::Error("Failed to find GPUs with Vulkan support");
        return physicalDevices;
    }

    physicalDevices.resize(deviceCount);
    std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(VulkanInstance::GetInstance(), &deviceCount, vkPhysicalDevices.data());
    for (uint32_t i = 0; i < deviceCount; i++)
    {
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceProperties(vkPhysicalDevices[i], &properties);
        vkGetPhysicalDeviceFeatures(vkPhysicalDevices[i], &features);
        auto ret = physicalDevices.emplace_back(vkPhysicalDevices[i], properties, features);

    }
}
}