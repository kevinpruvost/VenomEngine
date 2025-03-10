///
/// Project: VenomEngine
/// @file Error.h
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Export.h>
#include <venom/common/Debug.h>
#include <venom/common/String.h>
#include <venom/common/Platform.h>

#include <cstdint>
#include <cstring>

namespace vc = venom::common;
namespace venom
{
namespace common
{
enum class VENOM_COMMON_API Error : uint32_t
{
    Success = 0,
    Failure         = 1 << 0,
    InvalidArgument = 1 << 1,
    OutOfMemory     = 1 << 2,
    DeviceLost      = 1 << 3,
    InitializationFailed = 1 << 4,
    InvalidUse      = 1 << 5,
    FeatureNotSupported = 1 << 6,
    Unknown = std::numeric_limits<uint32_t>::max()
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
}