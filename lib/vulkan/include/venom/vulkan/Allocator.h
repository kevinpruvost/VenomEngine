///
/// Project: VenomEngine
/// @file Allocator.h
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/MemoryPool.h>
#include <venom/vulkan/Debug.h>

#include <unordered_map>

namespace venom
{
namespace vulkan
{
class Allocator
{
private:
    Allocator();
public:
    ~Allocator();
    static const VkAllocationCallbacks * GetVKAllocationCallbacks();
    static void SetVKAllocationCallbacks();

public:
    int allocatedSize;
    int allocatedSizeMax;
    vc::UMap<void *, size_t> allocations;
};
}
}