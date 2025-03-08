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
#include <venom/vulkan/Framebuffer.h>

#include <venom/vulkan/plugin/graphics/Texture.h>

#include <venom/common/Context.h>
#include <venom/common/Containers.h>

namespace venom
{
namespace vulkan
{
class VulkanRenderPass;

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

    vc::Error InitSwapChainSettings(const Surface* surface);

    inline int GetSamples() const { return __samples; }
    inline void SetSamples(const int samples) { __samples = samples; }
    inline const vc::Vector<ImageView> & GetSwapChainImageViews() const { return __swapChainImageViews; }

    static SwapChain * Get();

    /// @brief Inits Swap chain and swap chain image views
    /// @return Error
    vc::Error InitSwapChain();

public:
    const Surface * surface;
    VkSurfaceFormatKHR activeSurfaceFormat;
    VkPresentModeKHR activePresentMode;
    VkExtent2D extent;
    VkViewport viewport;
    VkRect2D scissor;
    VkSwapchainKHR swapChain;
    vc::Vector<VkImage> swapChainImageHandles;

private:
    vc::Vector<ImageView> __swapChainImageViews;
    vc::Vector<Image> __swapChainImages;

    int __samples;

    friend class VulkanRenderPass;
};
}
}
