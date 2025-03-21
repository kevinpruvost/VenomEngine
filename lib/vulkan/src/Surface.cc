///
/// Project: VenomEngine
/// @file VulkanSurface.cc
/// @date Aug, 20 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Surface.h>

#include <venom/vulkan/Instance.h>
#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/PhysicalDevice.h>

#include "venom/common/Config.h"

#if defined(VENOM_PLATFORM_APPLE)
#include <vulkan/vulkan_metal.h>
#include <venom/common/context/ContextApple.h>
#endif

namespace venom::vulkan
{
Surface::Surface()
    : __surface(VK_NULL_HANDLE)
{
}

Surface::~Surface()
{
    Destroy();
}

Surface::Surface(Surface&& surface) noexcept
    : __surface(surface.__surface)
{
    surface.__surface = VK_NULL_HANDLE;
}

Surface& Surface::operator=(Surface&& surface) noexcept
{
    if (this != &surface) {
        __surface = surface.__surface;
        surface.__surface =  VK_NULL_HANDLE;
    }
    return *this;
}

void Surface::Destroy()
{
    if (__surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(Instance::GetVkInstance(), __surface, Allocator::GetVKAllocationCallbacks());
        __surface = VK_NULL_HANDLE;
    }
}

vc::Error Surface::CreateSurface(vc::Context* context)
{
    Destroy();
    auto contextType = vc::Config::GetContextType();
    switch (contextType) {
#if !defined(VENOM_DISABLE_GLFW)
        case vc::Context::ContextType::GLFW: {
#if defined(VENOM_PLATFORM_WINDOWS)
            VkWin32SurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            createInfo.hwnd = glfwGetWin32Window(context->GetWindow());
            createInfo.hinstance = GetModuleHandle(nullptr);
            if (auto res = vkCreateWin32SurfaceKHR(Instance::GetVkInstance(), &createInfo, Allocator::GetVKAllocationCallbacks(), &__surface); res != VK_SUCCESS) {
                vc::Log::Error("Failed to create Win32 surface: %d", res);
                return vc::Error::InitializationFailed;
            }
#elif defined(VENOM_PLATFORM_MACOS)
            if (auto res = glfwCreateWindowSurface(Instance::GetVkInstance(), (GLFWwindow*)context->GetWindow(), Allocator::GetVKAllocationCallbacks(), &__surface); res != VK_SUCCESS) {
                vc::Log::Error("Failed to create MacOS surface: %x", res);
                return vc::Error::InitializationFailed;
            }
#elif defined(VENOM_PLATFORM_LINUX)
#if defined(_GLFW_X11)
            VkXlibSurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
            createInfo.dpy = glfwGetX11Display();
            createInfo.window = glfwGetX11Window(context->GetWindow());
            if (auto res = vkCreateXlibSurfaceKHR(Instance::GetVkInstance(), &createInfo, Allocator::GetVKAllocationCallbacks(), &__surface); res != VK_SUCCESS) {
                vc::Log::Error("Failed to create Xlib surface: %d", res);
                return vc::Error::InitializationFailed;
            }
#elif defined(_GLFW_WAYLAND)
            VkWaylandSurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
            createInfo.display = glfwGetWaylandDisplay();
            createInfo.surface = glfwGetWaylandWindow(context->GetWindow());
            if (auto res = vkCreateWaylandSurfaceKHR(Instance::GetVkInstance(), &createInfo, Allocator::GetVKAllocationCallbacks(), &__surface); res != VK_SUCCESS) {
                vc::Log::Error("Failed to create Wayland surface: %d", res);
                return vc::Error::InitializationFailed;
            }
#endif
#endif
            break;
        };
#endif
#if defined(VENOM_PLATFORM_APPLE)
        case vc::Context::ContextType::Apple: {
            VkMetalSurfaceCreateInfoEXT surfaceInfo = {};
            surfaceInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
            surfaceInfo.pLayer = venom::context::apple::ContextApple::GetMetalLayer();
            if (auto res = vkCreateMetalSurfaceEXT(Instance::GetVkInstance(), &surfaceInfo, Allocator::GetVKAllocationCallbacks(), &__surface); res != VK_SUCCESS) {
                vc::Log::Error("Failed to create Surface from Metal Layer: %d", res);
                return vc::Error::InitializationFailed;
            }
            break;
        }
#endif
        default: break;
    }
    // Get surface capabilities
    if (auto err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice::GetUsedVkPhysicalDevice(), GetVkSurface(), &__capabilities); err != VK_SUCCESS)
    {
        vc::Log::Error("Failed to get physical device surface capabilities: %d", err);
        return vc::Error::InitializationFailed;
    }

    // Get surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice::GetUsedVkPhysicalDevice(), GetVkSurface(), &formatCount, nullptr);

    if (formatCount != 0) {
        __surfaceFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice::GetUsedVkPhysicalDevice(), GetVkSurface(), &formatCount, __surfaceFormats.data());
    }

    // Get present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice::GetUsedVkPhysicalDevice(), GetVkSurface(), &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        __presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice::GetUsedVkPhysicalDevice(), GetVkSurface(), &presentModeCount, __presentModes.data());
    }
    return vc::Error::Success;
}

VkSurfaceKHR Surface::GetVkSurface() const
{
    return __surface;
}
}
