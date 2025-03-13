///
/// Project: VenomEngine
/// @file Debug.h
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Platform.h>
#include <venom/common/Timer.h>
#include <venom/common/Containers.h>

//#define VK_ENABLE_BETA_EXTENSIONS
#if !defined(VENOM_PLATFORM_IOS)
#define GLFW_INCLUDE_VULKAN
#if defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__APPLE__)
#define VK_USE_PLATFORM_MACOS_MVK
#elif defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <GLFW/glfw3.h>

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>

#else

#include <vulkan/vulkan.h>

#endif

#include <venom/common/math/Math.h>
#include <venom/common/Log.h>

namespace venom
{
namespace vulkan
{
class Instance;
class VulkanApplication;
class DebugApplication
{
    friend class Instance;
    friend class VulkanApplication;
public:
    DebugApplication();
    virtual ~DebugApplication();

    vc::Error InitDebug();
    void DestroyDebug();

protected:
    void _SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo * createInfos);
    template<typename CreateInfo>
    void _SetCreateInfoValidationLayers(CreateInfo * createInfos);
    vc::Error _PostInstance_SetDebugParameters();
    PFN_vkDebugUtilsMessengerCallbackEXT _GetDebugCallback();

    double _GetTheoreticalFPS(double fps);
    void _UpdateTheoreticalFPS(uint32_t microsecondsWaitedOnVSync);

protected:

private:
    vc::Error __InitValidationLayers();

#ifdef VENOM_DEBUG
    vc::Vector<const char *>      __validationLayersInUse;
    vc::Vector<VkLayerProperties> __validationLayersAvailable;

    VkDebugUtilsMessengerEXT __debugMessenger;
    VkDebugUtilsMessengerCreateInfoEXT __debugMessengerCreateInfo;
#endif
private:
    uint64_t __microsecondsWaitedOnVSync;
    vc::Timer __fpsTimer;
};

}
}
