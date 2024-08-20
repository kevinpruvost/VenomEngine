///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanQueueFamily.cc
/// Date: 8/20/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanQueueFamily.h"

namespace venom
{
VulkanQueueFamily::VulkanQueueFamily()
{
}

VulkanQueueFamily::~VulkanQueueFamily()
{
}

MappedVulkanQueueFamilies::MappedVulkanQueueFamilies()
{
}

MappedVulkanQueueFamilies::MappedVulkanQueueFamilies(std::vector<VulkanQueueFamily>& queueFamilies)
    : __queueFamilies(std::move(queueFamilies)) // giving ownership of queue families
{
    for (int i = 0; i < __queueFamilies.size(); ++i) {
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            __graphicsQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            __computeQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            __transferQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
            __sparseBindingQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_PROTECTED_BIT) {
            __protectedQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) {
            __videoDecodeQueueFamilyIndices.push_back(i);
        }
        if (__queueFamilies[i].properties.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) {
            __videoEncodeQueueFamilyIndices.push_back(i);
        }
    }
}

MappedVulkanQueueFamilies& MappedVulkanQueueFamilies::operator=(MappedVulkanQueueFamilies&& other) noexcept
{
    __queueFamilies = std::move(other.__queueFamilies);
    __graphicsQueueFamilyIndices = std::move(other.__graphicsQueueFamilyIndices);
    __presentQueueFamilyIndices = std::move(other.__presentQueueFamilyIndices);
    __computeQueueFamilyIndices = std::move(other.__computeQueueFamilyIndices);
    __transferQueueFamilyIndices = std::move(other.__transferQueueFamilyIndices);
    __sparseBindingQueueFamilyIndices = std::move(other.__sparseBindingQueueFamilyIndices);
    __protectedQueueFamilyIndices = std::move(other.__protectedQueueFamilyIndices);
    __videoDecodeQueueFamilyIndices = std::move(other.__videoDecodeQueueFamilyIndices);
    __videoEncodeQueueFamilyIndices = std::move(other.__videoEncodeQueueFamilyIndices);
    return *this;
}

MappedVulkanQueueFamilies getVulkanQueueFamilies(const VulkanPhysicalDevice& physicalDevice)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.physicalDevice, &queueFamilyCount, queueFamilies.data());

    std::vector<VulkanQueueFamily> families(queueFamilyCount);
    for (size_t i = 0; i < queueFamilyCount; i++) {
        families[i].properties = queueFamilies[i];
    }
    return MappedVulkanQueueFamilies(families);
}
}
