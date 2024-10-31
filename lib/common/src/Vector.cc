///
/// Project: VenomEngine
/// @file Vector.cc
/// @date Aug, 25 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/math/Vector.h>

namespace venom::common::math
{
vcm::Vec3 GetEulerAngles(const vcm::Quat& quat)
{
#if defined(VENOM_MATH_DXMATH)
    vcm::Vec3 euler;

    const float xx = quat.x * quat.x;
    const
    float yy = quat.y * quat.y;
    const
    float zz = quat.z * quat.z;

    // Roll (X-axis rotation)
    float sinr_cosp = 2.0f * (quat.w * quat.x + quat.y * quat.z);
    float cosr_cosp = 1.0f - 2.0f * (xx + yy);
    euler.x = std::atan2(sinr_cosp, cosr_cosp);

    // Pitch (Y-axis rotation)
    float sinp = 2.0f * (quat.w * quat.y - quat.z * quat.x);
    if (std::abs(sinp) >= 1)
        euler.y = std::copysign(DirectX::XM_PI / 2, sinp); // Use 90 degrees if out of range
    else
        euler.y = std::asin(sinp);

    // Yaw (Z-axis rotation)
    float siny_cosp = 2.0f * (quat.w * quat.z + quat.x * quat.y);
    float cosy_cosp = 1.0f - 2.0f * (yy + zz);
    euler.z = std::atan2(siny_cosp, cosy_cosp);

    // Convert to degrees (optional, remove if you prefer radians)
    euler.x = DirectX::XMConvertToDegrees(euler.x);
    euler.y = DirectX::XMConvertToDegrees(euler.y);
    euler.z = DirectX::XMConvertToDegrees(euler.z);

    return euler;
#elif defined(VENOM_MATH_GLM)
    return glm::eulerAngles(quat);
#endif
}

void RotateQuat(vcm::Quat& quat, const float angle, const vcm::Vec3& axis)
{
#if defined(VENOM_MATH_DXMATH)
    DirectX::XMVECTOR a = DirectX::XMLoadFloat3(&axis);
    DirectX::XMVECTOR result = DirectX::XMQuaternionRotationAxis(a, angle);
    result = DirectX::XMQuaternionMultiply(quat, result);
    DirectX::XMStoreFloat4(&quat, result);
#elif defined(VENOM_MATH_GLM)
    quat = glm::normalize(glm::rotate(quat, angle, axis));
#endif
}

vcm::Quat QuatFromViewMatrix(const vcm::Mat4& viewMatrix)
{
#if defined(VENOM_MATH_DXMATH)
    // Convert the view matrix to a quaternion
    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationMatrix(viewMatrix);
    // Conjugate the quaternion to convert from view space to world space
    return DirectX::XMQuaternionConjugate(quat);
#elif defined(VENOM_MATH_GLM)
    // Extract the rotation part (upper-left 3x3 matrix)
    glm::mat3 rotationMatrix = glm::mat3(viewMatrix);

    // Convert the rotation matrix to a quaternion
    glm::quat quaternion = glm::conjugate(glm::quat_cast(rotationMatrix));
    return quaternion;
#endif
}

vcm::Vec3 GetForward(const vcm::Quat& quat)
{
#if defined(VENOM_MATH_DXMATH)
    DirectX::XMVECTOR forward = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), quat);
    vcm::Vec3 result;
    DirectX::XMStoreFloat3(&result, forward);
    return result;
#elif defined(VENOM_MATH_GLM)
    return glm::normalize(glm::rotate(quat, glm::vec3(0.0f, 0.0f, -1.0f)));
#endif
}

vcm::Vec3 GetUp(const vcm::Quat& quat)
{
#if defined(VENOM_MATH_DXMATH)
    DirectX::XMVECTOR up = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), quat);
    vcm::Vec3 result;
    DirectX::XMStoreFloat3(&result, up);
    return result;
#elif defined(VENOM_MATH_GLM)
    return glm::normalize(glm::rotate(quat, glm::vec3(0.0f, 1.0f, 0.0f)));
#endif
}

vcm::Vec3 GetRight(const vcm::Quat& quat)
{
#if defined(VENOM_MATH_DXMATH)
    DirectX::XMVECTOR right = DirectX::XMVector3Rotate(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), quat);
    vcm::Vec3 result;
    DirectX::XMStoreFloat3(&result, right);
    return result;
#elif(defined(VENOM_MATH_GLM))
    return glm::normalize(glm::rotate(quat, glm::vec3(1.0f, 0.0f, 0.0f)));
#endif
}

vcm::Quat IdentityQuat()
{
#if defined(VENOM_MATH_DXMATH)
    return DirectX::XMQuaternionIdentity();
#elif defined(VENOM_MATH_GLM)
    return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
#endif
}

vcm::Vec3 GetPerpendicularVector(const vcm::Vec3& vec)
{
    vcm::Vec3 vecNormalized = vec;
    Normalize(vecNormalized);
    vcm::Vec3 arbitraryVector = vcm::Vec3(0.0f, 0.0f, 1.0f);
    if (std::abs(DotProduct(vecNormalized, arbitraryVector)) > 0.99f)
        arbitraryVector = vcm::Vec3(0.0f, 1.0f, 0.0f);
    return CrossProduct(arbitraryVector, vecNormalized);
}

vcm::Vec3 CrossProduct(const vcm::Vec3& a, const vcm::Vec3& b)
{
#if defined(VENOM_MATH_DXMATH)
    vcm::Vec3 result;
    DirectX::XMVECTOR cross = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b));
    DirectX::XMStoreFloat3(&result, cross);
    return result;
#elif defined(VENOM_MATH_GLM)
    return glm::cross(a, b);
#endif
}

float DotProduct(const vcm::Vec3& a, const vcm::Vec3& b)
{
#if defined(VENOM_MATH_DXMATH)
    return DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)));
#elif defined(VENOM_MATH_GLM)
    return glm::dot(a, b);
#endif
}

vcm::Vec3 RotateAround(const vcm::Vec3& point, const vcm::Vec3& target, const vcm::Vec3& planeNormal, float angle)
{
    auto u = planeNormal;

    auto w = vcm::GetPerpendicularVector(u);
    vcm::Normalize(w);
    auto v = vcm::CrossProduct(u, w);
    vcm::Normalize(v);

    auto pc = point - target;
    vcm::Vec2 position2D = vcm::Vec2(vcm::DotProduct(pc, w), vcm::DotProduct(pc, v));
    vcm::Vec2 rotatedPosition2D = vcm::RotateAround(position2D, vcm::Vec2(0.0f, 0.0f), angle);

    return target + (w * rotatedPosition2D.x) + (v * rotatedPosition2D.y);
}

vcm::Vec2 RotateAround(const vcm::Vec2& point, const vcm::Vec2& center, const float angle)
{
    float rad = angle * M_PI / 180.0f;
    return vcm::Vec2(
        std::cos(rad) * (point.x - center.x) - std::sin(rad) * (point.y - center.y) + center.x,
        std::sin(rad) * (point.x - center.x) + std::cos(rad) * (point.y - center.y) + center.y
    );
}

void Normalize(Vec3& vec)
{
#if defined(VENOM_MATH_DXMATH)
    DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vec));
#elif defined(VENOM_MATH_GLM)
    vec = glm::normalize(vec);
#endif
}

vcm::Quat FromEulerAngles(const float yaw, const float pitch, const float roll)
{
    vcm::Quat yawQuat{ std::cos(yaw / 2), 0.0f, std::sin(yaw / 2), 0.0f };
    vcm::Quat pitchQuat{ std::cos(pitch / 2), std::sin(pitch / 2), 0.0f, 0.0f };
    vcm::Quat rollQuat{ std::cos(roll / 2), 0.0f, 0.0f, std::sin(roll / 2) };
    return yawQuat * pitchQuat * rollQuat;
}
}
