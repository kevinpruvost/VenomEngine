///
/// Project: Bazel_Vulkan_Metal
/// @file Allocator.h
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/MemoryPool.h>
#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class Allocator
{
public:
    static const VkAllocationCallbacks * GetVKAllocationCallbacks();
    static void SetVKAllocationCallbacks();
};
}
}