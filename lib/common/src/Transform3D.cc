///
/// Project: VenomEngineWorkspace
/// @file Transform3D.cc
/// @date Oct, 21 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Transform3D.h>

namespace venom
{
namespace common
{
Transform3D::Transform3D()
    : _position(0.0f, 0.0f, 0.0f)
    , _rotation(vcm::IdentityQuat())
    , __3DrotationViewDirty(true)
    , __positionDirty(true)
{
}

void Transform3D::SetPosition(const vcm::Vec3& position)
{
    _position = position;
    __positionDirty = true;
}

void Transform3D::Move(const vcm::Vec3& delta)
{
    _position += delta;
    __positionDirty = true;
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
}

void Transform3D::Rotate(const vcm::Vec3& axis, float angle)
{
    vcm::RotateQuat(_rotation, angle, axis);
    __3DrotationViewDirty = true;
    __positionDirty = true;
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
}
}