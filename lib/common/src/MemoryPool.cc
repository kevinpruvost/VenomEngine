///
/// Project: VenomEngine
/// @file MemoryPool.cc
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/MemoryPool.h>

namespace venom
{
namespace common
{
MemoryPool::MemoryPool()
{
}

MemoryPool::~MemoryPool()
{
}

Error MemoryPool::CreateMemoryPool()
{
    return Error::Success;
}

void* MemoryPool::Malloc(const size_t size)
{
    return malloc(size);
}

void MemoryPool::Free(void* ptr)
{
    free(ptr);
}

void* MemoryPool::Realloc(void* ptr, const size_t size)
{
    return realloc(ptr, size);
}
}
}
