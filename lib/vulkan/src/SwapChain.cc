///
/// Project: VenomEngine
/// @file VulkanSwapChain.cc
/// @date Aug, 21 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/SwapChain.h>
#include <venom/vulkan/plugin/graphics/RenderPass.h>

#include <algorithm>

#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/QueueManager.h>

#include "venom/common/plugin/graphics/GraphicsSettings.h"
#include <venom/common/context/ContextApple.h>
#include <venom/common/Config.h>

namespace venom::vulkan
{
static SwapChain * s_swapChain = nullptr;

SwapChain::SwapChain()
    : surface(nullptr)
    , activeSurfaceFormat{VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
    , activePresentMode(VK_PRESENT_MODE_MAX_ENUM_KHR)
    , extent{0, 0}
    , swapChain(VK_NULL_HANDLE)
    , viewport{}
    , scissor{}
    , __samples(1)
{
    venom_assert(s_swapChain == nullptr, "SwapChain::SwapChain() : SwapChain already exists");
    s_swapChain = this;
}

SwapChain::~SwapChain()
{
    CleanSwapChain();
    if (s_swapChain == this) s_swapChain = nullptr;
}

SwapChain::SwapChain(SwapChain&& other)
    : swapChain(other.swapChain)
    , swapChainImageHandles(std::move(other.swapChainImageHandles))
    , __swapChainImageViews(std::move(other.__swapChainImageViews))
    , surface(std::move(other.surface))
    , activeSurfaceFormat(other.activeSurfaceFormat)
    , activePresentMode(other.activePresentMode)
    , extent(other.extent)
{
    other.swapChain = VK_NULL_HANDLE;
}

SwapChain& SwapChain::operator=(SwapChain&& other)
{
    if (this != &other) {
        swapChain = other.swapChain;
        swapChainImageHandles = std::move(other.swapChainImageHandles);
        __swapChainImageViews = std::move(other.__swapChainImageViews);
        surface = std::move(other.surface);
        activeSurfaceFormat = other.activeSurfaceFormat;
        activePresentMode = other.activePresentMode;
        extent = other.extent;
        other.swapChain = VK_NULL_HANDLE;
    }
    return *this;
}

void SwapChain::CleanSwapChain()
{
    __swapChainImageViews.clear();

    if (swapChain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(LogicalDevice::GetVkDevice(), swapChain, Allocator::GetVKAllocationCallbacks());
        swapChain = VK_NULL_HANDLE;
    }
}

vc::Error SwapChain::InitSwapChainSettings(const Surface* s)
{
    surface = s;
    const auto & capabilities = surface->GetCapabilities();
    const auto & surfaceFormats = surface->GetSurfaceFormats();
    const auto & presentModes = surface->GetPresentModes();

    // Default active settings
    // SRGB format because it's the most discernible to the human eye
    for (const auto& format : surfaceFormats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            activeSurfaceFormat = format;
            if (!vc::GraphicsSettings::IsHDREnabled()) break;
        }
        // TODO: HDR Textures
        else if (format.format == VK_FORMAT_R16G16B16A16_SFLOAT && format.colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT) {
            activeSurfaceFormat = format;
            break;
        }
    }
    // Triple Buffering (Mailbox) if possible, otherwise Double Buffering (FIFO), Single Buffering is Immediate
    for (const auto& presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
            activePresentMode = presentMode;
        if (presentMode == VK_PRESENT_MODE_FIFO_KHR) {
            activePresentMode = presentMode;
            break;
        }
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            activePresentMode = presentMode;
        }
    }
    // Extent, if currentExtent is std::numeric_limits<uint32_t>::max(), then the extent can vary, else it's the currentExtent
    activePresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        //extent.width = capabilities.currentExtent.width / vc::Context::GetWindowScale();
        //extent.height = capabilities.currentExtent.height / vc::Context::GetWindowScale();
        extent.width = std::min(capabilities.currentExtent.width, static_cast<uint32_t>(vc::Context::GetWindowWidth()));
        extent.height = std::min(capabilities.currentExtent.height, static_cast<uint32_t>(vc::Context::GetWindowHeight()));

#if defined(VENOM_PLATFORM_MACOS)
        if (IS_CONTEXT_TYPE(Apple)) {
            // Weird workaround for how bad ImGui handles DPI scales with Apple Context Management, on iOS, scales are not a thing for UIKit so just macOS
            int cmpH = venom::context::apple::ContextApple::GetWindowHeight();
            float scale = vc::Context::GetWindowScale();
            if (extent.height != cmpH * vc::Context::GetWindowScale()) {
                extent.width = venom::context::apple::ContextApple::GetWindowWidth();
                extent.height = cmpH;
            }
        }
#endif
    } else {
#if !defined(VENOM_DISABLE_GLFW)
        if (IS_CONTEXT_TYPE(GLFW)) {
            // Gets the window size in terms of total pixels, not to confuse with screen coordinates
            // Otherwise we would be using glfwGetWindowSize
            int w, h;
            glfwGetFramebufferSize((GLFWwindow*)(vc::Context::Get()->GetWindow()), &w, &h);
            
            extent.width = std::clamp<uint32_t>(w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            extent.height = std::clamp<uint32_t>(h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        } else
#endif
        {
            vc::Log::Error("Can't get extent.");
            return vc::Error::Failure;
        }
    }

    // Viewport
    viewport.x = 0.0f;
    viewport.y = static_cast<float>(extent.height);
    viewport.width = static_cast<float>(extent.width);
    viewport.height = -static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissor is a rectangle that defines the pixels that the rasterizer will use from the framebuffer
    scissor.offset = {0, 0};
    scissor.extent = extent;
    vc::GraphicsSettings::SetWindowExtent(extent.width, extent.height);
    return vc::Error::Success;
}

SwapChain* SwapChain::Get()
{
    return s_swapChain;
}

vc::Error SwapChain::InitSwapChain()
{
    const auto & capabilities = surface->GetCapabilities();

    venom_assert(capabilities.maxImageCount > 0, "Swap chain must have at least 1 image");

    // If already created, then clean up first
    if (swapChain != VK_NULL_HANDLE)
        CleanSwapChain();

    // Image count, must be at least the minimum image count, but no more than the maximum image count
    // One more is recommeneded to avoid waiting on the driver
    uint32_t imageCount = std::clamp(capabilities.minImageCount + 1, capabilities.minImageCount, capabilities.maxImageCount);
    imageCount = std::min<uint32_t>(VENOM_MAX_FRAMES_IN_FLIGHT, imageCount);

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->GetVkSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = activeSurfaceFormat.format;
    createInfo.imageColorSpace = activeSurfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // Always 1 unless stereoscopic 3D
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    createInfo.imageSharingMode = QueueManager::GetGraphicsComputeTransferSharingMode();
    vc::Vector<uint32_t> queueFamilyIndices;
    if (createInfo.imageSharingMode == VK_SHARING_MODE_CONCURRENT) {
        queueFamilyIndices.reserve(4);
        vc::Set<uint32_t> queueFamilyIndicesSet;
        queueFamilyIndicesSet.insert(QueueManager::GetGraphicsQueue().GetQueueFamilyIndex());
        queueFamilyIndicesSet.insert(QueueManager::GetComputeQueue().GetQueueFamilyIndex());
        queueFamilyIndicesSet.insert(QueueManager::GetTransferQueue().GetQueueFamilyIndex());
        queueFamilyIndicesSet.insert(QueueManager::GetPresentQueue().GetQueueFamilyIndex());
        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndicesSet.size());
        queueFamilyIndices.insert(queueFamilyIndices.end(), queueFamilyIndicesSet.begin(), queueFamilyIndicesSet.end());
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    } else {
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    if ((capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) == 0) {
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

    // VkHdrMetadataEXT hdrMetadata = {};
    // hdrMetadata.sType = VK_STRUCTURE_TYPE_HDR_METADATA_EXT;
    // hdrMetadata.pNext = nullptr;
    //
    // hdrMetadata.displayPrimaryRed = { 0.680f, 0.320f };
    // hdrMetadata.displayPrimaryGreen = { 0.265f, 0.690f };
    // hdrMetadata.displayPrimaryBlue = { 0.150f, 0.060f };
    // hdrMetadata.whitePoint = { 0.3127f, 0.3290f };
    //
    // // Set luminance values based on Liquid Retina XDR display capabilities
    // hdrMetadata.maxLuminance = 1600.0f;      // Max peak brightness in nits
    // hdrMetadata.minLuminance = 0.0001f;      // Minimum luminance
    // hdrMetadata.maxContentLightLevel = 1600.0f;  // MaxCLL in nits
    // hdrMetadata.maxFrameAverageLightLevel = 1000.0f;  // MaxFALL in nits
    //
    // auto vkSetHdrMetadataEXT = reinterpret_cast<PFN_vkSetHdrMetadataEXT>(
    //     vkGetDeviceProcAddr(LogicalDevice::GetVkDevice(), "vkSetHdrMetadataEXT")
    // );
    //vkSetHdrMetadataEXT(LogicalDevice::GetVkDevice(), 1, &swapChain, &hdrMetadata);


    // Getting handles of images in the swap chain
    swapChainImageHandles.clear();
    vkGetSwapchainImagesKHR(LogicalDevice::GetVkDevice(), swapChain, &imageCount, nullptr);
    swapChainImageHandles.resize(imageCount);
    vkGetSwapchainImagesKHR(LogicalDevice::GetVkDevice(), swapChain, &imageCount, swapChainImageHandles.data());

    // Create ImageViews
    __swapChainImageViews.clear();
    __swapChainImageViews.resize(imageCount);
    __swapChainImages.clear();
    __swapChainImages.resize(imageCount);
    for (size_t i = 0; i < imageCount; ++i) {
        __swapChainImages[i].CreateFromSwapChainImage(swapChainImageHandles[i], createInfo);
        if (__swapChainImageViews[i].Create(swapChainImageHandles[i], activeSurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT) != vc::Error::Success) {
            vc::Log::Error("Failed to create image view");
            return vc::Error::InitializationFailed;
        }
    }
    return vc::Error::Success;
}
}
