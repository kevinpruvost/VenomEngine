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
#include "VulkanRenderPass.h"

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
    
    /// @brief Inits Swap chain and swap chain image views
    /// @param physicalDevice 
    /// @param surface 
    /// @param context
    /// @param queueFamilies 
    /// @return Error
    Error InitSwapChain(const VulkanPhysicalDevice * physicalDevice, const VulkanSurface * surface,
        const Context * context, const MappedVulkanQueueFamilies * queueFamilies);
    Error InitSwapChainFramebuffers(const VulkanRenderPass * renderPass);

public:
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR> presentModes;
    VkSurfaceFormatKHR activeSurfaceFormat;
    VkPresentModeKHR activePresentMode;
    VkExtent2D extent;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImageHandles;

private:
    std::vector<VkImageView> __swapChainImageViews;
    std::vector<VkFramebuffer> __swapChainFramebuffers;

private:
    // For destruction
    VkDevice __logicalDevice;
};
}
