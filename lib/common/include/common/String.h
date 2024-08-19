///
/// Project: Bazel_Vulkan_Metal
/// File: String.h
/// Date: 8/18/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <string>

namespace venom
{

using String = std::string;
template<typename ...Args>
String format(const char* fmt, Args&&... args)
{
    const int length = snprintf(nullptr, 0, fmt, args...);
    String message(length + 1, 0);
    sprintf(const_cast<char *>(message.c_str()), fmt, args...);
    return message;
}

}