///
/// Project: VenomEngine
/// @file VulkanPhysicalDevice.cc
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/PhysicalDevice.h>
#include <venom/vulkan/LogicalDevice.h>

namespace venom::vulkan
{
PhysicalDevice::PhysicalDevice()
    : __physicalDevice(VK_NULL_HANDLE)
    , __properties()
    , __features()
    , __memoryProperties()
{
}

PhysicalDevice::~PhysicalDevice()
{
}

uint64_t PhysicalDevice::GetDeviceLocalVRAMAmount() const
{
    uint64_t totalVRAM = 0;
    for (uint32_t i = 0; i < __memoryProperties.memoryHeapCount; i++) {
        if (__memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            totalVRAM += __memoryProperties.memoryHeaps[i].size;
        }
    }
    return totalVRAM;
}

void PhysicalDevice::GetDeviceQueue(VkQueue* queuePtr, uint32_t queueFamilyIndex, uint32_t queueIndex) const
{
    vkGetDeviceQueue(LogicalDevice::GetVkDevice(), queueFamilyIndex, queueIndex, queuePtr);
}

static const PhysicalDevice * s_usedPhysicalDevice = nullptr;
VkPhysicalDevice PhysicalDevice::GetUsedVkPhysicalDevice()
{
    venom_assert(s_usedPhysicalDevice != VK_NULL_HANDLE, "No physical device has been set");
    return s_usedPhysicalDevice->__physicalDevice;
}

const PhysicalDevice & PhysicalDevice::GetUsedPhysicalDevice()
{
    return *s_usedPhysicalDevice;
}

void PhysicalDevice::SetUsedPhysicalDevice(const PhysicalDevice * device)
{
    s_usedPhysicalDevice = device;
}

std::vector<PhysicalDevice> PhysicalDevice::GetVulkanPhysicalDevices()
{
    uint32_t deviceCount = 0;
    std::vector<PhysicalDevice> physicalDevices;

    vkEnumeratePhysicalDevices(Instance::GetVkInstance(), &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        vc::Log::Error("Failed to find GPUs with Vulkan support");
        return physicalDevices;
    }

    physicalDevices.resize(deviceCount);
    std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(Instance::GetVkInstance(), &deviceCount, vkPhysicalDevices.data());
    for (uint32_t i = 0; i < deviceCount; i++)
    {
        physicalDevices[i].__physicalDevice = vkPhysicalDevices[i];
        vkGetPhysicalDeviceProperties(vkPhysicalDevices[i], &physicalDevices[i].__properties);
        vkGetPhysicalDeviceFeatures(vkPhysicalDevices[i], &physicalDevices[i].__features);
        vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevices[i], &physicalDevices[i].__memoryProperties);
    }
    return physicalDevices;
}

const VkPhysicalDevice & PhysicalDevice::GetVkPhysicalDevice() const
{
    return __physicalDevice;
}

const VkPhysicalDeviceProperties& PhysicalDevice::GetProperties() const
{
    return __properties;
}

const VkPhysicalDeviceFeatures& PhysicalDevice::GetFeatures() const
{
    return __features;
}

const VkPhysicalDeviceMemoryProperties& PhysicalDevice::GetMemoryProperties() const
{
    return __memoryProperties;
}
}
