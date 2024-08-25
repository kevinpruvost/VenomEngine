///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanSwapChain.h
/// Date: 8/21/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/PhysicalDevice.h>
#include <venom/vulkan/Surface.h>
#include <venom/vulkan/QueueFamily.h>

#include <common/Context.h>

namespace venom
{
namespace vulkan
{
class RenderPass;
class SwapChain
{
public:
    SwapChain();
    ~SwapChain();
    SwapChain(const SwapChain&) = delete;
    SwapChain& operator=(const SwapChain&) = delete;
    SwapChain(SwapChain&&);
    SwapChain& operator=(SwapChain&&);

    void CleanSwapChain();

    Error InitSwapChainSettings(const PhysicalDevice * physicalDevice, const Surface * surface, const Context * context);
    
    /// @brief Inits Swap chain and swap chain image views
    /// @param physicalDevice 
    /// @param surface 
    /// @param context
    /// @param queueFamilies 
    /// @return Error
    Error InitSwapChain(const PhysicalDevice * physicalDevice, const Surface * surface,
        const Context * context, const MappedQueueFamilies * queueFamilies);
    Error InitSwapChainFramebuffers(const RenderPass * renderPass);

public:
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR> presentModes;
    VkSurfaceFormatKHR activeSurfaceFormat;
    VkPresentModeKHR activePresentMode;
    VkExtent2D extent;
    VkViewport viewport;
    VkRect2D scissor;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImageHandles;

private:
    std::vector<VkImageView> __swapChainImageViews;
    std::vector<VkFramebuffer> __swapChainFramebuffers;

    friend class RenderPass;

private:
    // For destruction
    VkDevice __logicalDevice;
};
}
}