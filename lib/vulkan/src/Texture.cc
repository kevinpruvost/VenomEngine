///
/// Project: Bazel_Vulkan_Metal
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

vc::Error VulkanTexture::_LoadImage(unsigned char* pixels, int width, int height, int channels)
{
    // Load Image
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
}
}
