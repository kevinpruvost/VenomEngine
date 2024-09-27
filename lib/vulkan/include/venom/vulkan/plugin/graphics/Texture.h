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
private:
    Image __image;
};

}
}