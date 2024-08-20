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

class MappedVulkanQueueFamilies
{
public:
    MappedVulkanQueueFamilies();

    explicit MappedVulkanQueueFamilies(std::vector<VulkanQueueFamily>& queueFamilies);
    MappedVulkanQueueFamilies & operator=(MappedVulkanQueueFamilies&& other) noexcept;

public:
    std::vector<uint32_t> graphicsQueueFamilyIndices,
    presentQueueFamilyIndices, computeQueueFamilyIndices,
    transferQueueFamilyIndices, sparseBindingQueueFamilyIndices,
    protectedQueueFamilyIndices, videoDecodeQueueFamilyIndices,
    videoEncodeQueueFamilyIndices;

private:
    std::vector<VulkanQueueFamily> __queueFamilies;
};

MappedVulkanQueueFamilies getVulkanQueueFamilies(const VulkanPhysicalDevice& physicalDevice);
}