///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanSurface.h
/// Date: 8/20/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanDebug.h"

#include <common/Context.h>

namespace venom
{
class VulkanSurface
{
public:
    VulkanSurface();
    ~VulkanSurface();

    Error CreateSurface(Context * context);

public:
    VkSurfaceKHR surface;
};
}
