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
    : physicalDevice(VK_NULL_HANDLE)
    , properties()
    , features()
    , memoryProperties()
{
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
}

uint64_t VulkanPhysicalDevice::GetDeviceLocalVRAM() const
{
    uint64_t totalVRAM = 0;
    for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; i++) {
        if (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            totalVRAM += memoryProperties.memoryHeaps[i].size;
        }
    }
    return totalVRAM;
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
        physicalDevices[i].physicalDevice = vkPhysicalDevices[i];
        vkGetPhysicalDeviceProperties(vkPhysicalDevices[i], &physicalDevices[i].properties);
        vkGetPhysicalDeviceFeatures(vkPhysicalDevices[i], &physicalDevices[i].features);
        vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevices[i], &physicalDevices[i].memoryProperties);
    }
    return physicalDevices;
}
}