///
/// Project: VenomEngineWorkspace
/// @file Transform3D.cc
/// @date Oct, 21 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Transform3D.h>
#include <venom/common/plugin/graphics/ShaderResourceTable.h>

#include <venom/common/plugin/graphics/Light.h>
#include <venom/common/plugin/graphics/Camera.h>

namespace venom
{
namespace common
{
Transform3D::Transform3D()
    : _position(0.0f, 0.0f, 0.0f)
    , _scale(1.0f, 1.0f, 1.0f)
    , _rotationQuat(vcm::IdentityQuat())
    , __3DrotationViewDirty(true)
    , __positionDirty(true)
    , __modelDirty(true)
    , _yaw(0), _pitch(0), _roll(0)
#if defined(VENOM_EXTERNAL_PACKED_MODEL_MATRIX)
    , _modelMatrix(ShaderResourceTable::GetModelMatrixBuffer())
#else
    , _modelMatrix(vcm::Identity())
#endif
{
}

Transform3D::~Transform3D()
{
#if defined(VENOM_EXTERNAL_PACKED_MODEL_MATRIX)
    if (_modelMatrix) ShaderResourceTable::ReleaseModelMatrixBuffer(_modelMatrix);
#endif
}

Transform3D::Transform3D(const Transform3D& other)
    : _position(other._position)
    , _scale(other._scale)
    , _rotationQuat(other._rotationQuat)
    , __3DrotationViewDirty(other.__3DrotationViewDirty)
    , __positionDirty(other.__positionDirty)
    , __modelDirty(other.__modelDirty)
    , _yaw(other._yaw), _pitch(other._pitch), _roll(other._roll)
#ifndef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    , _modelMatrix(other._modelMatrix)
#endif
{
#if defined(VENOM_EXTERNAL_PACKED_MODEL_MATRIX)
    _modelMatrix = ShaderResourceTable::GetModelMatrixBuffer();
    *_modelMatrix = *other._modelMatrix;
#endif
}

Transform3D& Transform3D::operator=(const Transform3D& other)
{
    if (this != &other) {
        _position = other._position;
        _scale = other._scale;
        _rotationQuat = other._rotationQuat;
        __3DrotationViewDirty = other.__3DrotationViewDirty;
        __positionDirty = other.__positionDirty;
        __modelDirty = other.__modelDirty;
        _3Drotation = other._3Drotation;
        _yaw = other._yaw;
        _pitch = other._pitch;
        _roll = other._roll;
#if defined(VENOM_EXTERNAL_PACKED_MODEL_MATRIX)
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
    , _scale(std::move(other._scale))
    , _rotationQuat(std::move(other._rotationQuat))
    , __3DrotationViewDirty(std::move(other.__3DrotationViewDirty))
    , __positionDirty(std::move(other.__positionDirty))
    , __modelDirty(std::move(other.__modelDirty))
    , _modelMatrix(std::move(other._modelMatrix))
    , _yaw(other._yaw)
    , _pitch(other._pitch)
    , _roll(other._roll)
{
#if defined(VENOM_EXTERNAL_PACKED_MODEL_MATRIX)
    other._modelMatrix = nullptr;
#endif
}

Transform3D& Transform3D::operator=(Transform3D&& other) noexcept
{
    if (this != &other) {
        _position = std::move(other._position);
        _scale = std::move(other._scale);
        _rotationQuat = std::move(other._rotationQuat);
        __3DrotationViewDirty = std::move(other.__3DrotationViewDirty);
        __positionDirty = std::move(other.__positionDirty);
        __modelDirty = std::move(other.__modelDirty);
        _3Drotation = std::move(other._3Drotation);
        _modelMatrix = std::move(other._modelMatrix);
        _yaw = other._yaw;
        _pitch = other._pitch;
        _roll = other._roll;
#if defined(VENOM_EXTERNAL_PACKED_MODEL_MATRIX)
        other._modelMatrix = nullptr;
#endif
    }
    return *this;
}

void Transform3D::Update(Entity entity)
{
}

void Transform3D::Init(Entity entity)
{
}

void Transform3D::_GUI(const Entity entity)
{
    vcm::Vec3 position = GetPosition();
    if (vc::GUI::SliderFloat3("Position", &position.x, -100.0f, 100.0f)) {
        SetPosition(position);
    }
    vcm::Vec3 rotation = GetRotation();
    if (vc::GUI::SliderFloat3("Rotation", &rotation.x, -M_PI, M_PI)) {
        SetRotation(rotation);
    }
    vcm::Vec3 scale = GetScale();
    if (vc::GUI::SliderFloat3("Scale", &scale.x, 0.0f, 10.0f)) {
        SetScale(scale);
    }
}

vc::String Transform3D::_GetComponentTitle()
{
    return ICON_MS_TRANSFORM " Transform3D";
}

bool Transform3D::CanRemove(Entity entity)
{
    return false;
}

void Transform3D::SetPosition(const vcm::Vec3& position)
{
    _position = position;
    __positionDirty = true;
    __modelDirty = true;
}

void Transform3D::SetScale(const vcm::Vec3& scale)
{
    _scale = scale;
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
    bool ret = __positionDirty || __3DrotationViewDirty;
    __positionDirty = false;
    __3DrotationViewDirty = false;
    return ret;
}

void Transform3D::SetRotation(const vcm::Vec3& rotation)
{
    _3Drotation = rotation;
    _UpdateRotationQuat();
}

void Transform3D::SetRawRotation(const vcm::Vec3& rotation)
{
    _3Drotation = rotation;
    _rotationQuat = vcm::FromEulerAngles(_yaw, _pitch, _roll);
}

void Transform3D::SetYaw(float angle)
{
    _yaw = angle;
    _UpdateRotationQuat();
}

void Transform3D::SetPitch(float angle)
{
    _pitch = angle;
    _UpdateRotationQuat();
}

void Transform3D::SetRoll(float angle)
{
    _roll = angle;
    _UpdateRotationQuat();
}

void Transform3D::SetYawPitchRoll(float yaw, float pitch, float roll)
{
    _yaw = yaw;
    _pitch = pitch;
    _roll = roll;
    _UpdateRotationQuat();
}

void Transform3D::RotateYaw(float angle)
{
    _yaw += angle;
    _UpdateRotationQuat();
}

void Transform3D::RotatePitch(float angle)
{
    _pitch += angle;
    _UpdateRotationQuat();
}

void Transform3D::RotateRoll(float angle)
{
    _roll += angle;
    _UpdateRotationQuat();
}

void Transform3D::Rotate(const vcm::Vec3& rotation)
{
    _3Drotation += rotation;
    _UpdateRotationQuat();
}

const vcm::Vec3& Transform3D::GetRotation()
{
    return _3Drotation;
}

const vcm::Vec3& Transform3D::GetScale()
{
    return _scale;
}

void Transform3D::RotateAround(const vcm::Vec3& target, const vcm::Vec3& planeNormal, float angle)
{
    _position = vcm::RotateAround(_position, target, planeNormal, angle);
}

void Transform3D::LookAt(const vcm::Vec3& target)
{
    vcm::Mat4 lookAt = vcm::LookAt(_position, target, vcm::Vec3(0.0f, 1.0f, 0.0f));
    _yaw = std::atan2(lookAt[0][2], lookAt[2][2]);
    _pitch = std::asin(-lookAt[1][2]);
    _roll = 0.0f;
    _UpdateRotationQuat();
}

void Transform3D::UpdateModelMatrix()
{
    if (__modelDirty)
    {
        __GetModelMatrix() = vcm::Identity();
        vcm::TranslateMatrix(__GetModelMatrix(), _position);
        vcm::RotateMatrix(__GetModelMatrix(), _rotationQuat);
        vcm::ScaleMatrix(__GetModelMatrix(), _scale);
        __modelDirty = false;
    }
}

const vcm::Mat4& Transform3D::GetModelMatrix()
{
    UpdateModelMatrix();
    return __GetModelMatrix();
}

vcm::Mat4& Transform3D::GetModelMatrixMut()
{
    UpdateModelMatrix();
    return __GetModelMatrix();
}

void Transform3D::_UpdateRotationQuat()
{
    _rotationQuat = vcm::FromEulerAngles(_yaw, _pitch, _roll);
    __3DrotationViewDirty = true;
    __positionDirty = true;
    __modelDirty = true;
}
}
}
