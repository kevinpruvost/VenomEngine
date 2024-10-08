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

namespace venom::vulkan
{
Surface::Surface()
    : __surface(VK_NULL_HANDLE)
{
}

Surface::~Surface()
{
    if (__surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(Instance::GetVkInstance(), __surface, Allocator::GetVKAllocationCallbacks());
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

vc::Error Surface::CreateSurface(vc::Context* context)
{
#if defined(_WIN32)
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(context->GetWindow());
    createInfo.hinstance = GetModuleHandle(nullptr);
    if (auto res = vkCreateWin32SurfaceKHR(Instance::GetVkInstance(), &createInfo, Allocator::GetVKAllocationCallbacks(), &__surface); res != VK_SUCCESS) {
        vc::Log::Error("Failed to create Win32 surface: %d", res);
        return vc::Error::InitializationFailed;
    }
#elif defined(__APPLE__)
    if (auto res = glfwCreateWindowSurface(Instance::GetVkInstance(), context->GetWindow(), Allocator::GetVKAllocationCallbacks(), &__surface); res != VK_SUCCESS) {
        vc::Log::Error("Failed to create MacOS surface: %x", res);
        return vc::Error::InitializationFailed;
    }
#elif defined(__linux__)
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
    return vc::Error::Success;
}

VkSurfaceKHR Surface::GetVkSurface() const
{
    return __surface;
}
}
