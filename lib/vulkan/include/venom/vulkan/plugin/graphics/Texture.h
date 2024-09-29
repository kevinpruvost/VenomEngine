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

class VulkanTexture : public vc::Texture
{
public:
    VulkanTexture();
    ~VulkanTexture();

    vc::Error __LoadImage(unsigned char * pixels, int width, int height, int channels) override;
    const Image & GetImage() const;
    const ImageView & GetImageView() const;
    const Sampler & GetSampler() const;
private:
    Image __image;
    ImageView __imageView;
    Sampler __sampler;
};

}
}