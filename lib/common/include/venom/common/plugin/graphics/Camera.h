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

#include <venom/common/math/Matrix.h>

namespace venom
{
namespace common
{
class CameraImpl : public PluginObjectImpl, public GraphicsPluginObject
{
public:
    CameraImpl();
    virtual ~CameraImpl();

    // Basic movement
    void SetPosition(const vcm::Vec3& position);  // Set position of the camera
    void Move(const vcm::Vec3& delta);            // Move camera by delta
    const vcm::Vec3 & GetPosition() const;                // Get current camera position

    // Rotation and orientation
    void SetRotation(const vcm::Quat& rotation);  // Set camera orientation with quaternion
    void Rotate(const vcm::Vec3& axis, float angle); // Rotate camera around axis by angle
    const vcm::Quat & GetRotationQuat() const;                // Get current camera rotation
    const vcm::Vec3 & GetRotation();                // Get current camera rotation

    // View matrix generation
    const vcm::Mat4 & GetViewMatrix();              // Generate and return the view matrix

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
    vcm::Vec3 GetForwardVector() const;    // Get camera's forward direction
    vcm::Vec3 GetUpVector() const;         // Get camera's up direction
    vcm::Vec3 GetRightVector() const;      // Get camera's right direction

private:
    vcm::Vec3 __position;
    vcm::Quat __rotation;
    vcm::Vec3 __3DrotationView;
    vcm::Mat4 __viewMatrix;
    vcm::Mat4 __projectionMatrix;

    // Projection parameters
    float __fov;
    float __aspect;
    float __near;
    float __far;

    // Internal state
    bool __viewMatrixDirty;
    bool __projectionMatrixDirty;
    bool __3DrotationViewDirty;

    void __UpdateViewMatrix();
    void __UpdateProjectionMatrix();
};

/**
 * @brief Camera class
 * Classic layout with position, rotation and projection matrices
 */
class Camera : public PluginObjectImplWrapper
{
public:
    Camera();
    ~Camera();
    static void SetMainCamera(const Camera & camera);
    static Camera & GetMainCamera();

    inline void SetPosition(const vcm::Vec3& position) { _impl->As<CameraImpl>()->SetPosition(position); }
    inline void Move(const vcm::Vec3& delta) { _impl->As<CameraImpl>()->Move(delta); }
    inline const vcm::Vec3 & GetPosition() const { return _impl->As<CameraImpl>()->GetPosition(); }

    inline void SetRotation(const vcm::Quat& rotation) { _impl->As<CameraImpl>()->SetRotation(rotation); }
    inline void Rotate(const vcm::Vec3& axis, float angle) { _impl->As<CameraImpl>()->Rotate(axis, angle); }
    inline const vcm::Quat & GetRotationQuat() const { return _impl->As<CameraImpl>()->GetRotationQuat(); }
    inline const vcm::Vec3 & GetRotation() const { return _impl->As<CameraImpl>()->GetRotation(); }

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
    inline vcm::Vec3 GetForwardVector() const { return _impl->As<CameraImpl>()->GetForwardVector(); }
    inline vcm::Vec3 GetUpVector() const { return _impl->As<CameraImpl>()->GetUpVector(); }
    inline vcm::Vec3 GetRightVector() const { return _impl->As<CameraImpl>()->GetRightVector(); }
};
}
}