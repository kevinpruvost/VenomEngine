///
/// Project: VenomEngine
/// @file GraphicsPlugin.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/GraphicsPlugin.h>

#include <venom/metal/MetalApplication.h>
#include <venom/metal/plugin/graphics/Model.h>
#include <venom/metal/plugin/graphics/Mesh.h>
#include <venom/metal/plugin/graphics/Material.h>
#include <venom/metal/plugin/graphics/Texture.h>
#include <venom/metal/plugin/graphics/Camera.h>
#include <venom/metal/plugin/graphics/GUI.h>
#include <venom/metal/plugin/graphics/RenderingPipeline.h>
#include <venom/metal/plugin/graphics/RenderTarget.h>
#include <venom/metal/plugin/graphics/ShaderPipeline.h>
#include <venom/metal/plugin/graphics/Skybox.h>
#include <venom/metal/plugin/graphics/Light.h>

#include <venom/metal/plugin/graphics/ShaderResourceTable.h>

namespace venom
{
namespace metal
{
vc::GraphicsApplication * MetalGraphicsPlugin::CreateGraphicsApplication(int argc, const char *argv[])
{
    return new MetalApplication();
}

vc::ModelImpl * MetalGraphicsPlugin::CreateModel()
{
    return new MetalModel();
}

vc::MeshImpl * MetalGraphicsPlugin::CreateMesh()
{
    return new MetalMesh();
}

vc::TextureImpl * MetalGraphicsPlugin::CreateTexture()
{
    return new MetalTexture();
}

vc::MaterialImpl * MetalGraphicsPlugin::CreateMaterial()
{
    return new MetalMaterial();
}

vc::CameraImpl * MetalGraphicsPlugin::CreateCamera()
{
    return new MetalCamera();
}

vc::ShaderPipelineImpl* MetalGraphicsPlugin::CreateShaderPipeline()
{
    return new MetalShaderPipeline();
}

vc::RenderingPipelineImpl* MetalGraphicsPlugin::CreateRenderingPipeline()
{
    return new MetalRenderingPipeline();
}

vc::SkyboxImpl* MetalGraphicsPlugin::CreateSkybox()
{
    return new MetalSkybox();
}

vc::RenderTargetImpl* MetalGraphicsPlugin::CreateRenderTarget()
{
    return new MetalRenderTarget();
}

vc::LightImpl* MetalGraphicsPlugin::CreateLight()
{
    return new MetalLight();
}

vc::ShaderResourceTable* MetalGraphicsPlugin::CreateShaderResourceTable()
{
    return new MetalShaderResourceTable();
}

vc::GUI* MetalGraphicsPlugin::CreateGUI()
{
    return new MetalGUI();
}
}
}

extern "C" EXPORT vc::GraphicsPlugin* createGraphicsPlugin() {
    return new venom::metal::MetalGraphicsPlugin();
}
