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
    , __csmDataDirty(true)
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

void Camera::_GUI(const Entity entity)
{
    GetImpl()->As<CameraImpl>()->__transform->_GUI(entity);
}

void Camera::Init(Entity entity)
{
}

void Camera::Update(Entity entity)
{
    GetImpl()->As<CameraImpl>()->__transform = entity.get_mut<Transform3D>();
}

bool Camera::CanRemove(Entity entity)
{
    return true;
}

vc::String Camera::_GetComponentTitle()
{
    return ICON_MS_PHOTO_CAMERA " Camera";
}

const vcm::Mat4& CameraImpl::GetViewMatrix()
{
    if (__transform->HasPositionChanged()) {
        __viewMatrix = vcm::LookAt(__transform->GetPosition(), __transform->GetPosition() + __transform->GetForwardVector(), __transform->GetUpVector());
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
    __csmDataDirty = true;
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

void CameraImpl::SetPosition(const vcm::Vec3& position)
{
    __transform->SetPosition(position);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::Move(const vcm::Vec3& delta)
{
    __transform->Move(delta);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::MoveForward(const float delta)
{
    __transform->Move(__transform->GetForwardVector() * delta);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::MoveRight(const float delta)
{
    __transform->Move(__transform->GetRightVector() * delta);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::MoveUp(const float delta)
{
    __transform->Move(__transform->GetUpVector() * delta);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

const vcm::Vec3 & CameraImpl::GetPosition()
{
    return __transform->GetPosition();
}

void CameraImpl::SetYaw(float angle)
{
    __transform->SetYaw(angle);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::SetPitch(float angle)
{
    __transform->SetPitch(angle);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::SetRoll(float angle)
{
    __transform->SetRoll(angle);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::SetRotation(const vcm::Vec3& rotation)
{
    __transform->SetRotation(rotation);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::RotateYaw(float angle)
{
    __transform->RotateYaw(angle);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::RotatePitch(float angle)
{
    __transform->RotatePitch(angle);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::RotateRoll(float angle)
{
    __transform->RotateRoll(angle);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::Rotate(const vcm::Vec3 & rotation)
{
    __transform->Rotate(rotation);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

const vcm::Quat & CameraImpl::GetRotationQuat() const
{
    return __transform->GetRotationQuat();
}

const vcm::Vec3 & CameraImpl::GetRotation() const
{
    return __transform->GetRotation();
}

vcm::Vec3 CameraImpl::GetForwardVector() const
{
    return __transform->GetForwardVector();
}

vcm::Vec3 CameraImpl::GetUpVector() const
{
    return __transform->GetUpVector();
}

vcm::Vec3 CameraImpl::GetRightVector() const
{
    return __transform->GetRightVector();
}

void CameraImpl::RotateAround(const vcm::Vec3& target, const vcm::Vec3& planeNormal, float angle)
{
    __transform->RotateAround(target, planeNormal, angle);
    __viewMatrixDirty = true;
    __csmDataDirty = true;
}

void CameraImpl::SetFieldOfView(float fovY)
{
    __fov = fovY;
    __projectionMatrixDirty = true;
    __csmDataDirty = true;
}

float CameraImpl::GetFieldOfView() const
{
    return __fov;
}

void CameraImpl::SetAspectRatio(float aspectRatio)
{
    __aspect = aspectRatio;
    __projectionMatrixDirty = true;
    __csmDataDirty = true;
}

float CameraImpl::GetAspectRatio() const
{
    return __aspect;
}

void CameraImpl::SetNearPlane(float nearPlane)
{
    __near = nearPlane;
    __projectionMatrixDirty = true;
    __csmDataDirty = true;
}

float CameraImpl::GetNearPlane() const
{
    return __near;
}

void CameraImpl::SetFarPlane(float farPlane)
{
    __far = farPlane;
    __projectionMatrixDirty = true;
    __csmDataDirty = true;
}

float CameraImpl::GetFarPlane() const
{
    return __far;
}

void CameraImpl::LookAt(const vcm::Vec3& target)
{
    __viewMatrix = vcm::LookAt(__transform->GetPosition(), target, __transform->GetUpVector());

    __transform->SetYawPitchRoll(
        std::atan2(__viewMatrix[0][2], __viewMatrix[2][2]),
        std::asin(-__viewMatrix[1][2]),
        0.0f
    );
}
}
}