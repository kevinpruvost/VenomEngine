///
/// Project: Bazel_Vulkan_Metal
/// @file Allocator.cc
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Allocator.h>

namespace venom
{
namespace vulkan
{
static VkAllocationCallbacks s_vkAllocationCallbacks = {};

const VkAllocationCallbacks * Allocator::GetVKAllocationCallbacks()
{
    return &s_vkAllocationCallbacks;
}

static void * fnAllocation(void * pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return vc::MemoryPool::Malloc(size);
}

static void * fnReallocation(void * pUserData, void * pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return vc::MemoryPool::Realloc(pOriginal, size);
}

static void fnInternalAllocation(void * pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{

}

static void fnFree(void * pUserData, void * pMemory)
{
    vc::MemoryPool::Free(pMemory);
}

static void fnInternalFree(void * pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{

}

void Allocator::SetVKAllocationCallbacks()
{
    s_vkAllocationCallbacks = {
        .pUserData = nullptr,
        .pfnAllocation = fnAllocation,
        .pfnReallocation = fnReallocation,
        .pfnFree = fnFree,
        .pfnInternalAllocation = fnInternalAllocation,
        .pfnInternalFree = fnInternalFree
    };
}
}
}