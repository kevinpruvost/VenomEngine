///
/// Project: VenomEngine
/// @file PhysicalDevice.h
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Instance.h>

#include "venom/common/Containers.h"

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
    static VkFormat FindSupportedFormat(const std::vector<VkFormat> & candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    static VkFormat FindDepthFormat();
    static VkPhysicalDevice GetUsedVkPhysicalDevice();
    static const PhysicalDevice & GetUsedPhysicalDevice();
    static void SetUsedPhysicalDevice(const PhysicalDevice * device);
    static std::vector<PhysicalDevice> GetVulkanPhysicalDevices();

    const VkPhysicalDevice & GetVkPhysicalDevice() const { return __physicalDevice; }
    const VkPhysicalDeviceProperties & GetProperties() const { return __properties; }
    const VkPhysicalDeviceFeatures & GetFeatures() const { return __features; }
    const VkPhysicalDeviceMemoryProperties & GetMemoryProperties() const { return __memoryProperties; }
    const VkFormatProperties & GetFormatProperties() const { return __formatProperties; }

private:
    VkFormatProperties __formatProperties;
    VkPhysicalDevice __physicalDevice;
    VkPhysicalDeviceProperties __properties;
    VkPhysicalDeviceFeatures __features;
    VkPhysicalDeviceMemoryProperties __memoryProperties;
};

}
}
