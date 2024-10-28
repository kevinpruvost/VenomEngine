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
#include <venom/vulkan/plugin/graphics/Shader.h>
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

vc::ShaderImpl* VulkanGraphicsPlugin::CreateShader()
{
    return new VulkanShader();
}

vc::ShaderResourceTable* VulkanGraphicsPlugin::CreateShaderResourceTable()
{
    return new VulkanShaderResourceTable();
}
}
}

extern "C" EXPORT vc::GraphicsPlugin* createGraphicsPlugin() {
    return new venom::vulkan::VulkanGraphicsPlugin();
}