///
/// Project: VenomEngine
/// @file Vector.h
/// @date Aug, 25 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/math/Math.h>
#include <venom/common/Error.h>

namespace venom
{
namespace common
{
namespace math
{
#if defined(VENOM_MATH_DXMATH)
typedef DirectX::XMFLOAT4 Vec4;
typedef DirectX::XMFLOAT3 Vec3;
typedef DirectX::XMFLOAT2 Vec2;

typedef DirectX::XMINT2 IVec2;
typedef DirectX::XMINT3 IVec3;
typedef DirectX::XMINT4 IVec4;

typedef DirectX::XMUINT2 UVec2;
typedef DirectX::XMUINT3 UVec3;
typedef DirectX::XMUINT4 UVec4;

typedef DirectX::XMVECTOR Quat;

typedef DirectX::XMMATRIX Mat4;
typedef DirectX::XMFLOAT3X3 Mat3;
typedef DirectX::XMFLOAT2X2 Mat2;
#elif defined(VENOM_MATH_GLM)
typedef glm::vec4 Vec4;
typedef glm::vec3 Vec3;
typedef glm::vec2 Vec2;

typedef glm::ivec2 IVec2;
typedef glm::ivec3 IVec3;
typedef glm::ivec4 IVec4;

typedef glm::uvec2 UVec2;
typedef glm::uvec3 UVec3;
typedef glm::uvec4 UVec4;

typedef glm::quat Quat;

typedef glm::mat4 Mat4;
typedef glm::mat3 Mat3;
typedef glm::mat2 Mat2;
#endif

// We want vertices to be used separate and not packed (non-interleaved and not interleaved)

/// @brief Vertex Position (3 floats)
typedef Vec3 VertexPos;

/// @brief Vertex Normal (3 floats)
typedef Vec3 VertexNormal;

/// @brief Vertex Color (4 floats) RGBA
typedef Vec4 VertexColor;

/// @brief Vertex Texture Coordinates (2 floats)
typedef Vec2 VertexUV;

/// @brief Vertex Tangents (3 floats)
typedef Vec3 VertexTangent;

/// @brief Vertex Bitangent (3 floats)
typedef Vec3 VertexBitangent;

/// @brief Vertex Index (used for indexing in element buffers)
typedef uint32_t VertexIndex;


/**
 * @brief Gets Euler Angles From Quaternion
 * @param quat
 * @return Euler Angles
 */
vcm::Vec3 GetEulerAngles(const vcm::Quat & quat);

/**
 * @brief Rotate a quaternion
 * @param quat
 * @param angle
 * @param axis
 */
void RotateQuat(vcm::Quat & quat, const float angle, const vcm::Vec3 & axis);

/**
 * @brief Create a quaternion from a view matrix
 * @param viewMatrix
 * @return Quaternion
 */
vcm::Quat QuatFromViewMatrix(const vcm::Mat4 & viewMatrix);

/**
 * @brief Get the forward vector from a quaternion
 * @param quat
 * @return Forward vector
 */
vcm::Vec3 GetForward(const vcm::Quat & quat);

/**
 * @brief Get the up vector from a quaternion
 * @param quat
 * @return up vector
 */
vcm::Vec3 GetUp(const vcm::Quat & quat);

/**
 * @brief Get the right vector from a quaternion
 * @param quat
 * @return right vector
 */
vcm::Vec3 GetRight(const vcm::Quat & quat);

/**
 * @brief Get the identity quaternion
 * @return Identity quaternion
 */
vcm::Quat IdentityQuat();

/**
 * @brief Get the perpendicular vector of a vector
 * @return Perpendicular vector
 */
vcm::Vec3 GetPerpendicularVector(const vcm::Vec3 & vec);

vcm::Vec3 CrossProduct(const vcm::Vec3 & a, const vcm::Vec3 & b);

float DotProduct(const vcm::Vec3 & a, const vcm::Vec3 & b);

/**
 * @brief Rotates around the target on the given planeNormal by the given angle
 * @param point
 * @param target
 * @param planeNormal
 * @param angle
 * @return new point
 */
vcm::Vec3 RotateAround(const vcm::Vec3 & point, const vcm::Vec3& target, const vcm::Vec3& planeNormal, float angle);

/**
 * @brief Rotates point around center by angle (degrees)
 * @param point
 * @param center
 * @param angle
 * @return new point
 */
vcm::Vec2 RotateAround(const vcm::Vec2 & point, const vcm::Vec2 & center, const float angle);

VENOM_COMMON_API void Normalize(Vec3& vec);

}
}
}