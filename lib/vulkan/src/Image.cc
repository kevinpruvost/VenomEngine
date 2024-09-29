///
/// Project: VenomEngine
/// @file Image.cc
/// @date Sep, 24 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Image.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/QueueManager.h>
#include <venom/vulkan/Buffer.h>
#include <venom/vulkan/CommandPoolManager.h>

namespace venom
{
namespace vulkan
{
Image::Image()
    : __image(VK_NULL_HANDLE)
    , __imageMemory(VK_NULL_HANDLE)
    , __width(0), __height(0)
{
}

Image::~Image()
{
    if (__image != VK_NULL_HANDLE)
        vkDestroyImage(LogicalDevice::GetVkDevice(), __image, Allocator::GetVKAllocationCallbacks());
    if (__imageMemory != VK_NULL_HANDLE)
        vkFreeMemory(LogicalDevice::GetVkDevice(), __imageMemory, Allocator::GetVKAllocationCallbacks());
}

Image::Image(Image&& image) noexcept
    : __image(image.__image)
    , __imageMemory(image.__imageMemory)
{
    image.__image = VK_NULL_HANDLE;
    image.__imageMemory = VK_NULL_HANDLE;
}

Image& Image::operator=(Image&& image) noexcept
{
    if (this != &image) {
        __image = image.__image;
        __imageMemory = image.__imageMemory;
        image.__image = VK_NULL_HANDLE;
        image.__imageMemory = VK_NULL_HANDLE;
    }
    return *this;
}

vc::Error Image::Load(unsigned char* pixels, int width, int height, int channels,
    VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
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
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    // VK_IMAGE_USAGE_TRANSFER_DST_BIT: Image will be used as a destination for a transfer operation
    // VK_IMAGE_USAGE_SAMPLED_BIT: Image will be used to create a VkImageView suitable for occupying a texture unit
    // and be usable as a sampled image in a shader
    imageInfo.usage = usage;
    imageInfo.sharingMode = QueueManager::GetGraphicsComputeTransferSharingMode();
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    if (VkResult vkErr = vkCreateImage(LogicalDevice::GetVkDevice(), &imageInfo, Allocator::GetVKAllocationCallbacks(), &__image); vkErr != VK_SUCCESS) {
        vc::Log::Error("Failed to create image: %d", vkErr);
        return vc::Error::Failure;
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(LogicalDevice::GetVkDevice(), __image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Buffer::FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (VkResult vkErr = vkAllocateMemory(LogicalDevice::GetVkDevice(), &allocInfo, Allocator::GetVKAllocationCallbacks(), &__imageMemory); vkErr != VK_SUCCESS) {
        vc::Log::Error("Failed to allocate image memory: %d", vkErr);
        return vc::Error::Failure;
    }

    vkBindImageMemory(LogicalDevice::GetVkDevice(), __image, __imageMemory, 0);
    __width  = static_cast<uint32_t>(width);
    __height = static_cast<uint32_t>(height);

    SingleTimeCommandBuffer commandBuffer;
    if (err = CommandPoolManager::GetGraphicsCommandPool()->CreateSingleTimeCommandBuffer(commandBuffer); err != vc::Error::Success)
        return err;
    commandBuffer.TransitionImageLayout(*this, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    commandBuffer.CopyBufferToImage(stagingBuffer, *this);
    return vc::Error::Success;
}

VkImage Image::GetVkImage() const
{
    return __image;
}

Image::operator VkImage() const
{
    return __image;
}

uint32_t Image::GetWidth() const
{
    return __width;
}

uint32_t Image::GetHeight() const
{
    return __height;
}
}
}