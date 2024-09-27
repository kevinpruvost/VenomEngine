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

    vc::Error CreateSurface(vc::Context * context);
    VkSurfaceKHR GetVkSurface() const;

private:
    VkSurfaceKHR __surface;
};
}
}