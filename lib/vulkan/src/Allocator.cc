///
/// Project: VenomEngine
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

Allocator::Allocator()
#if defined(VENOM_DEBUG)
    : allocatedSize(0)
    , allocatedSizeMax(0)
#endif
{
}

Allocator::~Allocator()
{
#if defined(VENOM_DEBUG)
   vc::Log::Print("Maximum size allocated by Vulkan: %ukB", allocatedSizeMax / 1000);
#endif
}

const VkAllocationCallbacks * Allocator::GetVKAllocationCallbacks()
{
    return &s_vkAllocationCallbacks;
}

static void * fnAllocation(void * pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
#if defined(VENOM_DEBUG)
    Allocator* ptr = (Allocator*)pUserData;
    ptr->allocatedSize += size;
    if (ptr->allocatedSize > ptr->allocatedSizeMax) {
        ptr->allocatedSizeMax = ptr->allocatedSize;
    }
    void * ret = vc::MemoryPool::Malloc(size);
    ptr->allocations[ret] = size;
    return ret;
#else
    return vc::MemoryPool::Malloc(size);
#endif
}

static void * fnReallocation(void * pUserData, void * pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
#if defined(VENOM_DEBUG)
    Allocator* ptr = (Allocator*)pUserData;
    ptr->allocatedSize += size;
    ptr->allocatedSize -= ptr->allocations[pOriginal];
    if (ptr->allocatedSize > ptr->allocatedSizeMax) {
        ptr->allocatedSizeMax = ptr->allocatedSize;
    }
    void * ret = vc::MemoryPool::Realloc(pOriginal, size);
    ptr->allocations[ret] = size;
    ptr->allocations.erase(pOriginal);
    return ret;
#else
    return vc::MemoryPool::Realloc(pOriginal, size);
#endif
}

static void fnInternalAllocation(void * pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
    // Just a notification basically: https://docs.vulkan.org/spec/latest/chapters/memory.html
}

static void fnFree(void * pUserData, void * pMemory)
{
#if defined(VENOM_DEBUG)
    Allocator* ptr = (Allocator*)pUserData;
    ptr->allocatedSize -= ptr->allocations[pMemory];
    ptr->allocations.erase(pMemory);
    vc::MemoryPool::Free(pMemory);
#else
    return vc::MemoryPool::Free(pMemory);
#endif
}

static void fnInternalFree(void * pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
    // Just a notification basically: https://docs.vulkan.org/spec/latest/chapters/memory.html
}

void Allocator::SetVKAllocationCallbacks()
{
    static Allocator allocator;
    s_vkAllocationCallbacks = {
        .pUserData = &allocator,
        .pfnAllocation = fnAllocation,
        .pfnReallocation = fnReallocation,
        .pfnFree = fnFree,
        .pfnInternalAllocation = fnInternalAllocation,
        .pfnInternalFree = fnInternalFree
    };
}
}
}