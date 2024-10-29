///
/// Project: VenomEngineWorkspace
/// @file Skybox.cc
/// @date Oct, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Skybox.h>

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

namespace venom
{
namespace common
{
SkyboxImpl::SkyboxImpl()
    : __panorama()
{
}

vc::Error SkyboxImpl::LoadSkybox(const char * texturePath)
{
    vc::Error err =  __panorama.LoadImageFromFile(texturePath);
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to load skybox from file: %s", texturePath);
        return err;
    }
    __shader.AddVertexBufferToLayout(ShaderVertexFormat::Vec3, 0, 0, 0);
    if (err = __shader.LoadShaderFromFile("skybox"); err != vc::Error::Success) {
        vc::Log::Error("Failed to load skybox shader");
        return err;
    }
    err = _LoadSkybox(__panorama);
    return err;
}

Skybox::Skybox()
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateSkybox())
{
}

Skybox::Skybox(const char* path)
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateSkybox())
{
    if (Error err = LoadSkybox(path); err != Error::Success) {
        Destroy();
        return;
    }
}

Skybox::~Skybox()
{
}
}
}