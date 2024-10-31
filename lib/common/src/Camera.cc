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
static std::unique_ptr<Camera> s_mainCamera = nullptr;

CameraImpl::CameraImpl()
    : GraphicsPluginObject()
    , __fov(45.0f)
    , __aspect(16.0f / 9.0f)
    , __near(0.1f)
    , __far(100.0f)
    , __projectionMatrixDirty(true)
{
}

CameraImpl::~CameraImpl()
{
}

Camera::Camera()
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateCamera())
{
    if (s_mainCamera == nullptr)
        SetMainCamera(*this);
}

Camera::~Camera()
{
    if (s_mainCamera.get() == this)
        s_mainCamera.reset();
}

void Camera::SetMainCamera(const Camera & camera)
{
    s_mainCamera = std::make_unique<Camera>(camera);
}

Camera * Camera::GetMainCamera()
{
    return s_mainCamera.get();
}

const vcm::Mat4& CameraImpl::GetViewMatrix()
{
    if (HasPositionChanged()) {
        __viewMatrix = vcm::LookAt(_position, _position + GetForwardVector(), GetUpVector());
    }
    return __viewMatrix;
}

void CameraImpl::SetPerspective(float fovY, float aspectRatio, float nearPlane, float farPlane)
{
    __fov = fovY;
    __aspect = aspectRatio;
    __near = nearPlane;
    __far = farPlane;

    __projectionMatrixDirty = true;
}

const vcm::Mat4& CameraImpl::GetProjectionMatrix()
{
    if (__projectionMatrixDirty)
    {
        __projectionMatrix = vcm::Perspective(__fov, __aspect, __near, __far);
        __projectionMatrixDirty = false;
    }
    return __projectionMatrix;
}

void CameraImpl::SetFieldOfView(float fovY)
{
    __fov = fovY;
    __projectionMatrixDirty = true;
}

float CameraImpl::GetFieldOfView() const
{
    return __fov;
}

void CameraImpl::SetAspectRatio(float aspectRatio)
{
    __aspect = aspectRatio;
    __projectionMatrixDirty = true;
}

float CameraImpl::GetAspectRatio() const
{
    return __aspect;
}

void CameraImpl::SetNearPlane(float nearPlane)
{
    __near = nearPlane;
    __projectionMatrixDirty = true;
}

float CameraImpl::GetNearPlane() const
{
    return __near;
}

void CameraImpl::SetFarPlane(float farPlane)
{
    __far = farPlane;
    __projectionMatrixDirty = true;
}

float CameraImpl::GetFarPlane() const
{
    return __far;
}

void CameraImpl::LookAt(const vcm::Vec3& target)
{
    __viewMatrix = vcm::LookAt(_position, target, GetUpVector());

    _yaw = std::atan2(__viewMatrix[0][2], __viewMatrix[2][2]);
    _pitch = std::asin(-__viewMatrix[1][2]);
    _roll = 0.0f;
    _UpdateRotationQuat();
}
}
}