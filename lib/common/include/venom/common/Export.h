///
/// Project: VenomEngine
/// @file Export.h
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif __APPLE__
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#endif

#ifdef VENOM_COMMON_EXPORTS
#define VENOM_COMMON_API EXPORT
#else
#define APPLICATION_API IMPORT
#endif