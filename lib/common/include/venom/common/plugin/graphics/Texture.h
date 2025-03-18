///
/// Project: VenomEngine
/// @file Texture.h
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPlugin.h>
#include <venom/common/plugin/graphics/ShaderResourceTable.h>

#include <venom/common/Types.h>

#include <venom/common/plugin/graphics/ShaderPipeline.h>

namespace venom
{
namespace common
{
class GraphicsApplication;
class Texture;

enum class TextureMemoryAccess
{
    None = -1,
    ReadOnly = 0,
    WriteOnly,
    ReadWrite,
    Count
};

enum class TextureType
{
    None = -1,
    Texture2D = 0,
    TextureCube,
    Texture3D,
    Count
};

enum TextureUsage
{
    None = -1,
    Sampled = 0x1,
    Storage = 0x2,
    Attachment = 0x4,
    DepthStencil = 0x8,
    Immutable = 0x10,
    TextureUsageCount
};

class VENOM_COMMON_API TextureResource : public GraphicsCachedResource
{
public:
    TextureResource();
    virtual ~TextureResource();

#if defined(VENOM_BINDLESS_TEXTURES)
    inline int GetTextureID() const { return __textureID; }
private:
    const int __textureID;
#endif
};

vc::Error VENOM_COMMON_API SaveImageToExr(const void * data, const char * path, int width, int height, int channels = 4);
vc::Error VENOM_COMMON_API SaveImageToPng(const void * data, const char * path, int width, int height, int channels = 4);

class VENOM_COMMON_API TextureImpl : public GraphicsPluginObject, public GraphicsCachedResourceHolder
{
public:
    TextureImpl();
    virtual ~TextureImpl();

    vc::Error SaveImageToFile(const char * path);
    vc::Error LoadImageFromFile(const char * path);
    vc::Error LoadImage(const char * path, int id, char * bgraData, unsigned int width, unsigned int height);
    vc::Error InitDepthBuffer(int width, int height);
    /**
     * @brief Corresponds to Storage Images / Sampled Images for Vulkan for instance
     * Switch between layouts or types is handled automatically
     * @param width
     * @param height
     * @param format
     * @param mipMapLevels
     * @param arrayLayers
     * @return
     */
    vc::Error CreateReadWriteTexture(int width, int height, vc::ShaderVertexFormat format, int mipMapLevels, int arrayLayers = 1);
    vc::Error CreateAttachment(int width, int height, int imageCount, vc::ShaderVertexFormat format);
    vc::Error CreateShadowMaps(int dimension);
    vc::Error CreateShadowCubeMaps(int dimension);
#if defined(VENOM_BINDLESS_TEXTURES)
    inline int GetTextureID() const { return _GetResourceToCache()->As<TextureResource>()->GetTextureID(); }
#endif
    virtual bool HasTexture() const = 0;

    vc::Error SetMemoryAccess(const TextureMemoryAccess access);

    virtual vc::Error LoadImage(unsigned char * pixels, int width, int height, int channels) = 0;
    virtual vc::Error LoadImageRGBA(unsigned char * pixels, int width, int height, int channels) = 0;
    virtual vc::Error LoadImage(uint16_t * pixels, int width, int height, int channels) = 0;
    inline void SetTexturePeakLuminance(float peakLuminance) { __peakLuminance = peakLuminance; }
    inline void SetTextureAverageLuminance(float averageLuminance) { __averageLuminance = averageLuminance; }
    inline const float & GetTexturePeakLuminance() const { return __peakLuminance; }
    inline const float & GetTextureAverageLuminance() const { return __averageLuminance; }
    inline const TextureMemoryAccess & GetMemoryAccess() const { return __memoryAccess; }
    bool operator==(const GraphicsCachedResource * res) const;

    vc::Error GetGUITextureID(void ** ptrToTextureId);

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual void SetDimensions(int width, int height) = 0;

protected:
    virtual vc::Error _InitDepthBuffer(int width, int height) = 0;
    virtual vc::Error _CreateAttachment(int width, int height, int imageCount, vc::ShaderVertexFormat format) = 0;
    virtual vc::Error _SetMemoryAccess(const TextureMemoryAccess access) = 0;
    virtual vc::Error _CreateReadWriteTexture(int width, int height, vc::ShaderVertexFormat format, int mipMapLevels, int arrayLayers) = 0;
    virtual vc::Error _CreateShadowMaps(int dimension) = 0;
    virtual vc::Error _CreateShadowCubeMaps(int dimension) = 0;
    virtual vc::Error _SaveImageToFile(const char * path) = 0;

private:
    friend class Texture;
    void __CreateDummyTexture();

protected:
    TextureType _textureType;
    TextureUsage _textureUsage;

public:
    /**
     * Has to be specialized by the Graphics API
     * Contains GUI Texture ID, especially for IMGUI
     */
    class VENOM_COMMON_API GUITexture
    {
    public:
        GUITexture();
        virtual ~GUITexture() = default;

        void * GetTextureId();
        vc::Error LoadTextureToGUI(vc::TextureImpl * impl);
        vc::Error UnloadTextureFromGUI();

    protected:
        virtual vc::Error _UnloadTextureFromGUI(void * guiTextureId) = 0;
        virtual vc::Error _LoadTextureToGUI(vc::TextureImpl * impl, void ** ptrToGuiTextureId) = 0;

    private:
        void * __guiTextureId;
    };
protected:
    virtual GUITexture * _NewGuiTextureInstance() = 0;
    static void UnloadAllGuiTextures();

private:
    TextureMemoryAccess __memoryAccess;
    float __peakLuminance, __averageLuminance;

    UPtr<GUITexture> __guiTexture;
};

enum class ColorAttachmentType
{
    Present = 0,
    NormalSpecular = 1,
    MetallicRougnessAO = 2,
    Position = 3,
    Depth = 4,
    LightingAddition = 5,
    Count
};

class VENOM_COMMON_API Texture : public PluginObjectWrapper
{
public:
    Texture();
    Texture(const char * path);
    Texture(const char * path, int id, char * bgraData, unsigned int width, unsigned int height);
    ~Texture();

    bool operator==(const GraphicsCachedResource * res) const { return _impl->As<TextureImpl>()->operator==(res); }
    bool operator==(const vc::SPtr<GraphicsCachedResource> & res) const { return _impl->As<TextureImpl>()->operator==(res.get()); }
    inline vc::Error LoadImageFromFile(const char * path) { return _impl->As<TextureImpl>()->LoadImageFromFile(path); }
    inline vc::Error LoadImage(const char * path, int id, char * bgraData, unsigned int width, unsigned int height) { return _impl->As<TextureImpl>()->LoadImage(path, id, bgraData, width, height); }
    inline void LoadImageFromCachedResource(const SPtr<GraphicsCachedResource> res) { _impl->As<TextureImpl>()->SetResource(res); }
    inline vc::Error InitDepthBuffer(int width, int height) { return _impl->As<TextureImpl>()->InitDepthBuffer(width, height); }
    inline vc::Error CreateAttachment(int width, int height, int imageCount, vc::ShaderVertexFormat format) { return _impl->As<TextureImpl>()->CreateAttachment(width, height, imageCount, format); }
    inline vc::Error CreateReadWriteTexture(int width, int height, vc::ShaderVertexFormat format, int mipMapLevels, int arraLayers = 1) { return _impl->As<TextureImpl>()->CreateReadWriteTexture(width, height, format, mipMapLevels, arraLayers); }
    inline vc::Error CreateShadowMaps(int dimension) { return _impl->As<TextureImpl>()->CreateShadowMaps(dimension); }
    inline vc::Error CreateShadowCubeMaps(int dimension) { return _impl->As<TextureImpl>()->CreateShadowCubeMaps(dimension); }
    inline vc::Error SetMemoryAccess(const TextureMemoryAccess access) { return _impl->As<TextureImpl>()->SetMemoryAccess(access); }
    inline bool HasTexture() const { return _impl->As<TextureImpl>()->HasTexture(); }
    inline const vc::String & GetName() { return _impl->As<TextureImpl>()->GetResourceName(); }
    inline const vc::String & GetShortName() { return _impl->As<TextureImpl>()->GetResourceShortName(); }
    inline int GetWidth() const { return _impl->As<TextureImpl>()->GetWidth(); }
    inline int GetHeight() const { return _impl->As<TextureImpl>()->GetHeight(); }
    inline const TextureMemoryAccess & GetMemoryAccess() const { return _impl->As<TextureImpl>()->GetMemoryAccess(); }
    inline const float & GetTexturePeakLuminance() const { return _impl->As<TextureImpl>()->GetTexturePeakLuminance(); }
    inline const float & GetTextureAverageLuminance() const { return _impl->As<TextureImpl>()->GetTextureAverageLuminance(); }
    inline vc::Error SaveImageToFile(const char * path) { return _impl->As<TextureImpl>()->SaveImageToFile(path); }

    inline vc::Error GetGUITextureID(void ** ptrToTextureId) const { return _impl->As<TextureImpl>()->GetGUITextureID(ptrToTextureId); }
#if defined(VENOM_BINDLESS_TEXTURES)
    inline int GetTextureID() const { return _impl->As<TextureImpl>()->GetTextureID(); }
#endif

    inline static void UnloadAllGuiTextures() { TextureImpl::UnloadAllGuiTextures(); }
private:
    friend class GraphicsApplication;
    inline void __CreateDummyTexture() { _impl->As<TextureImpl>()->__CreateDummyTexture(); }
};

}
}
