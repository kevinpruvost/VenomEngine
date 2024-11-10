///
/// Project: VenomEngine
/// @file GraphicsPlugin.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/GraphicsPlugin.h>

#include <venom/vulkan/VulkanApplication.h>
#include <venom/vulkan/plugin/graphics/Model.h>
#include <venom/vulkan/plugin/graphics/Mesh.h>
#include <venom/vulkan/plugin/graphics/Material.h>
#include <venom/vulkan/plugin/graphics/Texture.h>
#include <venom/vulkan/plugin/graphics/Camera.h>
#include <venom/vulkan/plugin/graphics/GUI.h>
#include <venom/vulkan/plugin/graphics/RenderingPipeline.h>
#include <venom/vulkan/plugin/graphics/ShaderPipeline.h>
#include <venom/vulkan/plugin/graphics/Skybox.h>

#include <venom/vulkan/plugin/graphics/ShaderResourceTable.h>

namespace venom
{
namespace vulkan
{
vc::GraphicsApplication * VulkanGraphicsPlugin::CreateGraphicsApplication()
{
    return new VulkanApplication();
}

vc::ModelImpl * VulkanGraphicsPlugin::CreateModel()
{
    return new VulkanModel();
}

vc::MeshImpl * VulkanGraphicsPlugin::CreateMesh()
{
    return new VulkanMesh();
}

vc::TextureImpl * VulkanGraphicsPlugin::CreateTexture()
{
    return new VulkanTexture();
}

vc::MaterialImpl * VulkanGraphicsPlugin::CreateMaterial()
{
    return new VulkanMaterial();
}

vc::CameraImpl * VulkanGraphicsPlugin::CreateCamera()
{
    return new VulkanCamera();
}

vc::ShaderPipelineImpl* VulkanGraphicsPlugin::CreateShaderPipeline()
{
    return new VulkanShaderPipeline();
}

vc::RenderingPipelineImpl* VulkanGraphicsPlugin::CreateRenderingPipeline()
{
    return new VulkanRenderingPipeline();
}

vc::SkyboxImpl* VulkanGraphicsPlugin::CreateSkybox()
{
    return new VulkanSkybox();
}

vc::ShaderResourceTable* VulkanGraphicsPlugin::CreateShaderResourceTable()
{
    return new VulkanShaderResourceTable();
}

vc::GUI* VulkanGraphicsPlugin::CreateGUI()
{
    return new VulkanGUI();
}
}
}

extern "C" EXPORT vc::GraphicsPlugin* createGraphicsPlugin() {
    return new venom::vulkan::VulkanGraphicsPlugin();
}