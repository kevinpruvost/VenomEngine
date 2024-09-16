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
#elif defined(VENOM_MATH_GLM)
typedef glm::vec4 Vec4;
typedef glm::vec3 Vec3;
typedef glm::vec2 Vec2;
#endif

// We want vertices to be used separate and not packed (non-interleaved and not interleaved)

/// @brief Vertex Position (3 floats)
typedef Vec3 VertexPos;

/// @brief Vertex Normal (3 floats)
typedef Vec3 VertexNormal;

/// @brief Vertex Color (3 floats)
typedef Vec3 VertexColor;

/// @brief Vertex Texture Coordinates (2 floats)
typedef Vec2 VertexUV;

/// @brief Vertex Bitangent (3 floats)
typedef Vec3 VertexBitangent;

/// @brief Vertex Index (used for indexing in element buffers)
typedef uint32_t VertexIndex;
}
}
}