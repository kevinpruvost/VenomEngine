///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanInstance.h
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
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

#define GLM_FORCE_INTRINSICS
// Will most likely be SSE2 as it's very common among CPUs today
#if defined(__SSE__)
#define GLM_FORCE_SSE2
#elif defined(__AVX__)
#define GLM_FORCE_AVX
#elif defined(__ARM_NEON)  // Check if NEON is supported on ARM
#define GLM_FORCE_NEON
#endif
#include <glm/glm.hpp>
#include <common/Log.h>

namespace venom
{
class VulkanInstance
{
public:
    VulkanInstance();
    ~VulkanInstance();
    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;
    VulkanInstance(VulkanInstance&&) = delete;
    VulkanInstance& operator=(VulkanInstance&&) = delete;

    static VkInstance & GetInstance();
};
}
