///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanSurface.cc
/// Date: 8/20/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanSurface.h"

namespace venom
{
VulkanSurface::VulkanSurface()
{
}

VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(VulkanInstance::GetInstance(), surface, nullptr);
}

Error VulkanSurface::CreateSurface(Context* context)
{
#if defined(_WIN32)
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(context->GetWindow());
    createInfo.hinstance = GetModuleHandle(nullptr);
    if (auto res = vkCreateWin32SurfaceKHR(VulkanInstance::GetInstance(), &createInfo, nullptr, &surface); res != VK_SUCCESS) {
        Log::Error("Failed to create Win32 surface: %d", res);
        return Error::InitializationFailed;
    }
#elif defined(__APPLE__)
    VkMacOSSurfaceCreateInfoMVK createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    createInfo.pView = glfwGetCocoaWindow(context->GetWindow());
    if (auto res = vkCreateMacOSSurfaceMVK(VulkanInstance::GetInstance(), &createInfo, nullptr, &surface); res != VK_SUCCESS) {
        Log::Error("Failed to create MacOS surface: %d", res);
        return Error::InitializationFailed;
    }
#elif defined(__linux__)
#if defined(_GLFW_X11)
    VkXlibSurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.dpy = glfwGetX11Display();
    createInfo.window = glfwGetX11Window(context->GetWindow());
    if (auto res = vkCreateXlibSurfaceKHR(VulkanInstance::GetInstance(), &createInfo, nullptr, &surface); res != VK_SUCCESS) {
        Log::Error("Failed to create Xlib surface: %d", res);
        return Error::InitializationFailed;
    }
#elif defined(_GLFW_WAYLAND)
    VkWaylandSurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    createInfo.display = glfwGetWaylandDisplay();
    createInfo.surface = glfwGetWaylandWindow(context->GetWindow());
    if (auto res = vkCreateWaylandSurfaceKHR(VulkanInstance::GetInstance(), &createInfo, nullptr, &surface); res != VK_SUCCESS) {
        Log::Error("Failed to create Wayland surface: %d", res);
        return Error::InitializationFailed;
    }
#endif
#endif
    return Error::Success;
}
}
