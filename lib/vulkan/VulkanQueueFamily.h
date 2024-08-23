///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanQueueFamily.h
/// Date: 8/20/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanSurface.h"

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
typedef uint32_t VulkanQueueFamilyIndex;
typedef std::vector<uint32_t> VulkanQueueFamilyIndices;

class MappedVulkanQueueFamilies
{
public:
    MappedVulkanQueueFamilies();

    explicit MappedVulkanQueueFamilies(std::vector<VulkanQueueFamily>& queueFamilies);
    MappedVulkanQueueFamilies & operator=(MappedVulkanQueueFamilies&& other) noexcept;
    const std::vector<VulkanQueueFamily> GetQueueFamilies() const;
    Error SetPresentQueueFamilyIndices(const VulkanPhysicalDevice& physicalDevice, const VulkanSurface & surface);

public:
    VulkanQueueFamilyIndices graphicsQueueFamilyIndices,
    computeQueueFamilyIndices, transferQueueFamilyIndices, sparseBindingQueueFamilyIndices,
    protectedQueueFamilyIndices, videoDecodeQueueFamilyIndices,
    videoEncodeQueueFamilyIndices;
    // To set considering the utilized surface
    VulkanQueueFamilyIndices presentQueueFamilyIndices;

private:
    std::vector<VulkanQueueFamily> __queueFamilies;
};

MappedVulkanQueueFamilies getVulkanQueueFamilies(const VulkanPhysicalDevice& physicalDevice);
}
