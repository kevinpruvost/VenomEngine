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

namespace venom
{
namespace vulkan
{
VulkanTexture::VulkanTexture()
{
}

VulkanTexture::~VulkanTexture()
{
}

vc::Error VulkanTexture::__LoadImage(unsigned char* pixels, int width, int height, int channels)
{
    if (__image.Load(pixels, width, height, channels,
        VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    ) != vc::Error::Success)
        return vc::Error::Failure;

    return vc::Error::Success;
}
}
}
