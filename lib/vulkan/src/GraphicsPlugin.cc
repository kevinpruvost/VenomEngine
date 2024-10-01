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
#include <venom/vulkan/plugin/graphics/Texture.h>

namespace venom
{
namespace vulkan
{
vc::GraphicsApplication* VulkanGraphicsPlugin::CreateGraphicsApplication()
{
    return new VulkanApplication();
}

vc::Model* VulkanGraphicsPlugin::CreateModel()
{
    return new VulkanModel();
}

vc::Mesh * VulkanGraphicsPlugin::CreateMesh()
{
    return new VulkanMesh();
}

vc::Texture* VulkanGraphicsPlugin::CreateTexture()
{
    return new VulkanTexture();
}
}
}

extern "C" EXPORT vc::GraphicsPlugin* createGraphicsPlugin() {
    return new venom::vulkan::VulkanGraphicsPlugin();
}