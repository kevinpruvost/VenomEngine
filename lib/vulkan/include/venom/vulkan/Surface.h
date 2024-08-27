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

    vc::Error CreateSurface(vc::Context * context);

public:
    VkSurfaceKHR surface;
};
}
}