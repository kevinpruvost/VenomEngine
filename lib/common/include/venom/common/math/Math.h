///
/// Project: VenomEngine
/// @file Math.h
/// @date Aug, 25 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

// Coordinate System (default is right hand because Vulkan, Metal & OpenGL use it)
#if not defined(VENOM_COORDINATE_LEFT_HAND)
#define VENOM_COORDINATE_RIGHT_HAND
#endif

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
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#endif

namespace venom
{
namespace common
{
namespace math
{
}
}
}
namespace vcm = venom::common::math;
