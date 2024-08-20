///
/// Project: Bazel_Vulkan_Metal
/// File: Debug.h
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#if defined(_DEBUG) || defined(DEBUG)
#define VENOM_DEBUG
#endif

#if !defined(NDEBUG) && defined(__GNUC__)
#define VENOM_DEBUG
#endif

#ifdef VENOM_DEBUG
#define DEBUG_PRINT(str, ...) venom::Log::Print(str, __VA_ARGS__)
#define DEBUG_LOG(str, ...) venom::Log::LogToFile(str, __VA_ARGS__)
#define DEBUG_CODE(code) code

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
#define DEBUG_CODE(code)

#define venom_assert(condition, ...)
#endif