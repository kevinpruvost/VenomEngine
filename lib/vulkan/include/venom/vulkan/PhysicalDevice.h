///
/// Project: VenomEngine
/// @file PhysicalDevice.h
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Instance.h>

namespace venom
{
namespace vulkan
{
class PhysicalDevice
{
public:
    PhysicalDevice();
    ~PhysicalDevice();

    uint64_t GetDeviceLocalVRAMAmount() const;

    void GetDeviceQueue(VkQueue * queuePtr, uint32_t queueFamilyIndex, uint32_t queueIndex) const;
    static const VkPhysicalDevice GetUsedPhysicalDevice();
    static void SetUsedPhysicalDevice(const VkPhysicalDevice * device);
    static std::vector<PhysicalDevice> GetVulkanPhysicalDevices();

public:
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;
};

}
}