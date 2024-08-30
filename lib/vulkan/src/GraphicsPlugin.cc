///
/// Project: VenomEngine
/// @file GraphicsPlugin.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/GraphicsPlugin.h>

#include <venom/vulkan/VulkanApplication.h>
#include <venom/vulkan/plugin/graphics/Mesh.h>

namespace venom
{
namespace vulkan
{
vc::GraphicsApplication* VulkanGraphicsPlugin::CreateGraphicsApplication()
{
    return new VulkanApplication();
}

vc::Mesh* VulkanGraphicsPlugin::CreateMesh()
{
    return new VulkanMesh();
}
}
}

extern "C" EXPORT vc::GraphicsPlugin* createGraphicsPlugin() {
    return new venom::vulkan::VulkanGraphicsPlugin();
}