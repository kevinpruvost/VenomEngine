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
static SkyboxImpl * s_skybox = nullptr;

SkyboxImpl::SkyboxImpl()
    : __panorama()
{
    if (s_skybox == nullptr)
        s_skybox = this;
}

SkyboxImpl::~SkyboxImpl()
{
    if (s_skybox == this)
        s_skybox = nullptr;
}

vc::Error SkyboxImpl::LoadSkybox(const char * texturePath)
{
    vc::Error err =  __panorama.LoadImageFromFile(texturePath);
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to load skybox from file: %s", texturePath);
        return err;
    }
    if (err = _LoadSkybox(__panorama); err != vc::Error::Success) {
        vc::Log::Error("Failed to load skybox from file: %s", texturePath);
    }
    __irradianceMap.CreateReadWriteTexture(256, 128, vc::ShaderVertexFormat::Vec4, 1);
    __irradianceMap.SetMemoryAccess(vc::TextureMemoryAccess::ReadWrite);
    vc::ShaderResourceTable::UpdateDescriptor(DSETS_INDEX_MATERIAL, 2, &__irradianceMap);
    if (err = _LoadIrradianceMap(__panorama, __irradianceMap); err != vc::Error::Success) {
        vc::Log::Error("Failed to load irradiance map from file: %s", texturePath);
    }
    __irradianceMap.SetMemoryAccess(vc::TextureMemoryAccess::ReadOnly);
    if (err = _LoadRadianceMap(__panorama, __radianceMap); err != vc::Error::Success) {
        vc::Log::Error("Failed to load radiance map from file: %s", texturePath);
    }
    __radianceMap.SetMemoryAccess(vc::TextureMemoryAccess::ReadOnly);
    vc::ShaderResourceTable::UpdateDescriptor(DSETS_INDEX_PANORAMA, 2, &__irradianceMap);
    vc::ShaderResourceTable::UpdateDescriptor(DSETS_INDEX_PANORAMA, 3, &__radianceMap);
    return err;
}

vc::Error SkyboxImpl::LoadSkybox(const SPtr<GraphicsCachedResource> res)
{
    __panorama.LoadImageFromCachedResource(res);
    return _LoadSkybox(__panorama);
}

Skybox::Skybox()
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateSkybox())
{
}

Skybox::Skybox(const char* path)
    : Skybox()
{
    if (Error err = LoadSkybox(path); err != Error::Success) {
        Destroy();
        return;
    }
}

Skybox::~Skybox()
{
}

void Skybox::Init(Entity entity)
{
}

void Skybox::Update(Entity entity)
{
}

void Skybox::_GUI(const Entity entity)
{
    vc::String newPath;
    if (vc::GUI::EditableTexture(&GetPanoramaMut(), newPath)) {
        if (vc::Error err = LoadSkybox(newPath.c_str()); err != vc::Error::Success) {
            vc::Log::Error("Failed to load skybox from file: %s", newPath.c_str());
        } else {

        }
    }
}

vc::String Skybox::_GetComponentTitle()
{
    return ICON_MS_VRPANO " Skybox";
}

bool Skybox::CanRemove(Entity entity)
{
    return false;
}

COMPONENT_CAN_ADD_DEFINITION(Skybox,
    return s_skybox == nullptr;
);
}
}
