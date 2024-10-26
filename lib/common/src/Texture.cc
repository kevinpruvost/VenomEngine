///
/// Project: VenomEngine
/// @file Texture.cc
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <venom/common/plugin/graphics/Texture.h>
#include <venom/common/Log.h>
#include <venom/common/Resources.h>

namespace venom
{
namespace common
{
Texture::Texture()
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateTexture())
{
}

Texture::Texture(const char * path)
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateTexture())
{
    if (Error err = LoadImageFromFile(path); err != Error::Success) {
        Destroy();
        return;
    }
}

Texture::~Texture()
{
}

TextureResource::TextureResource()
#ifdef VENOM_BINDLESS_TEXTURES
    : __textureID(ShaderResourceTable::BindTexture())
#endif
{
}

TextureResource::~TextureResource()
{
#ifdef VENOM_BINDLESS_TEXTURES
    ShaderResourceTable::UnbindTexture(__textureID);
#endif
}

TextureImpl::TextureImpl()
{
}

vc::Error TextureImpl::LoadImageFromFile(const char* path)
{
    auto realPath = Resources::GetTexturesResourcePath(path);

    {
        // Load from cache if already loaded
        std::shared_ptr<GraphicsCachedResource> cachedTexture = GraphicsPluginObject::GetCachedObject(realPath);
        if (cachedTexture) {
            _LoadFromCache(cachedTexture);
            return vc::Error::Success;
        }
    }

    int width, height, channels;
    unsigned char * pixels = stbi_load(realPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels) {
        vc::Log::Error("Failed to load image from file: %s", realPath.c_str());
        return vc::Error::Failure;
    }
    if (_LoadImage(pixels, width, height, channels) != vc::Error::Success) {
        vc::Log::Error("Failed to load image from file: %s", realPath.c_str());
        return vc::Error::Failure;
    }
    stbi_image_free(pixels);

    // Set In Cache
    _SetInCache(realPath, _GetTextureToCache());
    return vc::Error::Success;
}

static TextureImpl * s_dummyTexture = nullptr;
void TextureImpl::__CreateDummyTexture()
{
    venom_assert(s_dummyTexture == nullptr, "Dummy texture already created");
    unsigned char pixels[4] = { 255, 255, 255, 255 };
    _LoadImage(pixels, 1, 1, 4);
    s_dummyTexture = this;
}

const TextureImpl* TextureImpl::GetDummyTexture()
{
    venom_assert(s_dummyTexture != nullptr, "Dummy texture not created");
    return s_dummyTexture;
}

vc::Error TextureImpl::InitDepthBuffer(int width, int height)
{
    return _InitDepthBuffer(width, height);
}
}
}
