///
/// Project: VenomEngine
/// @file VulkanSurface.h
/// @date Aug, 20 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Instance.h>

#include <venom/common/Context.h>

namespace venom
{
namespace vulkan
{
class Surface
{
public:
    Surface();
    ~Surface();
    Surface(const Surface& surface) = delete;
    Surface& operator=(const Surface& surface) = delete;
    Surface(Surface&& surface) noexcept;
    Surface& operator=(Surface&& surface) noexcept;

    void Destroy();
    vc::Error CreateSurface(vc::Context * context);
    VkSurfaceKHR GetVkSurface() const;
    const VkSurfaceCapabilitiesKHR & GetCapabilities() const { return __capabilities; }
    const vc::Vector<VkSurfaceFormatKHR> & GetSurfaceFormats() const { return __surfaceFormats; }
    const vc::Vector<VkPresentModeKHR> & GetPresentModes() const { return __presentModes; }

private:
    VkSurfaceKHR __surface;
    VkSurfaceCapabilitiesKHR __capabilities;
    vc::Vector<VkSurfaceFormatKHR> __surfaceFormats;
    vc::Vector<VkPresentModeKHR> __presentModes;
};
}
}
