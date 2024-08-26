///
/// Project: Bazel_Vulkan_Metal
/// @file GraphicsPlugin.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

namespace venom
{
namespace vulkan
{
class VulkanGraphicsPlugin : public vc::GraphicsPlugin
{
public:
    vc::GraphicsApplication * CreateGraphicsApplication();
    vc::Mesh * CreateMesh();
};
}
}

extern "C" EXPORT vc::GraphicsPlugin * createGraphicsPlugin();