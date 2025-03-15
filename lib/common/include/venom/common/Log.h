///
/// Project: VenomEngine
/// @file Log.h
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Error.h>
#include <venom/common/Export.h>

namespace venom
{
namespace common
{
class VENOM_COMMON_API Log
{
public:
    static void LogToFile(const char* str);
    template <typename... Args>
    static void LogToFile(const char* fmt, Args&&... args)
    {
        const String str = format(fmt, args...);
        LogToFile(str.c_str());
    }

    static void Print(const char* str);
    template <typename... Args>
    static void Print(const char* fmt, Args&&... args)
    {
        const String str = format(fmt, args...);
        Print(str.c_str());
    }

    static void Print(FILE * const stream, const char* str);
    template <typename... Args>
    static void Print(FILE * const stream, const char* fmt, Args&&... args)
    {
        const String str = format(fmt, args...);
        Print(stream, str.c_str());
    }

    inline static void Error(const char* str) { Print(stderr, str); }
    template <typename... Args>
    inline static void Error(const char* fmt, Args&&... args) { Print(stderr, fmt, args...); }
};
}
}

#if defined(VENOM_DEBUG)
#define DEBUG_PRINT(...) vc::Log::Print(__VA_ARGS__)
#define DEBUG_LOG(...) vc::Log::LogToFile(__VA_ARGS__)

#define venom_assert(condition, ...) \
if (!(condition)) \
{ \
DEBUG_PRINT("Assertion failed: %s\n", #condition); \
DEBUG_PRINT("File: %s, Line: %d\n", __FILE__, __LINE__); \
DEBUG_PRINT(__VA_ARGS__); \
DEBUG_PRINT("\n"); \
abort(); \
}

#else
#define DEBUG_PRINT(str, ...)
#define DEBUG_LOG(str, ...)

#define venom_assert(condition, ...)
#endif
