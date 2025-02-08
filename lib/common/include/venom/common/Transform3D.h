///
/// Project: VenomEngineWorkspace
/// @file Transform3D.h
/// @date Oct, 21 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/math/Matrix.h>
#include <venom/common/plugin/graphics/ShaderResourceTable.h>
#include <venom/common/ECS.h>

namespace venom
{
namespace common
{
class VENOM_COMMON_API Transform3D : public Component
{
public:
    Transform3D();
    virtual ~Transform3D();
    Transform3D(const Transform3D & other);
    Transform3D & operator=(const Transform3D & other);
    Transform3D(Transform3D && other) noexcept;
    Transform3D & operator=(Transform3D && other) noexcept;

    void Update(Entity entity) override;
    void Init(Entity entity) override;
    void _GUI(const Entity entity) override;
    vc::String _GetComponentTitle() override;
    bool CanRemove(Entity entity) override;

    // Basic movement
    void SetPosition(const vcm::Vec3& position);  // Set position of the camera
    inline void SetRawPosition(const vcm::Vec3& position) { _position = position; }  // Set position of the camera without updating the model matrix
    void SetScale(const vcm::Vec3& scale);        // Set scale of the camera
    inline void SetRawScale(const vcm::Vec3& scale) { _scale = scale; } // Set scale of the camera without updating the model matrix
    void Move(const vcm::Vec3& delta);            // Move camera by delta
    inline void MoveForward(const float delta) { Move(GetForwardVector() * delta); } // Move camera forward by delta
    inline void MoveRight(const float delta) { Move(GetRightVector() * delta); } // Move camera right by delta
    inline void MoveUp(const float delta) { Move(GetUpVector() * delta); } // Move camera up by delta
    inline const vcm::Vec3 & GetPosition() const { return _position; }  // Get current camera position
    bool HasPositionChanged();

    // Rotation and orientation
    void SetRotation(const vcm::Vec3& rotation);  // Set camera orientation with quaternion
    void SetRawRotation(const vcm::Vec3& rotation); // Set camera orientation with quaternion without updating the model matrix
    void SetYaw(float angle);                     // Set camera yaw
    void SetPitch(float angle);                   // Set camera pitch
    void SetRoll(float angle);                    // Set camera roll
    void SetYawPitchRoll(float yaw, float pitch, float roll); // Set camera yaw, pitch and roll
    void RotateYaw(float angle);                  // Rotate camera around Y axis by angle
    void RotatePitch(float angle);                // Rotate camera around X axis by angle
    void RotateRoll(float angle);                 // Rotate camera around Z axis by angle
    void Rotate(const vcm::Vec3 & rotation);     // Rotate camera around X, Y and Z axis by the given angles
    inline const vcm::Quat & GetRotationQuat() const { return _rotationQuat; } // Get current camera rotation
    const vcm::Vec3 & GetRotation();                                       // Get current camera rotation
    const vcm::Vec3 & GetScale();                                          // Get current camera scale


    /**
     * @brief Rotates around the target on the given planeNormal by the given angle
     * @param target
     * @param planeNormal
     * @param angle
     */
    void RotateAround(const vcm::Vec3& target, const vcm::Vec3& planeNormal, float angle); // Rotate camera around target

    void LookAt(const vcm::Vec3& target);  // Point camera towards a specific target
    inline vcm::Vec3 GetForwardVector() const { return vcm::GetForward(_rotationQuat); } // Get camera's forward direction
    inline vcm::Vec3 GetUpVector() const { return vcm::GetUp(_rotationQuat); }           // Get camera's up direction
    inline vcm::Vec3 GetRightVector() const { return vcm::GetRight(_rotationQuat); }     // Get camera's right direction

    void UpdateModelMatrix();
    const vcm::Mat4 & GetModelMatrix(); // Get the model matrix of the camera
    vcm::Mat4 & GetModelMatrixMut(); // Get the model matrix for mutation

#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    inline int GetModelMatrixId() {
        return ShaderResourceTable::GetModelMatrixBufferId(_modelMatrix);
    }
#endif

private:
    /**
     * @brief Get the model matrix of the camera depending on type
     * @return
     */
    inline vcm::Mat4 & __GetModelMatrix() {
#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
        return *_modelMatrix;
#else
        return _modelMatrix;
#endif
    }

protected:
    void _UpdateRotationQuat();

protected:
    vcm::Vec3 _position;
    vcm::Vec3 _scale;
    vcm::Quat _rotationQuat;
    union
    {
        vcm::Vec3 _3Drotation;
        struct
        {
            float _pitch,_yaw, _roll;
        };
    };
#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
    vcm::Mat4 * _modelMatrix;
#else
    vcm::Mat4 _modelMatrix;
#endif

private:
    bool __3DrotationViewDirty;
    bool __positionDirty;
    bool __modelDirty;
};
}
}