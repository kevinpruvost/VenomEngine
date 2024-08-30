///
/// Project: VenomEngine
/// @file Debug.h
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#if defined(_DEBUG) || defined(DEBUG) || (!defined(NDEBUG) && defined(__GNUC__))
#define VENOM_DEBUG
#endif

#ifdef VENOM_DEBUG
#define _ANALYSIS
#define DEBUG_PRINT(str, ...) vc::Log::Print(str, __VA_ARGS__)
#define DEBUG_LOG(str, ...) vc::Log::LogToFile(str, __VA_ARGS__)
#define DEBUG_CODE(code) code

#define venom_assert(condition, ...) \
    if (!(condition)) \
    { \
        DEBUG_PRINT("Assertion failed: %s\n", #condition); \
        DEBUG_PRINT("File: %s, Line: %d\n", __FILE__, __LINE__); \
        DEBUG_PRINT(__VA_ARGS__); \
        DEBUG_PRINT("\n"); \
        DEBUG_LOG("Assertion failed: %s\n", #condition); \
        DEBUG_LOG("File: %s, Line: %d\n", __FILE__, __LINE__); \
        DEBUG_LOG(__VA_ARGS__); \
        DEBUG_LOG("\n"); \
        abort(); \
    }

#else
#define DEBUG_PRINT(str, ...)
#define DEBUG_LOG(str, ...)
#define DEBUG_CODE(code)

#define venom_assert(condition, ...)
#endif