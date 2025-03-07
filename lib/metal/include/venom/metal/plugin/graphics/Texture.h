///
/// Project: VenomEngine
/// @file Texture.h
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Texture.h>

namespace venom
{
namespace metal
{

class MetalTextureResource : public vc::TextureResource
{
public:
    MetalTextureResource();
};

class MetalTexture : public vc::TextureImpl
{
public:
    MetalTexture();
    ~MetalTexture();

    static MetalTexture * GetDummyTexture();
    static void SetDummyTexture(MetalTexture * texture);

    void _ResetResource() override;

    vc::Error LoadImage(unsigned char * pixels, int width, int height, int channels) override;
    vc::Error LoadImageRGBA(unsigned char * pixels, int width, int height, int channels) override;
    vc::Error LoadImage(uint16_t * pixels, int width, int height, int channels) override;
    vc::Error _InitDepthBuffer(int width, int height) override;
    vc::Error _CreateAttachment(int width, int height, int imageCount, vc::ShaderVertexFormat format) override;
    vc::Error _CreateReadWriteTexture(int width, int height, vc::ShaderVertexFormat format, int mipLevels, int arrayLayers) override;
    vc::Error _CreateShadowMaps(int dimension) override;
    vc::Error _CreateShadowCubeMaps(int dimension) override;
    vc::Error _SaveImageToFile(const char* path) override;
    
    bool HasTexture() const override { return _resource && true; }

    vc::Error _SetMemoryAccess(const vc::TextureMemoryAccess access) override;

    class MetalGUITexture : public vc::TextureImpl::GUITexture
    {
    public:
        MetalGUITexture() = default;
        ~MetalGUITexture() override = default;

        vc::Error _LoadTextureToGUI(vc::TextureImpl* impl, void** ptrToGuiTextureId) override;
        vc::Error _UnloadTextureFromGUI(void* guiTextureId) override;
    };
    GUITexture * _NewGuiTextureInstance() override;

    int GetHeight() const override;
    int GetWidth() const override;
    void SetDimensions(int width, int height) override;
};

}
}
