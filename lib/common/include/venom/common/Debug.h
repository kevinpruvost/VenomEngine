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

#if defined(VENOM_DEBUG)
#define _ANALYSIS
#define DEBUG_CODE(code) code
#else
#define DEBUG_CODE(code)
#endif
