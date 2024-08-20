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

private:
    std::vector<VulkanQueueFamily> __queueFamilies;
    std::vector<uint32_t> __graphicsQueueFamilyIndices,
    __presentQueueFamilyIndices, __computeQueueFamilyIndices,
    __transferQueueFamilyIndices, __sparseBindingQueueFamilyIndices,
    __protectedQueueFamilyIndices, __videoDecodeQueueFamilyIndices,
    __videoEncodeQueueFamilyIndices;

};

MappedVulkanQueueFamilies getVulkanQueueFamilies(const VulkanPhysicalDevice& physicalDevice);
}