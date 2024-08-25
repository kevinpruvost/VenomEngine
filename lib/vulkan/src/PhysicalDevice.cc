///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanPhysicalDevice.cc
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/PhysicalDevice.h>

namespace venom::vulkan
{
PhysicalDevice::PhysicalDevice()
    : physicalDevice(VK_NULL_HANDLE)
    , logicalDevice(VK_NULL_HANDLE)
    , properties()
    , features()
    , memoryProperties()
{
}

PhysicalDevice::~PhysicalDevice()
{
    if (logicalDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(logicalDevice, nullptr);
    }
}

uint64_t PhysicalDevice::GetDeviceLocalVRAMAmount() const
{
    uint64_t totalVRAM = 0;
    for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; i++) {
        if (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            totalVRAM += memoryProperties.memoryHeaps[i].size;
        }
    }
    return totalVRAM;
}

void PhysicalDevice::GetDeviceQueue(VkQueue* queuePtr, uint32_t queueFamilyIndex, uint32_t queueIndex) const
{
    venom_assert(logicalDevice != VK_NULL_HANDLE, "Logical device is not initialized");
    vkGetDeviceQueue(logicalDevice, queueFamilyIndex, queueIndex, queuePtr);
}

std::vector<PhysicalDevice> GetVulkanPhysicalDevices()
{
    uint32_t deviceCount = 0;
    std::vector<PhysicalDevice> physicalDevices;

    vkEnumeratePhysicalDevices(Instance::GetInstance(), &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        Log::Error("Failed to find GPUs with Vulkan support");
        return physicalDevices;
    }

    physicalDevices.resize(deviceCount);
    std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(Instance::GetInstance(), &deviceCount, vkPhysicalDevices.data());
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