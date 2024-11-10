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

    vc::ModelImpl * CreateModel() override;
    vc::MeshImpl * CreateMesh() override;
    vc::TextureImpl * CreateTexture() override;
    vc::MaterialImpl * CreateMaterial() override;
    vc::CameraImpl * CreateCamera() override;
    vc::ShaderPipelineImpl * CreateShaderPipeline() override;
    vc::RenderingPipelineImpl * CreateRenderingPipeline() override;
    vc::SkyboxImpl * CreateSkybox() override;

    vc::ShaderResourceTable * CreateShaderResourceTable() override;
    vc::GUI * CreateGUI() override;
};
}
}

extern "C" EXPORT vc::GraphicsPlugin * createGraphicsPlugin();