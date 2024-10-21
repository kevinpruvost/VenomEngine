///
/// Project: Bazel_Vulkan_Metal
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
    ImageView imageView;
};

class VulkanTexture : public vc::TextureImpl
{
public:
    VulkanTexture();
    ~VulkanTexture();

    vc::Error _LoadImage(unsigned char * pixels, int width, int height, int channels) override;
    vc::Error _InitDepthBuffer(int width, int height) override;
    inline bool HasTexture() const override { return _resource && GetImage().GetVkImage() != VK_NULL_HANDLE; }

    inline const Image & GetImage() const { return _resource->As<VulkanTextureResource>()->image; }
    inline const ImageView & GetImageView() const { return _resource->As<VulkanTextureResource>()->imageView; }
private:
    inline Image & GetImage() { return _resource->As<VulkanTextureResource>()->image; }
    inline ImageView & GetImageView() { return _resource->As<VulkanTextureResource>()->imageView; }
};

}
}