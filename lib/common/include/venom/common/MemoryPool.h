///
/// Project: Bazel_Vulkan_Metal
/// @file MemoryPool.h
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Error.h>

namespace venom
{
namespace common
{
/// @brief MemoryPool class to manage memory allocation and deallocation.
/// For now, it's just a wrapper around memory C functions.
class VENOM_COMMON_API MemoryPool
{
private:
    MemoryPool();
public:
    ~MemoryPool();
    static Error CreateMemoryPool();
    static void * Malloc(const size_t size);
    static void Free(void * ptr);
    static void * Realloc(void * ptr, const size_t size);
};
}
}
