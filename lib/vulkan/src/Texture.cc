///
/// Project: VenomEngine
/// @file Texture.cc
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/Texture.h>

#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/QueueManager.h>
#include <venom/vulkan/PhysicalDevice.h>

#include "venom/common/plugin/graphics/GraphicsSettings.h"

namespace venom
{
namespace vulkan
{
VulkanTextureResource::VulkanTextureResource()
{
}

VulkanTexture::VulkanTexture()
    : TextureImpl()
{
    _resource.reset(new VulkanTextureResource());
}

VulkanTexture::~VulkanTexture()
{
}

static VulkanTexture * s_dummyTexture = nullptr;
VulkanTexture* VulkanTexture::GetDummyTexture()
{
    return s_dummyTexture;
}

void VulkanTexture::SetDummyTexture(VulkanTexture* texture)
{
    s_dummyTexture = texture;
}

vc::Error VulkanTexture::LoadImage(unsigned char* pixels, int width, int height, int channels)
{
    // Load Image
    GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
    if (GetImage().Load(pixels, width, height, channels,
        VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    ) != vc::Error::Success)
        return vc::Error::Failure;

    // Create Image View
    if (GetImageView().Create(GetImage().GetVkImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::LoadImageRGBA(unsigned char* pixels, int width, int height, int channels)
{
    GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
    if (GetImage().Load(pixels, width, height, channels,
        VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    ) != vc::Error::Success)
        return vc::Error::Failure;

    if (GetImageView().Create(GetImage().GetVkImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::LoadImage(uint16_t* pixels, int width, int height, int channels)
{
    // Load Image
    GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
    if (GetImage().Load(pixels, width, height, channels,
        VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    ) != vc::Error::Success)
        return vc::Error::Failure;

    // Create Image View
    if (GetImageView().Create(GetImage().GetVkImage(), VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanTexture::_InitDepthBuffer(int width, int height)
{
    VkFormat depthFormat = PhysicalDevice::FindDepthFormat();
    if (GetImage().Create(depthFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, width, height) != vc::Error::Success)
        return vc::Error::Failure;
    if (GetImageView().Create(GetImage().GetVkImage(), depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, 1) != vc::Error::Success)
        return vc::Error::Failure;
    GetImage().SetImageLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    return vc::Error::Success;
}

vc::Error VulkanTexture::_CreateAttachment(int width, int height, int imageCount, vc::ShaderVertexFormat format)
{
    VkFormat vkFormat;
    switch (format) {
        case vc::ShaderVertexFormat::Float:
            vkFormat = vc::GraphicsSettings::IsHDREnabled() ? VK_FORMAT_R16_SFLOAT : VK_FORMAT_R8_SRGB;
            break;
        case vc::ShaderVertexFormat::Vec2:
            vkFormat = vc::GraphicsSettings::IsHDREnabled() ? VK_FORMAT_R16G16_SFLOAT : VK_FORMAT_R8G8_SRGB;
            break;
        case vc::ShaderVertexFormat::Vec3:
            vkFormat = vc::GraphicsSettings::IsHDREnabled() ? VK_FORMAT_R16G16B16_SFLOAT : VK_FORMAT_B8G8R8_SRGB;
            break;
        case vc::ShaderVertexFormat::Vec4:
            vkFormat = vc::GraphicsSettings::IsHDREnabled() ? VK_FORMAT_R16G16B16A16_SFLOAT : VK_FORMAT_B8G8R8A8_SRGB;
            break;
        default:
            vc::Log::Error("Unsupported format for attachment");
            return vc::Error::Failure;
    };

    if (GetImage().Create(vkFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, width, height, imageCount) != vc::Error::Success)
        return vc::Error::Failure;
    if (GetImageView().Create(GetImage().GetVkImage(), vkFormat, VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_2D, 0, 1, 0, imageCount) != vc::Error::Success)
        return vc::Error::Failure;
    GetImage().SetImageLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    return vc::Error::Success;
}
}
}
