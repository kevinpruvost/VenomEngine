///
/// Project: VenomEngine
/// @file Texture.h
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Texture.h>

#include <venom/vulkan/Buffer.h>
#include <venom/vulkan/Image.h>
#include <venom/vulkan/ImageView.h>
#include <venom/vulkan/Sampler.h>

namespace venom
{
namespace vulkan
{

class VulkanTextureResource : public vc::TextureResource
{
public:
    VulkanTextureResource();

    Image image;
    vc::Vector<ImageView> imageViews;
};

class VulkanTexture : public vc::TextureImpl
{
public:
    VulkanTexture();
    ~VulkanTexture();

    static VulkanTexture * GetDummyTexture();
    static void SetDummyTexture(VulkanTexture * texture);

    void _ResetResource() override;

    vc::Error LoadImage(unsigned char * pixels, int width, int height, int channels) override;
    vc::Error LoadImageRGBA(unsigned char * pixels, int width, int height, int channels) override;
    vc::Error LoadImage(uint16_t * pixels, int width, int height, int channels) override;
    vc::Error _InitDepthBuffer(int width, int height) override;
    vc::Error _CreateAttachment(int width, int height, int imageCount, vc::ShaderVertexFormat format) override;
    vc::Error _CreateReadWriteTexture(int width, int height, vc::ShaderVertexFormat format, int mipLevels, int arrayLayers) override;
    vc::Error _CreateShadowMaps(int dimension, int arrayLayers) override;

    vc::Error _SetMemoryAccess(const vc::TextureMemoryAccess access) override;

    class VulkanGUITexture : public vc::TextureImpl::GUITexture
    {
    public:
        VulkanGUITexture() = default;
        ~VulkanGUITexture() override = default;

        vc::Error _LoadTextureToGUI(vc::TextureImpl* impl, void** ptrToGuiTextureId) override;
        vc::Error _UnloadTextureFromGUI(void* guiTextureId) override;
    };
    GUITexture * _NewGuiTextureInstance() override;

    int GetHeight() const override;
    int GetWidth() const override;
    void SetDimensions(int width, int height) override;

    bool HasTexture() const override { return _resource && GetImage().GetVkImage() != VK_NULL_HANDLE; }

    inline const Image & GetImage() const { return _resource->As<VulkanTextureResource>()->image; }
    inline Image & GetImage() { return _resource->As<VulkanTextureResource>()->image; }
    inline const ImageView & GetImageView(int index = 0) const { return _resource->As<VulkanTextureResource>()->imageViews[index]; }
    inline ImageView & GetImageView(int index = 0) { return _resource->As<VulkanTextureResource>()->imageViews[index]; }
    inline ImageView & CreateImageView() const { return _resource->As<VulkanTextureResource>()->imageViews.emplace_back(); }
};

}
}
