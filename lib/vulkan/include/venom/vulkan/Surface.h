///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanSurface.h
/// Date: 8/20/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

#include <common/Context.h>

namespace venom
{
namespace vulkan
{
class Surface
{
public:
    Surface();
    ~Surface();

    Error CreateSurface(Context * context);

public:
    VkSurfaceKHR surface;
};
}
}