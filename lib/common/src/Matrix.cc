///
/// Project: Bazel_Vulkan_Metal
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
void RotateMatrix(Mat4& matrix, const Vec3& axis, const float angle)
{
#if defined(VENOM_MATH_DXMATH)
    const Mat4 rotationMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&axis), angle);
    matrix = DirectX::XMMatrixMultiply(matrix, rotationMatrix);
#elif defined(VENOM_MATH_GLM)
    matrix = glm::rotate(matrix, angle, axis);
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
}
}
}