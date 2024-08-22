///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanSwapChain.h
/// Date: 8/21/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanSurface.h"
#include "VulkanQueueFamily.h"

#include <common/Context.h>

namespace venom
{
class VulkanSwapChain
{
public:
    VulkanSwapChain();
    ~VulkanSwapChain();
    VulkanSwapChain(const VulkanSwapChain&) = delete;
    VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;
    VulkanSwapChain(VulkanSwapChain&&);
    VulkanSwapChain& operator=(VulkanSwapChain&&);

    Error InitSwapChainSettings(const VulkanPhysicalDevice * physicalDevice, const VulkanSurface * surface,
        const Context * context, const MappedVulkanQueueFamilies * queueFamilies);
    Error InitSwapChain(const VulkanPhysicalDevice * physicalDevice, const VulkanSurface * surface,
        const Context * context, const MappedVulkanQueueFamilies * queueFamilies);

public:
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR> presentModes;
    VkSurfaceFormatKHR activeSurfaceFormat;
    VkPresentModeKHR activePresentMode;
    VkExtent2D extent;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImageHandles;
    std::vector<VkImageView> swapChainImageViews;

private:
    // For destruction
    VkDevice __logicalDevice;
};
}
