///
/// Project: VenomEngine
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
    vc::GraphicsApplication * CreateGraphicsApplication() override;

    vc::Model * CreateModel() override;
    vc::Mesh * CreateMesh() override;
    vc::Texture * CreateTexture() override;
    vc::Material* CreateMaterial() override;
};
}
}

extern "C" EXPORT vc::GraphicsPlugin * createGraphicsPlugin();