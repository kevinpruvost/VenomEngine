///
/// Project: Bazel_Vulkan_Metal
/// File: Error.h
/// Date: 8/18/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <common/Export.h>

#include <cstdint>
#include <cstring>

#include "String.h"

namespace venom
{
enum class Error : uint32_t
{
    Success = 0,
    Failure         = 1 << 0,
    InvalidArgument = 1 << 1,
    OutOfMemory     = 1 << 2,
    DeviceLost      = 1 << 3,
    InitializationFailed = 1 << 4,
    Unknown = UINT32_MAX
};
VENOM_COMMON_API void setErrorString(const char * str);
template <typename... Args>
void setErrorString(const char* fmt, Args&&... args)
{
    String str = format(fmt, args...);
    setErrorString(str.c_str());
}

VENOM_COMMON_API const char * getErrorString();
}
