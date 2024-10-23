///
/// Project: VenomEngine
/// @file Matrix.h
/// @date Sep, 15 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/math/Vector.h>

namespace venom
{
namespace common
{
namespace math
{
VENOM_COMMON_API Mat4 Identity();

/**
 * @brief Rotate matrix
 * @param matrix to rotate
 * @param axis normalized
 * @param angle in radians
 */
VENOM_COMMON_API void RotateMatrix(Mat4& matrix, const Vec3& axis, const float angle);

/**
 * @brief Rotate matrix
 * @param matrix to rotate
 * @param quat quaternion
 */
VENOM_COMMON_API void RotateMatrix(Mat4& matrix, const Quat& quat);

/**
 * @brief Translate matrix
 * @param matrix
 * @param translation 
 */
VENOM_COMMON_API void TranslateMatrix(Mat4& matrix, const Vec3& translation);
VENOM_COMMON_API Mat4 LookAtLH(const Vec3& eye, const Vec3& center, const Vec3& up);
VENOM_COMMON_API Mat4 LookAtRH(const Vec3& eye, const Vec3& center, const Vec3& up);
inline Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
#if defined(VENOM_COORDINATE_LEFT_HAND)
    return LookAtLH(eye, center, up);
#elif defined(VENOM_COORDINATE_RIGHT_HAND)
    return LookAtRH(eye, center, up);
#endif
};
VENOM_COMMON_API Mat4 PerspectiveLH(const float fov, const float aspect, const float nearPlane, const float farPlane);
VENOM_COMMON_API Mat4 PerspectiveRH(const float fov, const float aspect, const float nearPlane, const float farPlane);
inline Mat4 Perspective(const float fov, const float aspect, const float nearPlane, const float farPlane)
{
#if defined (VENOM_COORDINATE_LEFT_HAND)
    return PerspectiveLH(fov, aspect, nearPlane, farPlane);
#elif defined (VENOM_COORDINATE_RIGHT_HAND)
    return PerspectiveRH(fov, aspect, nearPlane, farPlane);
#endif
}
}
}
}