///
/// Project: VenomEngine
/// @file String.h
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <string>

namespace venom
{
namespace common
{
using String = std::string;
template<typename ...Args>
String format(const char* fmt, Args&&... args)
{
    const int length = snprintf(nullptr, 0, fmt, args...);
    String message(length, 0);
    snprintf(const_cast<char *>(message.c_str()), length+1, fmt, args...);
    return message;
}
}
}