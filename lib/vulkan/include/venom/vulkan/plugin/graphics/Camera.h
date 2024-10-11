///
/// Project: VenomEngineWorkspace
/// @file Camera.h
/// @date Oct, 11 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Camera.h>

namespace venom
{
namespace vulkan
{
class VulkanCamera : public vc::Camera
{
public:
    VulkanCamera();
    ~VulkanCamera();
};
}
}