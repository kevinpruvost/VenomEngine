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