///
/// Project: VenomEngine
/// @file SwapChain.h
/// @date Aug, 21 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/PhysicalDevice.h>
#include <venom/vulkan/Surface.h>
#include <venom/vulkan/QueueFamily.h>
#include <venom/vulkan/ImageView.h>
#include <venom/vulkan/plugin/graphics/Texture.h>

#include <venom/common/Context.h>

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

    vc::Error InitSwapChainSettings(const PhysicalDevice* physicalDevice, const Surface* surface,
                                    const vc::Context* context);

    /// @brief Inits Swap chain and swap chain image views
    /// @param physicalDevice 
    /// @param surface 
    /// @param context
    /// @param queueFamilies 
    /// @return Error
    vc::Error InitSwapChain(const Surface* surface, const vc::Context* context,
                            const MappedQueueFamilies* queueFamilies);
    vc::Error InitSwapChainFramebuffers(const RenderPass* renderPass);

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
    std::vector<ImageView> __swapChainImageViews;
    VulkanTexture * __depthTexture;
    std::vector<VkFramebuffer> __swapChainFramebuffers;

    friend class RenderPass;
};
}
}
