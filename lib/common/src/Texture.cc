///
/// Project: VenomEngine
/// @file Texture.cc
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <ImfRgbaFile.h>
#include <ImfArray.h>
#include <ImathBox.h>

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

class TextureLoader
{
public:
    TextureLoader() = default;
    virtual ~TextureLoader() = default;

    virtual vc::Error Load(TextureImpl * impl, const char * path) = 0;
};

class Stbi_TextureLoader : public TextureLoader
{
public:
    Stbi_TextureLoader() : __pixels(nullptr) { }
    ~Stbi_TextureLoader() override
    {
        if (__pixels) stbi_image_free(__pixels);
    }

    vc::Error Load(TextureImpl * impl, const char * path) override
    {
        int width, height, channels;
        __pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
        if (!__pixels) {
            vc::Log::Error("Failed to load image from file: %s", path);
            return vc::Error::Failure;
        }
        return impl->LoadImage(__pixels, width, height, channels);
    }

private:
    unsigned char * __pixels;
};

class EXR_TextureLoader : public TextureLoader
{
public:
    EXR_TextureLoader() {}
    ~EXR_TextureLoader()
    {
    }

    vc::Error Load(TextureImpl * impl, const char * path) override
    {
        int width, height, channels;

        Imf::RgbaInputFile file(path);
        const Imf::Header& header = file.header();
        const Imf::ChannelList& channelsList = header.channels();
        Imath::Box2i dw = file.dataWindow();
        width = dw.max.x - dw.min.x + 1;
        height = dw.max.y - dw.min.y + 1;

        // Set channels to 4 (RGBA) since Imf::Rgba has 4 components
        channels = 4;

        __pixels.resize(width * height * channels);

        Imf::Array2D<Imf::Rgba> pixelData;
        pixelData.resizeErase(height, width);
        file.setFrameBuffer(&pixelData[0][0] - dw.min.x - dw.min.y * width, 1, width);
        file.readPixels(dw.min.y, dw.max.y);

        // Copy pixelData to pixels
        memcpy(__pixels.data(), &pixelData[0][0], width * height * channels * sizeof(uint16_t));

        return impl->LoadImage(__pixels.data(), width, height, channels);
    }

private:
    std::vector<uint16_t> __pixels;
};

TextureLoader * CreateTextureLoader(const char * path)
{
    // Get extension of the file to select which loader to take after that
    vc::String extension = path;
    extension = extension.substr(extension.find_last_of('.') + 1);
    if (extension == "png" || extension == "jpg" || extension == "jpeg" || extension == "bmp"
     || extension == "tga" || extension == "gif" || extension == "psd"  || extension == "hdr") {
        return new Stbi_TextureLoader();
    } else if (extension == "exr" || extension == "hdr") {
        return new EXR_TextureLoader();
    } else {
        vc::Log::Error("Unsupported image format: %s for %s", extension.c_str(), path);
    }
    return nullptr;
}

vc::Error TextureImpl::LoadImageFromFile(const char* path)
{
    auto realPath = Resources::GetTexturesResourcePath(path);

    {
        // Load from cache if already loaded
        vc::SPtr<GraphicsCachedResource> cachedTexture = GraphicsPluginObject::GetCachedObject(realPath);
        if (cachedTexture) {
            _LoadFromCache(cachedTexture);
            return vc::Error::Success;
        }
    }

    UPtr<TextureLoader> loader(CreateTextureLoader(realPath.c_str()));
    if (!loader || loader->Load(this, realPath.c_str()) != vc::Error::Success) {
        vc::Log::Error("Failed to load image from file: %s", path);
        return vc::Error::Failure;
    }
    // Set In Cache
    _SetInCache(realPath, _GetTextureToCache());
    return vc::Error::Success;
}

static TextureImpl * s_dummyTexture = nullptr;
void TextureImpl::__CreateDummyTexture()
{
    venom_assert(s_dummyTexture == nullptr, "Dummy texture already created");
    unsigned char pixels[4] = { 255, 255, 255, 255 };
    LoadImage(pixels, 1, 1, 4);
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
