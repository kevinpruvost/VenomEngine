///
/// Project: VenomEngineWorkspace
/// @file Transform3D.h
/// @date Oct, 21 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/math/Matrix.h>

namespace venom
{
namespace common
{
class Transform3D
{
public:
    Transform3D();

    // Basic movement
    void SetPosition(const vcm::Vec3& position);  // Set position of the camera
    void Move(const vcm::Vec3& delta);            // Move camera by delta
    inline const vcm::Vec3 & GetPosition() const { return _position; }  // Get current camera position
    bool HasPositionChanged();

    // Rotation and orientation
    void SetRotation(const vcm::Quat& rotation);  // Set camera orientation with quaternion
    void Rotate(const vcm::Vec3& axis, float angle); // Rotate camera around axis by angle
    inline const vcm::Quat & GetRotationQuat() const { return _rotation; } // Get current camera rotation
    const vcm::Vec3 & GetRotation();                                       // Get current camera rotation

    void LookAt(const vcm::Vec3& target);  // Point camera towards a specific target
    inline vcm::Vec3 GetForwardVector() const { return vcm::GetForward(_rotation); } // Get camera's forward direction
    inline vcm::Vec3 GetUpVector() const { return vcm::GetUp(_rotation); }           // Get camera's up direction
    inline vcm::Vec3 GetRightVector() const { return vcm::GetRight(_rotation); }     // Get camera's right direction

protected:
    vcm::Vec3 _position;
    vcm::Quat _rotation;
    vcm::Vec3 _3DrotationView;

private:
    bool __3DrotationViewDirty;
    bool __positionDirty;
};
}
}