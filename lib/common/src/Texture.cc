///
/// Project: VenomEngine
/// @file Texture.cc
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

#include <ImfRgbaFile.h>
#include <ImfArray.h>
#include <ImathBox.h>

#include <venom/common/plugin/graphics/Texture.h>
#include <venom/common/Log.h>
#include <venom/common/Resources.h>
#include <filesystem>

#include <flatbuffers/flatbuffers.h>
#define LZ4_DEBUG
#include <lz4hc.h>
#include "Texture_generated.h"

#include <venom/common/FileSystem.h>

namespace venom
{
namespace common
{

Texture::Texture()
    : PluginObjectWrapper(GraphicsPlugin::Get()->CreateTexture())
{
}

Texture::Texture(const char * path)
    : PluginObjectWrapper(GraphicsPlugin::Get()->CreateTexture())
{
    if (Error err = LoadImageFromFile(path); err != Error::Success) {
        Destroy();
        return;
    }
}

Texture::Texture(const char* path, int id, char* bgraData, unsigned int width, unsigned int height)
    : PluginObjectWrapper(GraphicsPlugin::Get()->CreateTexture())
{
    if (Error err = LoadImage(path, id, bgraData, width, height); err != Error::Success) {
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

vc::Error SaveImageToExr(const void* data, const char* path, int width, int height, int channels)
{
    vc::String strPath = path;
    // Check if path has the right extension
    if (strPath.find(".exr") == vc::String::npos) {
        // Adds extension or replace the current extension
        strPath = strPath.substr(0, strPath.find_last_of('.')) + ".exr";
    }

    const uint16_t* imageData = static_cast<const uint16_t*>(data); // assuming float RGBA format

    // Create an image with RGBA channels
    Imf::Array2D<Imf::Rgba> pixels(width, height);
    memcpy(&pixels[0][0], imageData, width * height * channels * sizeof(uint16_t));

    // Create a file header
    Imf::RgbaOutputFile file(strPath.c_str(), width, height, Imf::WRITE_RGBA);
    file.setFrameBuffer(&pixels[0][0], 1, width);
    file.writePixels(height);
    return vc::Error::Success;
}

vc::Error SaveImageToPng(const void* data, const char* path, int width, int height, int channels)
{
    vc::String strPath = path;
    // Check if path has the right extension
    if (strPath.find(".png") == vc::String::npos) {
        // Adds extension or replace the current extension
        strPath = strPath.substr(0, strPath.find_last_of('.')) + ".png";
    }

    // Save image to file
    stbi_write_png(strPath.c_str(), width, height, channels, data, width * channels);
}

TextureImpl::TextureImpl()
    : __peakLuminance(0.0f)
    , __averageLuminance(0.0f)
    , __memoryAccess(TextureMemoryAccess::None)
    , _textureType(TextureType::Texture2D)
    , _textureUsage(TextureUsage::None)
    , __guiTexture(nullptr)
{
}

TextureImpl::~TextureImpl()
{
    if (__guiTexture) {
        __guiTexture->UnloadTextureFromGUI();
    }
}

vc::Error TextureImpl::SaveImageToFile(const char* path)
{
    // Verify if there is already a file at this path
//    if (std::filesystem::exists(path)) {
//        vc::Log::Error("File already exists at path: %s", path);
//        return vc::Error::Failure;
//    }
    return _SaveImageToFile(path);
}

class TextureLoader
{
public:
    TextureLoader() = default;
    virtual ~TextureLoader() = default;

    virtual vc::Error Load(TextureImpl * impl, const char * path) = 0;
    virtual vc::Error SaveToVenomAsset(const char * path) = 0;
    virtual vc::Error LoadFromVenomAsset(TextureImpl * impl, const char * path) = 0;
};

static vc::Error SaveToVenomAssetCommon(const char * path, int width, int height, int channels, uint8_t * pixels, uint8_t pixelSize, float peakLuminance, float averageLuminance)
{
    flatbuffers::FlatBufferBuilder builder(width * height * channels * pixelSize + 20);

    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = builder.CreateVector<uint8_t>(pixels, width * height * channels * pixelSize);
    auto textureData = CreateTextureData(builder, width, height, channels, averageLuminance, peakLuminance, data);

    builder.Finish(textureData);

    const uint8_t * buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();

    vc::Vector<char> compressedData(LZ4_compressBound(size));
    int compressedSize = LZ4_compress_HC(reinterpret_cast<const char*>(buf), compressedData.data(), size, compressedData.size(), LZ4HC_CLEVEL_DEFAULT);
    if (compressedSize <= 0) {
        vc::Log::Error("Failed to compress texture data: %s", path);
        return vc::Error::Failure;
    }

    vc::OFileStream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        vc::Log::Error("Could not write venom asset: %s\n", path);
        return vc::Error::Failure;
    }
    file.write(compressedData.data(), compressedSize);
    file.close();

    return vc::Error::Success;
}

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
        __pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
        if (!__pixels) {
            vc::Log::Error("Failed to load image from file: %s", path);
            return vc::Error::Failure;
        }
        return impl->LoadImage(__pixels, width, height, channels);
    }

    vc::Error SaveToVenomAsset(const char * path) override
    {
        return SaveToVenomAssetCommon(path, width, height, channels, __pixels, sizeof(unsigned char), 0.0f, 0.0f);
    }

    vc::Error LoadFromVenomAsset(TextureImpl * impl, const char * path)
    {
        vc::IFileStream inFile(path, std::ios::binary | std::ios::ate);
        std::streamsize size = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        std::vector<char> compressedData(size);
        if (!inFile.read(compressedData.data(), size)) {
            return vc::Error::Failure;
        }

         std::vector<char> decompressedData(size * 10);
         int decompressedSize = LZ4_decompress_safe(compressedData.data(), decompressedData.data(), size, decompressedData.size());
        
         if (decompressedSize <= 0) {
             vc::Log::Error("Failed to decompress texture data: %s", path);
             return vc::Error::Failure;
         }

        auto textureData = GetTextureData(decompressedData.data());

        width = textureData->width();
        height = textureData->height();
        channels = textureData->channels();

        return impl->LoadImage((unsigned char *)textureData->data()->data(), width, height, channels);
    }

private:
    int width, height, channels;
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
        Imf::RgbaInputFile file(path);
        const Imf::Header& header = file.header();
        const Imf::ChannelList& channelsList = header.channels();
        Imath::Box2i dw = file.dataWindow();
        width = dw.max.x - dw.min.x + 1;
        height = dw.max.y - dw.min.y + 1;

        // Set channels to 4 (RGBA) since Imf::Rgba has 4 components
        channels = 4;

        pixelData.reset(new Imf::Rgba[width * height]);
        file.setFrameBuffer(pixelData.get() - dw.min.x - dw.min.y * width, 1, width);
        file.readPixels(dw.min.y, dw.max.y);

        vc::Error err = impl->LoadImage(reinterpret_cast<uint16_t*>(pixelData.get()), width, height, channels);
        if (err != vc::Error::Success) {
            vc::Log::Error("Failed to load image from file: %s", path);
            return err;
        }

        // Calculate peak luminance
        __peakLuminance = 0.0f;
        float luminance;
        __averageLuminance = 0.0f;
        for (int i = 0; i < width * height; ++i) {
            const Imf_3_4::Rgba & rgba = *(pixelData.get() + i);
            luminance = rgba.r * 0.2126 + rgba.g * 0.7152 + rgba.b * 0.0722;
            __peakLuminance = std::max(__peakLuminance, luminance);
            __averageLuminance += luminance;
        }
        __averageLuminance /= width * height;

        impl->SetTextureAverageLuminance(__averageLuminance);
        impl->SetTexturePeakLuminance(__peakLuminance);
        return err;
    }

    vc::Error SaveToVenomAsset(const char * path) override
    {
        return SaveToVenomAssetCommon(path, width, height, channels, reinterpret_cast<uint8_t *>(pixelData.get()), sizeof(uint16_t), __peakLuminance, __averageLuminance);
    }

    vc::Error LoadFromVenomAsset(TextureImpl * impl, const char * path)
    {
        vc::IFileStream inFile(path, std::ios::binary | std::ios::ate);
        std::streamsize size = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        std::vector<char> compressedData(size);
        if (!inFile.read(compressedData.data(), size)) {
            return vc::Error::Failure;
        }

         std::vector<char> decompressedData(size * 4);
         int decompressedSize = LZ4_decompress_safe(compressedData.data(), decompressedData.data(), size, decompressedData.size());
        
         if (decompressedSize <= 0) {
             vc::Log::Error("Failed to decompress texture data: %s", path);
             return vc::Error::Failure;
         }

        auto textureData = GetTextureData(decompressedData.data());

        width = textureData->width();
        height = textureData->height();
        channels = textureData->channels();
        __averageLuminance = textureData->average_luminance();
        __peakLuminance = textureData->peak_luminance();
 
        vc::Error err = impl->LoadImage((uint16_t*)(textureData->data()->data()), width, height, channels);
        if (err != vc::Error::Success) {
            vc::Log::Error("Failed to load image from file: %s", path);
            return err;
        }
        impl->SetTextureAverageLuminance(__averageLuminance);
        impl->SetTexturePeakLuminance(__peakLuminance);
        return vc::Error::Success;
    }

private:
    int width, height, channels;
    vc::UPtr<Imf::Rgba> pixelData;
    float __averageLuminance;
    float __peakLuminance;
};

TextureLoader * CreateTextureLoader(const char * path)
{
    // Get extension of the file to select which loader to take after that
    vc::String extension = path;
    extension = extension.substr(extension.find_last_of('.') + 1);
    if (extension == "png" || extension == "jpg" || extension == "jpeg" || extension == "bmp"
     || extension == "tga" || extension == "gif" || extension == "psd") {
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

    _ResetResource();
    UPtr<TextureLoader> loader(CreateTextureLoader(realPath.c_str()));
    if (!loader) {
        vc::Log::Error("Failed to create texture loader for: %s", path);
        return vc::Error::Failure;
    }
    vc::String venomAssetPath = Resources::GetVenomAssetResourcePath(realPath);
    if (vc::Filesystem::Exists(venomAssetPath.c_str())) {
          if (loader->LoadFromVenomAsset(this, venomAssetPath.c_str()) != vc::Error::Success) {
              vc::Log::Error("Failed to load image from venom asset: %s", path);
              return vc::Error::Failure;
          }
    } else
    {
        if (loader->Load(this, realPath.c_str()) != vc::Error::Success) {
            vc::Log::Error("Failed to load image from file: %s", path);
            return vc::Error::Failure;
        }
        if (loader->SaveToVenomAsset(venomAssetPath.c_str()) != vc::Error::Success) {
            vc::Log::Error("Failed to save image to venom asset: %s", path);
            return vc::Error::Failure;
        }
    }
    // Set In Cache
    _SetInCache(realPath, _GetResourceToCache());
    _textureType = TextureType::Texture2D;
    _textureUsage = TextureUsage::Sampled;
    return vc::Error::Success;
}

vc::Error TextureImpl::LoadImage(const char* path, int id, char* bgraData, unsigned int width, unsigned int height)
{
    auto realPath = path + std::to_string(id);

    {
        // Load from cache if already loaded
        vc::SPtr<GraphicsCachedResource> cachedTexture = GraphicsPluginObject::GetCachedObject(realPath);
        if (cachedTexture) {
            _LoadFromCache(cachedTexture);
            return vc::Error::Success;
        }
    }

    _ResetResource();
    int realWidth, realHeight;
    int channels;
    unsigned char* imageData = stbi_load_from_memory(
        reinterpret_cast<unsigned char*>(bgraData),
        width,
        &realWidth,
        &realHeight,
        &channels,
        STBI_rgb_alpha // Use the original number of channels
    );
    if (imageData == nullptr) {
        vc::Log::Error("Failed to load image from memory: %s", path);
        return vc::Error::Failure;
    }

    if (LoadImageRGBA(imageData, realWidth, realHeight, channels) != vc::Error::Success) {
        vc::Log::Error("Failed to load image from memory: %s", path);
        return vc::Error::Failure;
    }
    _SetInCache(realPath, _GetResourceToCache());
    stbi_image_free(imageData);
    _textureType = TextureType::Texture2D;
    _textureUsage = TextureUsage::Sampled;
    return vc::Error::Success;
}

static TextureImpl * s_dummyTexture = nullptr;

bool TextureImpl::operator==(const GraphicsCachedResource* res) const
{
    return _GetResourceToCache().get() == res;
}

static vc::Vector<UPtr<TextureImpl::GUITexture> *> s_guiTextures;
void TextureImpl::UnloadAllGuiTextures()
{
    for (auto& guiTexture : s_guiTextures) {
        if (guiTexture->get() == nullptr) continue;
        guiTexture->get()->UnloadTextureFromGUI();
        guiTexture->reset();
    }
    s_guiTextures.clear();
}

vc::Error TextureImpl::GetGUITextureID(void** ptrToTextureId)
{
    if (__guiTexture == nullptr) {
        __guiTexture.reset(_NewGuiTextureInstance());
        s_guiTextures.emplace_back(&__guiTexture);
        __guiTexture->LoadTextureToGUI(this);
    }
    venom_assert(__guiTexture != nullptr, "Failed to create GUI texture");
    *ptrToTextureId = __guiTexture->GetTextureId();
    return vc::Error::Success;
}

void TextureImpl::__CreateDummyTexture()
{
    venom_assert(s_dummyTexture == nullptr, "Dummy texture already created");
    unsigned char pixels[4] = { 255, 255, 255, 255 };
    LoadImage(pixels, 1, 1, 4);
    s_dummyTexture = this;
    _textureType = TextureType::Texture2D;
    _textureUsage = TextureUsage::Sampled;
}

TextureImpl::GUITexture::GUITexture()
    : __guiTextureId(nullptr)
{
}

void* TextureImpl::GUITexture::GetTextureId()
{
    return __guiTextureId;
}

vc::Error TextureImpl::GUITexture::LoadTextureToGUI(vc::TextureImpl* impl)
{
    vc::Error err = _LoadTextureToGUI(impl, &__guiTextureId);
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to load texture to GUI");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error TextureImpl::GUITexture::UnloadTextureFromGUI()
{
    return _UnloadTextureFromGUI(__guiTextureId);
}

const TextureImpl* TextureImpl::GetDummyTexture()
{
    venom_assert(s_dummyTexture != nullptr, "Dummy texture not created");
    return s_dummyTexture;
}

vc::Error TextureImpl::SetMemoryAccess(const TextureMemoryAccess access)
{
    if (access == __memoryAccess) return vc::Error::Success;
    vc::Error err = _SetMemoryAccess(access);
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to set memory access");
        return vc::Error::Failure;
    }
    __memoryAccess = access;
    return err;
}

vc::Error TextureImpl::InitDepthBuffer(int width, int height)
{
    vc::Error err = _InitDepthBuffer(width, height);
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to create depth buffer");
        return vc::Error::Failure;
    }
    _textureType = TextureType::Texture2D;
    _textureUsage = TextureUsage::DepthStencil;
    return err;
}

vc::Error TextureImpl::CreateReadWriteTexture(int width, int height, vc::ShaderVertexFormat format, int mipLevels, int arrayLayers)
{
    vc::Error err = _CreateReadWriteTexture(width, height, format, mipLevels, arrayLayers);
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to create read write texture");
        return vc::Error::Failure;
    }
    _textureType = TextureType::Texture2D;
    _textureUsage = TextureUsage::Storage;
    __memoryAccess = TextureMemoryAccess::ReadWrite;
    return err;
}

vc::Error TextureImpl::CreateAttachment(int width, int height, int imageCount, vc::ShaderVertexFormat format)
{
    vc::Error err = _CreateAttachment(width, height, imageCount, format);
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to create attachment");
        return vc::Error::Failure;
    }
    _textureType = TextureType::Texture2D;
    _textureUsage = TextureUsage::Attachment;
    return err;
}

vc::Error TextureImpl::CreateShadowMaps(int dimension)
{
    return _CreateShadowMaps(dimension);
}

vc::Error TextureImpl::CreateShadowCubeMaps(int dimension)
{
    return _CreateShadowCubeMaps(dimension);
}
}
}
