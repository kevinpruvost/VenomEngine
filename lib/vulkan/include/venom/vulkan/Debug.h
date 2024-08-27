///
/// Project: VenomEngine
/// @file Debug.h
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <vector>
#include <set>

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

#include <venom/common/math/Math.h>
#include <venom/common/Log.h>

namespace venom
{
namespace vulkan
{
class Instance;
class DebugApplication
{
    friend class Instance;
public:
    DebugApplication();
    ~DebugApplication();

    vc::Error InitDebug();
    void DestroyDebug();

protected:
    void _SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo * createInfos);
    template<typename CreateInfo>
    void _SetCreateInfoValidationLayers(CreateInfo * createInfos);
    vc::Error _PostInstance_SetDebugParameters();
    PFN_vkDebugUtilsMessengerCallbackEXT _GetDebugCallback();

private:
    vc::Error __InitValidationLayers();

#ifdef VENOM_DEBUG
    std::vector<const char *>      __validationLayersInUse;
    std::vector<VkLayerProperties> __validationLayersAvailable;

    VkDebugUtilsMessengerEXT __debugMessenger;
    VkDebugUtilsMessengerCreateInfoEXT __debugMessengerCreateInfo;
#endif
};

}
}