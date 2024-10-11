///
/// Project: VenomEngineWorkspace
/// @file Camera.cc
/// @date Oct, 11 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Camera.h>

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

namespace venom
{
namespace common
{
Camera::Camera()
    : GraphicsPluginObject()
    , __position(0.0f, 0.0f, 0.0f)
    , __rotation(vcm::IdentityQuat())
    , __fov(45.0f)
    , __aspect(16.0f / 9.0f)
    , __near(0.1f)
    , __far(100.0f)
    , __viewMatrixDirty(true)
    , __projectionMatrixDirty(true)
{
}

Camera::~Camera()
{
}

Camera* Camera::Create()
{
    return GraphicsPlugin::Get()->CreateCamera();
}

void Camera::SetPosition(const vcm::Vec3& position)
{
    __position = position;
    __viewMatrixDirty = true;
}

void Camera::Move(const vcm::Vec3& delta)
{
    __position += delta;
    __viewMatrixDirty = true;
}

const vcm::Vec3 & Camera::GetPosition() const
{
    return __position;
}

void Camera::SetRotation(const vcm::Quat& rotation)
{
    __rotation = rotation;
    __3DrotationViewDirty = true;
    __viewMatrixDirty = true;
}

void Camera::Rotate(const vcm::Vec3& axis, float angle)
{
    vcm::RotateQuat(__rotation, angle, axis);
    __3DrotationViewDirty = true;
    __viewMatrixDirty = true;
}

const vcm::Quat & Camera::GetRotationQuat() const
{
    return __rotation;
}

const vcm::Vec3& Camera::GetRotation()
{
    if (__3DrotationViewDirty)
    {
        __3DrotationView = vcm::GetEulerAngles(__rotation);
        __3DrotationViewDirty = false;
    }
    return __3DrotationView;
}

const vcm::Mat4& Camera::GetViewMatrix()
{
    if (__viewMatrixDirty)
    {
        __viewMatrix = vcm::LookAt(__position, __position + GetForwardVector(), GetUpVector());
        __viewMatrixDirty = false;
    }
    return __viewMatrix;
}

void Camera::SetPerspective(float fovY, float aspectRatio, float nearPlane, float farPlane)
{
    __fov = fovY;
    __aspect = aspectRatio;
    __near = nearPlane;
    __far = farPlane;

    __projectionMatrixDirty = true;
}

const vcm::Mat4& Camera::GetProjectionMatrix()
{
    if (__projectionMatrixDirty)
    {
        __projectionMatrix = vcm::Perspective(__fov, __aspect, __near, __far);
        __projectionMatrixDirty = false;
    }
    return __projectionMatrix;
}

void Camera::SetFieldOfView(float fovY)
{
    __fov = fovY;
    __projectionMatrixDirty = true;
}

float Camera::GetFieldOfView() const
{
    return __fov;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    __aspect = aspectRatio;
    __projectionMatrixDirty = true;
}

float Camera::GetAspectRatio() const
{
    return __aspect;
}

void Camera::SetNearPlane(float nearPlane)
{
    __near = nearPlane;
    __projectionMatrixDirty = true;
}

float Camera::GetNearPlane() const
{
    return __near;
}

void Camera::SetFarPlane(float farPlane)
{
    __far = farPlane;
    __projectionMatrixDirty = true;
}

float Camera::GetFarPlane() const
{
    return __far;
}

void Camera::LookAt(const vcm::Vec3& target)
{
    auto upVector = GetUpVector();
    __viewMatrix = vcm::LookAt(__position, target, GetUpVector());
    __viewMatrixDirty = false;

    __rotation = vcm::QuatFromViewMatrix(__viewMatrix);
}

vcm::Vec3 Camera::GetForwardVector() const
{
    return vcm::GetForward(__rotation);
}

vcm::Vec3 Camera::GetUpVector() const
{
    return vcm::GetUp(__rotation);
}

vcm::Vec3 Camera::GetRightVector() const
{
    return vcm::GetRight(__rotation);
}
}
}