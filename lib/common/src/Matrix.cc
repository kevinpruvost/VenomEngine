///
/// Project: VenomEngine
/// @file Matrix.cc
/// @date Sep, 15 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/math/Matrix.h>

namespace venom
{
namespace common
{
namespace math
{
Mat4 Identity()
{
#if defined(VENOM_MATH_DXMATH)
    return DirectX::XMMatrixIdentity();
#elif defined(VENOM_MATH_GLM)
    return glm::mat4(1.0f);
#endif
}

void RotateMatrix(Mat4& matrix, const Vec3& axis, const float angle)
{
#if defined(VENOM_MATH_DXMATH)
    const Mat4 rotationMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&axis), angle);
    matrix = DirectX::XMMatrixMultiply(matrix, rotationMatrix);
#elif defined(VENOM_MATH_GLM)
    matrix = glm::rotate(matrix, angle, axis);
#endif
}

void RotateMatrix(Mat4& matrix, const Quat & quat)
{
#if defined(VENOM_MATH_DXMATH)
    // Convert the quaternion to a rotation matrix
    const Mat4 rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&quat));
    // Multiply the current matrix by the rotation matrix
    matrix = DirectX::XMMatrixMultiply(matrix, rotationMatrix);
#elif defined(VENOM_MATH_GLM)
    matrix *= glm::toMat4(quat);
#endif
}

void TranslateMatrix(Mat4& matrix, const Vec3& translation)
{
#if defined(VENOM_MATH_DXMATH)
    const Mat4 translationMatrix = DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);
    matrix = DirectX::XMMatrixMultiply(matrix, translationMatrix);
#elif defined(VENOM_MATH_GLM)
    matrix = glm::translate(matrix, translation);
#endif
}

void ScaleMatrix(Mat4& matrix, const Vec3& scale)
{
#if defined(VENOM_MATH_DXMATH)
    const Mat4 scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    matrix = DirectX::XMMatrixMultiply(matrix, scaleMatrix);
#elif defined(VENOM_MATH_GLM)
    matrix = glm::scale(matrix, scale);
#endif
}

Mat4 LookAtLH(const Vec3& eye, const Vec3& center, const Vec3& up)
{
#if defined(VENOM_MATH_DXMATH)
    Mat4 viewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&center), DirectX::XMLoadFloat3(&up));
    return viewMatrix;
#elif defined(VENOM_MATH_GLM)
    return glm::lookAtLH(eye, center, up);
#endif
}

Mat4 LookAtRH(const Vec3& eye, const Vec3& center, const Vec3& up)
{
#if defined(VENOM_MATH_DXMATH)
    Mat4 viewMatrix = DirectX::XMMatrixLookAtRH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&center), DirectX::XMLoadFloat3(&up));
    return viewMatrix;
#elif defined(VENOM_MATH_GLM)
    return glm::lookAt(eye, center, up);
#endif
}

Mat4 PerspectiveLH(const float fov, const float aspect, const float nearPlane, const float farPlane)
{
#if defined(VENOM_MATH_DXMATH)
    return DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearPlane, farPlane);
#elif defined(VENOM_MATH_GLM)
    return glm::perspectiveLH(fov, aspect, nearPlane, farPlane);
#endif
}

Mat4 PerspectiveRH(const float fov, const float aspect, const float nearPlane, const float farPlane)
{
#if defined(VENOM_MATH_DXMATH)
    return DirectX::XMMatrixPerspectiveFovRH(fov, aspect, nearPlane, farPlane);
#elif defined(VENOM_MATH_GLM)
    return glm::perspectiveRH(fov, aspect, nearPlane, farPlane);
#endif
}

Mat4 OrthographicLH(const float left, const float right, const float bottom, const float top, const float nearPlane, const float farPlane)
{
#if defined(VENOM_MATH_DXMATH)
    return DirectX::XMMatrixOrthographicLH(right - left, top - bottom, nearPlane, farPlane);
#elif defined(VENOM_MATH_GLM)
    return glm::ortho(left, right, bottom, top, nearPlane, farPlane);
#endif
}

Mat4 OrthographicRH(const float left, const float right, const float bottom, const float top, const float nearPlane, const float farPlane)
{
#if defined(VENOM_MATH_DXMATH)
    return DirectX::XMMatrixOrthographicRH(right - left, top - bottom, nearPlane, farPlane);
#elif defined(VENOM_MATH_GLM)
    return glm::orthoRH(left, right, bottom, top, nearPlane, farPlane);
#endif
}

Mat4 Inverse(const Mat4& matrix)
{
#if defined(VENOM_MATH_DXMATH)
    return DirectX::XMMatrixInverse(nullptr, matrix);
#elif defined(VENOM_MATH_GLM)
    return glm::inverse(matrix);
#endif
}
}
}
}