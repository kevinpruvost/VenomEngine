///
/// Project: VenomEngine
/// @file VulkanSwapChain.cc
/// @date Aug, 21 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/SwapChain.h>
#include <venom/vulkan/RenderPass.h>

#include <algorithm>

#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

namespace venom::vulkan
{

SwapChain::SwapChain()
    : capabilities()
    , surfaceFormats()
    , presentModes()
    , activeSurfaceFormat{VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
    , activePresentMode(VK_PRESENT_MODE_MAX_ENUM_KHR)
    , extent{0, 0}
    , swapChain(VK_NULL_HANDLE)
    , viewport{}
    , scissor{}
{
}

SwapChain::~SwapChain()
{
    CleanSwapChain();
}

SwapChain::SwapChain(SwapChain&& other)
    : swapChain(other.swapChain)
    , swapChainImageHandles(std::move(other.swapChainImageHandles))
    , __swapChainImageViews(std::move(other.__swapChainImageViews))
    , capabilities(other.capabilities)
    , surfaceFormats(std::move(other.surfaceFormats))
    , presentModes(std::move(other.presentModes))
    , activeSurfaceFormat(other.activeSurfaceFormat)
    , activePresentMode(other.activePresentMode)
    , extent(other.extent)
{
    other.swapChain = VK_NULL_HANDLE;
}

SwapChain& SwapChain::operator=(SwapChain&& other)
{
    if (this == &other) return *this;
    swapChain = other.swapChain;
    swapChainImageHandles = std::move(other.swapChainImageHandles);
    __swapChainImageViews = std::move(other.__swapChainImageViews);
    capabilities = other.capabilities;
    surfaceFormats = std::move(other.surfaceFormats);
    presentModes = std::move(other.presentModes);
    activeSurfaceFormat = other.activeSurfaceFormat;
    activePresentMode = other.activePresentMode;
    extent = other.extent;
    other.swapChain = VK_NULL_HANDLE;
    return *this;
}

void SwapChain::CleanSwapChain()
{
    for (auto & framebuffer : __swapChainFramebuffers) {
        if (framebuffer != VK_NULL_HANDLE)
            vkDestroyFramebuffer(LogicalDevice::GetVkDevice(), framebuffer, Allocator::GetVKAllocationCallbacks());
    }
    __swapChainFramebuffers.clear();

    for (auto & imageView : __swapChainImageViews) {
        if (imageView != VK_NULL_HANDLE)
            vkDestroyImageView(LogicalDevice::GetVkDevice(), imageView, Allocator::GetVKAllocationCallbacks());
    }
    __swapChainImageViews.clear();

    if (swapChain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(LogicalDevice::GetVkDevice(), swapChain, Allocator::GetVKAllocationCallbacks());
        swapChain = VK_NULL_HANDLE;
    }
}

vc::Error SwapChain::InitSwapChainSettings(const PhysicalDevice* physicalDevice, const Surface* surface, const vc::Context* context)
{
    // Get surface capabilities
    if (auto err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->physicalDevice, surface->surface, &capabilities); err != VK_SUCCESS)
    {
        vc::Log::Error("Failed to get physical device surface capabilities: %d", err);
        return vc::Error::InitializationFailed;
    }

    // Get surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->physicalDevice, surface->surface, &formatCount, nullptr);

    if (formatCount != 0) {
        surfaceFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->physicalDevice, surface->surface, &formatCount, surfaceFormats.data());
    }

    // Get present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->physicalDevice, surface->surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->physicalDevice, surface->surface, &presentModeCount, presentModes.data());
    }

    // Default active settings
    // SRGB format because it's the most discernable to the human eye
    for (const auto& format : surfaceFormats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            activeSurfaceFormat = format;
            break;
        }
    }
    // Triple Buffering (Mailbox) if possible, otherwise Double Buffering (FIFO), Single Buffering is Immediate
    for (const auto& presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_FIFO_KHR)
            activePresentMode = presentMode;
        else if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            activePresentMode = presentMode;
            break;
        }
    }
    // Extent, if currentExtent is UINT32_MAX, then the extent can vary, else it's the currentExtent
    if (capabilities.currentExtent.width != UINT32_MAX) {
        extent = capabilities.currentExtent;
    } else {
        // Gets the window size in terms of total pixels, not to confuse with screen coordinates
        // Otherwise we would be using glfwGetWindowSize
        int w, h;
        glfwGetFramebufferSize(const_cast<GLFWwindow*>(context->GetWindow()), &w, &h);

        extent.width = std::clamp<uint32_t>(w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp<uint32_t>(h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }

    // Viewport
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissor is a rectangle that defines the pixels that the rasterizer will use from the framebuffer
    scissor.offset = {0, 0};
    scissor.extent = extent;
    return vc::Error::Success;
}

vc::Error SwapChain::InitSwapChain(const Surface * surface, const vc::Context * context, const MappedQueueFamilies * queueFamilies)
{
    venom_assert(capabilities.maxImageCount > 0, "Swap chain must have at least 1 image");

    // If already created, then clean up first
    if (swapChain != VK_NULL_HANDLE)
        CleanSwapChain();

    // Image count, must be at least the minimum image count, but no more than the maximum image count
    // One more is recommeneded to avoid waiting on the driver
    uint32_t imageCount = std::clamp(capabilities.minImageCount + 1, capabilities.minImageCount, capabilities.maxImageCount);

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = activeSurfaceFormat.format;
    createInfo.imageColorSpace = activeSurfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // Always 1 unless stereoscopic 3D
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (queueFamilies->graphicsQueueFamilyIndices[0] != queueFamilies->presentQueueFamilyIndices[0]) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilies->graphicsQueueFamilyIndices.data();
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    if (capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR == 0) {
        vc::Log::Error("Opaque composite alpha not supported");
        return vc::Error::InitializationFailed;
    }

    createInfo.presentMode = activePresentMode;

    // Clip pixels that are obscured by other windows
    createInfo.clipped = VK_TRUE;

    // Old swap chain
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // Creating SwapChain
    if (vkCreateSwapchainKHR(LogicalDevice::GetVkDevice(), &createInfo, Allocator::GetVKAllocationCallbacks(), &swapChain) != VK_SUCCESS) {
        vc::Log::Error("Failed to create swap chain");
        return vc::Error::InitializationFailed;
    }

    // Getting handles of images in the swap chain
    vkGetSwapchainImagesKHR(LogicalDevice::GetVkDevice(), swapChain, &imageCount, nullptr);
    swapChainImageHandles.resize(imageCount);
    vkGetSwapchainImagesKHR(LogicalDevice::GetVkDevice(), swapChain, &imageCount, swapChainImageHandles.data());

    // Create ImageViews
    __swapChainImageViews.resize(imageCount);
    for (size_t i = 0; i < imageCount; ++i) {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = swapChainImageHandles[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = activeSurfaceFormat.format;
        // No swizzling, swizzling means changing the order of components
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(LogicalDevice::GetVkDevice(), &imageViewCreateInfo, Allocator::GetVKAllocationCallbacks(), &__swapChainImageViews[i]) != VK_SUCCESS) {
            vc::Log::Error("Failed to create image views");
            return vc::Error::InitializationFailed;
        }
    }
    return vc::Error::Success;
}

vc::Error SwapChain::InitSwapChainFramebuffers(const RenderPass* renderPass)
{
    __swapChainFramebuffers.resize(__swapChainImageViews.size());
    for (int i = 0; i < __swapChainImageViews.size(); ++i) {
        VkImageView attachments[] = {
            __swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->GetRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(LogicalDevice::GetVkDevice(), &framebufferInfo, Allocator::GetVKAllocationCallbacks(), &__swapChainFramebuffers[i]) != VK_SUCCESS) {
            vc::Log::Error("Failed to create framebuffer");
            return vc::Error::InitializationFailed;
        }
    }
    return vc::Error::Success;
}
}
