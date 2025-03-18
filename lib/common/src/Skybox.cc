///
/// Project: VenomEngineWorkspace
/// @file Skybox.cc
/// @date Oct, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Skybox.h>

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

#include <venom/common/FileSystem.h>
#include <venom/common/Resources.h>
#include <venom/common/plugin/graphics/GraphicsSettings.h>

namespace venom
{
namespace common
{
static SkyboxImpl * s_skybox = nullptr;

SkyboxImpl::SkyboxImpl()
    : __panorama()
{
    venom_assert(s_skybox == nullptr, "SkyboxImpl::SkyboxImpl() : Skybox already exists");
    s_skybox = this;
}

SkyboxImpl::~SkyboxImpl()
{
    venom_assert(s_skybox, "SkyboxImpl::~SkyboxImpl() : Skybox already destroyed");
    s_skybox = nullptr;
}

vc::Error SkyboxImpl::LoadSkybox(const char * texturePath)
{
    // Load Panorama first
    vc::String path = Resources::GetTexturesResourcePath(texturePath);
    vc::Error err =  __panorama.LoadImageFromFile(texturePath);
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to load skybox from file: %s", texturePath);
        return err;
    }
    _shaderData.averageLuminance = __panorama.GetTextureAverageLuminance();
    _shaderData.peakLuminance = __panorama.GetTexturePeakLuminance();
    _shaderData.blurFactor = 0.0f;
    if (err = _LoadSkybox(__panorama); err != vc::Error::Success) {
        vc::Log::Error("Failed to load skybox from file: %s", texturePath);
    }

    // Reset Irradiance and Radiance maps
    __irradianceMap = vc::Texture();
    __radianceMap = vc::Texture();
    __blurMap = vc::Texture();


    //
    // Try loading maps if cached first, otherwise precompute then cache
    //
    vc::String irradiancePath = path + "_irradiance.exr";
    vc::String radiancePath = path + "_radiance.exr";
    vc::String blurPath = path + "_blur.exr";
    if (vc::Filesystem::Exists(irradiancePath.c_str())) {
        __irradianceMap.LoadImageFromFile(irradiancePath.c_str());
    } else {
        __irradianceMap.CreateReadWriteTexture(256, 128, vc::ShaderVertexFormat::Vec4, 1);
        __irradianceMap.SetMemoryAccess(vc::TextureMemoryAccess::ReadWrite);
        vc::ShaderResourceTable::UpdateDescriptor(DSETS_INDEX_MATERIAL, 8, &__irradianceMap);
        if (err = _LoadIrradianceMap(__panorama, __irradianceMap); err != vc::Error::Success) {
            vc::Log::Error("Failed to load irradiance map from file: %s", texturePath);
        }
        if (__irradianceMap.SaveImageToFile(irradiancePath.c_str()) != vc::Error::Success) {
            vc::Log::Error("Failed to save irradiance map to file");
        }
    }

    // TODO: Fix as we generate mipmaps
    //if (vc::Filesystem::Exists(radiancePath.c_str())) {
    //    __radianceMap.LoadImageFromFile(radiancePath.c_str());
    //} else
    {
        if (err = _LoadRadianceMap(__panorama, __radianceMap); err != vc::Error::Success) {
            vc::Log::Error("Failed to load radiance map from file: %s", texturePath);
        }
        if (__radianceMap.SaveImageToFile(radiancePath.c_str()) != vc::Error::Success) {
            vc::Log::Error("Failed to save radiance map to file");
        }
    }

    if (vc::Filesystem::Exists(blurPath.c_str())) {
        __blurMap.LoadImageFromFile(blurPath.c_str());
    } else {
        __blurMap.CreateReadWriteTexture(__panorama.GetWidth(), __panorama.GetHeight(), vc::ShaderVertexFormat::Vec4, 1);
        __blurMap.SetMemoryAccess(vc::TextureMemoryAccess::ReadWrite);
        vc::ShaderResourceTable::UpdateDescriptor(DSETS_INDEX_MATERIAL, 8, &__blurMap);
        if (err = _LoadBlurMap(__panorama, __blurMap); err != vc::Error::Success) {
            vc::Log::Error("Failed to load blur map from file: %s", texturePath);
        }
        if (__blurMap.SaveImageToFile(blurPath.c_str()) != vc::Error::Success) {
            vc::Log::Error("Failed to save blur map to file");
        }
    }

    //
    // Set Descriptor Sets
    //
    __irradianceMap.SetMemoryAccess(vc::TextureMemoryAccess::ReadOnly);
    __radianceMap.SetMemoryAccess(vc::TextureMemoryAccess::ReadOnly);
    __blurMap.SetMemoryAccess(vc::TextureMemoryAccess::ReadOnly);
    vc::ShaderResourceTable::UpdateDescriptor(DSETS_INDEX_PANORAMA, 2, &__irradianceMap);
    vc::ShaderResourceTable::UpdateDescriptor(DSETS_INDEX_PANORAMA, 3, &__radianceMap);
    vc::ShaderResourceTable::UpdateDescriptor(DSETS_INDEX_PANORAMA, 4, &__blurMap);
    return err;
}

vc::Error SkyboxImpl::LoadSkybox(const SPtr<GraphicsCachedResource> res)
{
    __panorama.LoadImageFromCachedResource(res);
    return _LoadSkybox(__panorama);
}

vc::Error SkyboxImpl::ChangeBlurFactor(const float factor)
{
    _shaderData.blurFactor = factor;
    return _ChangeBlurFactor(factor);
}

Skybox::Skybox()
    : PluginObjectWrapper(GraphicsPlugin::Get()->CreateSkybox())
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
        vc::GraphicsSettings::CallbackAfterDraws([newPath]() {
            if (vc::Error err = s_skybox->LoadSkybox(newPath.c_str()); err != vc::Error::Success) {
                vc::Log::Error("Failed to load skybox from file: %s", newPath.c_str());
            }
        });
    }
    if (vc::GUI::SliderFloat("Blur Factor", &s_skybox->_shaderData.blurFactor, 0.0f, 1.0f)) {
        vc::GraphicsSettings::CallbackAfterDraws([]() {
            if (vc::Error err = s_skybox->ChangeBlurFactor(s_skybox->_shaderData.blurFactor); err != vc::Error::Success) {
                vc::Log::Error("Failed to change blur factor");
            }
        });
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
