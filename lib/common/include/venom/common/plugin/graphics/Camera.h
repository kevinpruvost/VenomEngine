///
/// Project: VenomEngineWorkspace
/// @file Camera.h
/// @date Oct, 11 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPluginObject.h>
#include <venom/common/Error.h>

#include <venom/common/Transform3D.h>

#include <venom/common/math/Matrix.h>

namespace venom
{
namespace common
{
class Camera;

class VENOM_COMMON_API CameraImpl : public PluginObjectImpl, public GraphicsPluginObject
{
public:
    CameraImpl();
    virtual ~CameraImpl();

    // View matrix generation
    const vcm::Mat4 & GetViewMatrix();  // Generate and return the view matrix

    // Projection matrix generation
    void SetPerspective(float fovY, float aspectRatio, float nearPlane, float farPlane); // Set perspective projection
    const vcm::Mat4 & GetProjectionMatrix();        // Generate and return the projection matrix

    // Camera configuration
    void SetFieldOfView(float fovY);              // Set field of view (vertical)
    float GetFieldOfView() const;                 // Get field of view

    void SetAspectRatio(float aspectRatio);       // Set aspect ratio (width / height)
    float GetAspectRatio() const;                 // Get aspect ratio

    void SetNearPlane(float nearPlane);           // Set near clipping plane distance
    float GetNearPlane() const;                   // Get near clipping plane

    void SetFarPlane(float farPlane);             // Set far clipping plane distance
    float GetFarPlane() const;                    // Get far clipping plane

    void LookAt(const vcm::Vec3& target);  // Point camera towards a specific target

private:
    Transform3D * __transform;

    vcm::Mat4 __viewMatrix;
    vcm::Mat4 __projectionMatrix;

    // Projection parameters
    float __fov;
    float __aspect;
    float __near;
    float __far;

    // Internal state
    bool __projectionMatrixDirty;
    bool __3DrotationViewDirty;

    void __UpdateViewMatrix();
    void __UpdateProjectionMatrix();

    friend class Camera;
};

/**
 * @brief Camera class
 * Classic layout with position, rotation and projection matrices
 */
class VENOM_COMMON_API Camera : public Component, public PluginObjectImplWrapper
{
public:
    Camera();
    ~Camera();
    static void SetMainCamera(const Camera & camera);
    static Camera * GetMainCamera();

    void _GUI(const Entity entity) override;
    void Init(Entity entity) override;
    void Update(Entity entity) override;
    bool CanRemove(Entity entity) override;
    vc::String _GetComponentTitle() override;

    inline void SetPosition(const vcm::Vec3& position) { _impl->As<CameraImpl>()->__transform->SetPosition(position); }
    inline void Move(const vcm::Vec3& delta) { _impl->As<CameraImpl>()->__transform->Move(delta); }
    inline void MoveForward(const float delta) { _impl->As<CameraImpl>()->__transform->MoveForward(delta); }
    inline void MoveRight(const float delta) { _impl->As<CameraImpl>()->__transform->MoveRight(delta); }
    inline void MoveUp(const float delta) { _impl->As<CameraImpl>()->__transform->MoveUp(delta); }
    inline const vcm::Vec3 & GetPosition() const { return _impl->As<CameraImpl>()->__transform->GetPosition(); }

    inline void SetYaw(float angle) { _impl->As<CameraImpl>()->__transform->SetYaw(angle); }
    inline void SetPitch(float angle) { _impl->As<CameraImpl>()->__transform->SetPitch(angle); }
    inline void SetRoll(float angle) { _impl->As<CameraImpl>()->__transform->SetRoll(angle); }
    inline void SetRotation(const vcm::Vec3& rotation) { _impl->As<CameraImpl>()->__transform->SetRotation(rotation); }
    inline void RotateYaw(float angle) { _impl->As<CameraImpl>()->__transform->RotateYaw(angle); }
    inline void RotatePitch(float angle) { _impl->As<CameraImpl>()->__transform->RotatePitch(angle); }
    inline void RotateRoll(float angle) { _impl->As<CameraImpl>()->__transform->RotateRoll(angle); }
    inline void Rotate(const vcm::Vec3 & rotation) { _impl->As<CameraImpl>()->__transform->Rotate(rotation); }
    inline const vcm::Quat & GetRotationQuat() const { return _impl->As<CameraImpl>()->__transform->GetRotationQuat(); }
    inline const vcm::Vec3 & GetRotation() const { return _impl->As<CameraImpl>()->__transform->GetRotation(); }

    inline vcm::Vec3 GetForwardVector() const { return _impl->As<CameraImpl>()->__transform->GetForwardVector(); }
    inline vcm::Vec3 GetUpVector() const { return _impl->As<CameraImpl>()->__transform->GetUpVector(); }
    inline vcm::Vec3 GetRightVector() const { return _impl->As<CameraImpl>()->__transform->GetRightVector(); }

    inline void RotateAround(const vcm::Vec3& target, const vcm::Vec3& planeNormal, float angle) { _impl->As<CameraImpl>()->__transform->RotateAround(target, planeNormal, angle); }

    inline const vcm::Mat4 & GetViewMatrix() { return _impl->As<CameraImpl>()->GetViewMatrix(); }
    inline void SetPerspective(float fovY, float aspectRatio, float nearPlane, float farPlane) { _impl->As<CameraImpl>()->SetPerspective(fovY, aspectRatio, nearPlane, farPlane); }
    inline const vcm::Mat4 & GetProjectionMatrix() { return _impl->As<CameraImpl>()->GetProjectionMatrix(); }

    inline void SetFieldOfView(float fovY) { _impl->As<CameraImpl>()->SetFieldOfView(fovY); }
    inline float GetFieldOfView() const { return _impl->As<CameraImpl>()->GetFieldOfView(); }

    inline void SetAspectRatio(float aspectRatio) { _impl->As<CameraImpl>()->SetAspectRatio(aspectRatio); }
    inline float GetAspectRatio() const { return _impl->As<CameraImpl>()->GetAspectRatio(); }

    inline void SetNearPlane(float nearPlane) { _impl->As<CameraImpl>()->SetNearPlane(nearPlane); }
    inline float GetNearPlane() const { return _impl->As<CameraImpl>()->GetNearPlane(); }

    inline void SetFarPlane(float farPlane) { _impl->As<CameraImpl>()->SetFarPlane(farPlane); }
    inline float GetFarPlane() const { return _impl->As<CameraImpl>()->GetFarPlane(); }

    inline void LookAt(const vcm::Vec3& target) { _impl->As<CameraImpl>()->LookAt(target); }
};
}
}