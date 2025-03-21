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

struct CameraCascadedShadowMapData
{
    // Frustums and positions of near and far corners of each cascade
    vcm::Vec3 cascadeFrustumsCorners[VENOM_CSM_TOTAL_CASCADES + 1][4];
    vcm::Vec3 cascadeFrustumsCenters[VENOM_CSM_TOTAL_CASCADES];
    // The ones delimiting each cascade and the near and far planes
    vcm::Vec3 cascadeFrustumsPlanes[VENOM_CSM_TOTAL_CASCADES][4];
    // As in the name, the radius of each cascade (max distance from the center at it still has effect)
    float cascadeFrustumsRadius[VENOM_CSM_TOTAL_CASCADES];
};

struct CameraFocusData
{
    vc::Entity entity;
    vcm::Vec3 planeNormal;
};

class VENOM_COMMON_API CameraImpl : public GraphicsPluginObject
{
public:
    CameraImpl();
    virtual ~CameraImpl();

    // View matrix generation
    const vcm::Mat4 & GetViewMatrix();  // Generate and return the view matrix
    vcm::Mat4 & GetViewMatrixMut();  // Generate and return the view matrix

    // Projection matrix generation
    void SetPerspective(float fovY, float aspectRatio, float nearPlane, float farPlane); // Set perspective projection
    const vcm::Mat4 & GetProjectionMatrix();        // Generate and return the projection matrix

    void SetPosition(const vcm::Vec3& position);
    void Move(const vcm::Vec3& delta);
    void MoveForward(const float delta);
    void MoveRight(const float delta);
    void MoveUp(const float delta);
    const vcm::Vec3 & GetPosition();

    void SetYaw(float angle);
    void SetPitch(float angle);
    void SetRoll(float angle);
    void SetRotation(const vcm::Vec3& rotation);
    inline void SetRawRotation(const vcm::Vec3& rotation) { __transform->SetRawRotation(rotation); }
    void RotateYaw(float angle);
    void RotatePitch(float angle);
    void RotateRoll(float angle);
    void Rotate(const vcm::Vec3 & rotation);
    const vcm::Quat & GetRotationQuat() const;
    const vcm::Vec3 & GetRotation() const;

    vcm::Vec3 GetForwardVector() const;
    vcm::Vec3 GetUpVector() const;
    vcm::Vec3 GetRightVector() const;

    void RotateAround(const vcm::Vec3& target, const vcm::Vec3& planeNormal, float angle);

    void SetFocusEntity(vc::Entity entity);
    vc::Entity GetFocusEntity();
    void RemoveFocusEntity();

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

    const CameraCascadedShadowMapData & GetCascadedShadowMapData();

private:
    Transform3D * __transform;

    CameraFocusData __focusData;

    vcm::Mat4 __viewMatrix;
    vcm::Mat4 __projectionMatrix;

    // Projection parameters
    float __fov;
    float __aspect;
    float __near;
    float __far;

    // Internal state
    bool __projectionMatrixDirty;
    bool __viewMatrixDirty;
    bool __csmDataDirty;

    void __UpdateViewMatrix();
    void __UpdateProjectionMatrix();

    CameraCascadedShadowMapData __csmData;

    friend class Camera;
};

/**
 * @brief Camera class
 * Classic layout with position, rotation and projection matrices
 */
class VENOM_COMMON_API Camera : public Component, public PluginObjectWrapper
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

    inline void SetPosition(const vcm::Vec3& position) { _impl->As<CameraImpl>()->SetPosition(position); }
    inline void Move(const vcm::Vec3& delta) { _impl->As<CameraImpl>()->Move(delta); }
    inline void MoveForward(const float delta) { _impl->As<CameraImpl>()->MoveForward(delta); }
    inline void MoveRight(const float delta) { _impl->As<CameraImpl>()->MoveRight(delta); }
    inline void MoveUp(const float delta) { _impl->As<CameraImpl>()->MoveUp(delta); }
    inline const vcm::Vec3 & GetPosition() const { return _impl->As<CameraImpl>()->GetPosition(); }

    inline void SetYaw(float angle) { _impl->As<CameraImpl>()->SetYaw(angle); }
    inline void SetPitch(float angle) { _impl->As<CameraImpl>()->SetPitch(angle); }
    inline void SetRoll(float angle) { _impl->As<CameraImpl>()->SetRoll(angle); }
    inline void SetRotation(const vcm::Vec3& rotation) { _impl->As<CameraImpl>()->SetRotation(rotation); }
    inline void SetRawRotation(const vcm::Vec3& rotation) { _impl->As<CameraImpl>()->SetRawRotation(rotation); }
    inline void RotateYaw(float angle) { _impl->As<CameraImpl>()->RotateYaw(angle); }
    inline void RotatePitch(float angle) { _impl->As<CameraImpl>()->RotatePitch(angle); }
    inline void RotateRoll(float angle) { _impl->As<CameraImpl>()->RotateRoll(angle); }
    inline void Rotate(const vcm::Vec3 & rotation) { _impl->As<CameraImpl>()->Rotate(rotation); }
    inline const vcm::Quat & GetRotationQuat() const { return _impl->As<CameraImpl>()->GetRotationQuat(); }
    inline const vcm::Vec3 & GetRotation() const { return _impl->As<CameraImpl>()->GetRotation(); }

    inline vcm::Vec3 GetForwardVector() const { return _impl->As<CameraImpl>()->GetForwardVector(); }
    inline vcm::Vec3 GetUpVector() const { return _impl->As<CameraImpl>()->GetUpVector(); }
    inline vcm::Vec3 GetRightVector() const { return _impl->As<CameraImpl>()->GetRightVector(); }

    inline void RotateAround(const vcm::Vec3& target, const vcm::Vec3& planeNormal, float angle) { _impl->As<CameraImpl>()->RotateAround(target, planeNormal, angle); }

    inline const vcm::Mat4 & GetViewMatrix() const { return _impl->As<CameraImpl>()->GetViewMatrix(); }
    inline vcm::Mat4 & GetViewMatrixMut() { return _impl->As<CameraImpl>()->GetViewMatrixMut(); }
    inline void SetPerspective(float fovY, float aspectRatio, float nearPlane, float farPlane) { _impl->As<CameraImpl>()->SetPerspective(fovY, aspectRatio, nearPlane, farPlane); }
    inline const vcm::Mat4 & GetProjectionMatrix() const { return _impl->As<CameraImpl>()->GetProjectionMatrix(); }

    inline void SetFieldOfView(float fovY) { _impl->As<CameraImpl>()->SetFieldOfView(fovY); }
    inline float GetFieldOfView() const { return _impl->As<CameraImpl>()->GetFieldOfView(); }

    inline void SetAspectRatio(float aspectRatio) { _impl->As<CameraImpl>()->SetAspectRatio(aspectRatio); }
    inline float GetAspectRatio() const { return _impl->As<CameraImpl>()->GetAspectRatio(); }

    inline void SetNearPlane(float nearPlane) { _impl->As<CameraImpl>()->SetNearPlane(nearPlane); }
    inline float GetNearPlane() const { return _impl->As<CameraImpl>()->GetNearPlane(); }

    inline void SetFarPlane(float farPlane) { _impl->As<CameraImpl>()->SetFarPlane(farPlane); }
    inline float GetFarPlane() const { return _impl->As<CameraImpl>()->GetFarPlane(); }

    inline void LookAt(const vcm::Vec3& target) { _impl->As<CameraImpl>()->LookAt(target); }

    inline void SetFocusEntity(vc::Entity entity) { _impl->As<CameraImpl>()->SetFocusEntity(entity); }
    inline vc::Entity GetFocusEntity() const { return _impl->As<CameraImpl>()->GetFocusEntity(); }
    inline void RemoveFocusEntity() { _impl->As<CameraImpl>()->RemoveFocusEntity(); }
};
}
}