///
/// Project: VenomEngineWorkspace
/// @file Transform3D.cc
/// @date Oct, 21 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Transform3D.h>
#include <venom/common/plugin/graphics/ShaderResourceTable.h>

namespace venom
{
namespace common
{
Transform3D::Transform3D()
    : _position(0.0f, 0.0f, 0.0f)
    , _rotation(vcm::IdentityQuat())
    , __3DrotationViewDirty(true)
    , __positionDirty(true)
    , __modelDirty(true)
#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    , _modelMatrix(ShaderResourceTable::GetModelMatrixBuffer())
#else
    , _modelMatrix(vcm::Identity())
#endif
{
}

Transform3D::~Transform3D()
{
#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    if (_modelMatrix) ShaderResourceTable::ReleaseModelMatrixBuffer(_modelMatrix);
#endif
}

Transform3D::Transform3D(const Transform3D& other)
    : _position(other._position)
    , _rotation(other._rotation)
    , __3DrotationViewDirty(other.__3DrotationViewDirty)
    , __positionDirty(other.__positionDirty)
    , __modelDirty(other.__modelDirty)
    , _3DrotationView(other._3DrotationView)
#ifndef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    , _modelMatrix(other._modelMatrix)
#endif
{
#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    _modelMatrix = ShaderResourceTable::GetModelMatrixBuffer();
    *_modelMatrix = *other._modelMatrix;
#endif
}

Transform3D& Transform3D::operator=(const Transform3D& other)
{
    if (this != &other) {
        _position = other._position;
        _rotation = other._rotation;
        __3DrotationViewDirty = other.__3DrotationViewDirty;
        __positionDirty = other.__positionDirty;
        __modelDirty = other.__modelDirty;
        _3DrotationView = other._3DrotationView;
#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
        _modelMatrix = ShaderResourceTable::GetModelMatrixBuffer();
        *_modelMatrix = *other._modelMatrix;
#else
        _modelMatrix = other._modelMatrix;
#endif
    }
    return *this;
}

Transform3D::Transform3D(Transform3D&& other) noexcept
    : _position(std::move(other._position))
    , _rotation(std::move(other._rotation))
    , __3DrotationViewDirty(std::move(other.__3DrotationViewDirty))
    , __positionDirty(std::move(other.__positionDirty))
    , __modelDirty(std::move(other.__modelDirty))
    , _3DrotationView(std::move(other._3DrotationView))
    , _modelMatrix(std::move(other._modelMatrix))
{
#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    other._modelMatrix = nullptr;
#endif
}

Transform3D& Transform3D::operator=(Transform3D&& other) noexcept
{
    if (this != &other) {
        _position = std::move(other._position);
        _rotation = std::move(other._rotation);
        __3DrotationViewDirty = std::move(other.__3DrotationViewDirty);
        __positionDirty = std::move(other.__positionDirty);
        __modelDirty = std::move(other.__modelDirty);
        _3DrotationView = std::move(other._3DrotationView);
        _modelMatrix = std::move(other._modelMatrix);
#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
        other._modelMatrix = nullptr;
#endif
    }
    return *this;
}

void Transform3D::SetPosition(const vcm::Vec3& position)
{
    _position = position;
    __positionDirty = true;
    __modelDirty = true;
}

void Transform3D::Move(const vcm::Vec3& delta)
{
    _position += delta;
    __positionDirty = true;
    __modelDirty = true;
}

bool Transform3D::HasPositionChanged()
{
    bool ret = __positionDirty;
    __positionDirty = false;
    return ret;
}

void Transform3D::SetRotation(const vcm::Quat& rotation)
{
    _rotation = rotation;
    __3DrotationViewDirty = true;
    __positionDirty = true;
    __modelDirty = true;
}

void Transform3D::Rotate(const vcm::Vec3& axis, float angle)
{
    vcm::RotateQuat(_rotation, angle, axis);
    __3DrotationViewDirty = true;
    __positionDirty = true;
    __modelDirty = true;
}

const vcm::Vec3& Transform3D::GetRotation()
{
    if (__3DrotationViewDirty)
    {
        _3DrotationView = vcm::GetEulerAngles(_rotation);
        __3DrotationViewDirty = false;
    }
    return _3DrotationView;
}

void Transform3D::UpdateModelMatrix()
{
    if (__modelDirty)
    {
        __GetModelMatrix() = vcm::Identity();
        vcm::TranslateMatrix(__GetModelMatrix(), _position);
        vcm::RotateMatrix(__GetModelMatrix(), _rotation);
        __modelDirty = false;
    }
}

const vcm::Mat4& Transform3D::GetModelMatrix()
{
    UpdateModelMatrix();
    return __GetModelMatrix();
}
}
}