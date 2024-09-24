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
    : __image(VK_NULL_HANDLE)
    , __imageMemory(VK_NULL_HANDLE)
{
}

VulkanTexture::~VulkanTexture()
{
}

vc::Error VulkanTexture::__LoadImage(unsigned char* pixels, int width, int height, int channels)
{
    // Buffer
    Buffer stagingBuffer;
    vc::Error err = stagingBuffer.CreateBuffer(width * height * 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        QueueManager::GetGraphicsComputeTransferSharingMode(),
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (err != vc::Error::Success)
        return err;
    stagingBuffer.WriteBuffer(pixels);

    // Image
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    // VK_IMAGE_USAGE_TRANSFER_DST_BIT: Image will be used as a destination for a transfer operation
    // VK_IMAGE_USAGE_SAMPLED_BIT: Image will be used to create a VkImageView suitable for occupying a texture unit
    // and be usable as a sampled image in a shader
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = QueueManager::GetGraphicsComputeTransferSharingMode();
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    if (auto err = vkCreateImage(LogicalDevice::GetVkDevice(), &imageInfo, Allocator::GetVKAllocationCallbacks(), &__image); err != VK_SUCCESS) {
        vc::Log::Error("Failed to create image: %d", err);
        return vc::Error::Failure;
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(LogicalDevice::GetVkDevice(), __image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Buffer::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (auto err = vkAllocateMemory(LogicalDevice::GetVkDevice(), &allocInfo, Allocator::GetVKAllocationCallbacks(), &__imageMemory); err != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate image memory: %d", err);
        return vc::Error::Failure;
    }

    vkBindImageMemory(LogicalDevice::GetVkDevice(), __image, __imageMemory, 0);
    return vc::Error::Success;
}
}
}
