///
/// Project: VenomEngine
/// @file Math_Api.h
/// @date Aug, 25 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

// If on windows and FORCE_GLM is not defined, then use DirectXMath
#if defined(_WIN32) && !defined(FORCE_GLM)
#define VENOM_MATH_DXMATH
#include <DirectXMath.h>
#else
#define VENOM_MATH_GLM
// Will most likely be SSE2 as it's very common among CPUs today
#if defined(__SSE__)
#define GLM_FORCE_SSE2
#elif defined(__AVX__)
#define GLM_FORCE_AVX
#elif defined(__ARM_NEON)  // Check if NEON is supported on ARM
#define GLM_FORCE_NEON
#endif
#include <glm/glm.hpp>
#endif