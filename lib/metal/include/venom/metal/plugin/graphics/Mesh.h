///
/// Project: VenomEngine
/// @file Mesh.h
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/metal/Debug.h>
#include <venom/common/math/Vector.h>
#include <venom/common/plugin/graphics/Mesh.h>
#include <venom/common/DeferredTrash.h>

#ifdef __OBJC__
#import <simd/simd.h>
#endif

namespace venom
{
namespace metal
{
class MetalMesh : public vc::MeshImpl
{
public:
    MetalMesh();
    ~MetalMesh();
    void Draw() override;
    vc::Error __LoadMeshFromCurrentData() override;

private:
};
}
}
