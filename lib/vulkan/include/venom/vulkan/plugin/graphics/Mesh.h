///
/// Project: Bazel_Vulkan_Metal
/// @file Mesh.h
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>
#include <venom/common/math/Vector.h>
#include <venom/common/plugin/graphics/Mesh.h>

namespace venom
{
namespace vulkan
{
class VulkanMesh : public vc::Mesh
{
    void Draw();
};
}
}
