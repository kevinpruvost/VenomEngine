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
    , __layout(VK_IMAGE_LAYOUT_UNDEFINED)
{
    // Image
    __imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    __imageInfo.imageType = VK_IMAGE_TYPE_2D;
    __imageInfo.pNext = nullptr;
    __imageInfo.extent.depth = 1;
    __imageInfo.mipLevels = 1;
    __imageInfo.arrayLayers = 1;
    __imageInfo.initialLayout = __layout = VK_IMAGE_LAYOUT_UNDEFINED;
    __imageInfo.flags = 0;
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
    , __width(image.__width), __height(image.__height)
    , __layout(image.__layout)
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
        __width = image.__width;
        __height = image.__height;
        __layout = image.__layout;
    }
    return *this;
}

vc::Error Image::Load(unsigned char* pixels, int width, int height, int channels,
    VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
{
    // Buffer
    Buffer stagingBuffer;
    vc::Error err = stagingBuffer.CreateBuffer(width * height * 4 * sizeof(unsigned char), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        QueueManager::GetGraphicsComputeTransferSharingMode(),
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (err != vc::Error::Success)
        return err;
    stagingBuffer.WriteBuffer(pixels);

    if (err = Create(format, tiling, usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT, properties, width, height); err != vc::Error::Success)
        return err;

    SingleTimeCommandBuffer commandBuffer;
    if (err = CommandPoolManager::GetGraphicsCommandPool()->CreateSingleTimeCommandBuffer(commandBuffer); err != vc::Error::Success)
        return err;
    commandBuffer.TransitionImageLayout(*this, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    commandBuffer.CopyBufferToImage(stagingBuffer, *this);
    commandBuffer.TransitionImageLayout(*this, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    __layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    return vc::Error::Success;
}

vc::Error Image::Load(uint16_t* pixels, int width, int height, int channels, VkFormat format, VkImageTiling tiling,
    VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
{
    // Buffer
    Buffer stagingBuffer;
    vc::Error err = stagingBuffer.CreateBuffer(width * height * 4 * sizeof(uint16_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        QueueManager::GetGraphicsComputeTransferSharingMode(),
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (err != vc::Error::Success)
        return err;
    stagingBuffer.WriteBuffer(pixels);

    if (err = Create(format, tiling, usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT, properties, width, height); err != vc::Error::Success)
        return err;

    SingleTimeCommandBuffer commandBuffer;
    if (err = CommandPoolManager::GetGraphicsCommandPool()->CreateSingleTimeCommandBuffer(commandBuffer); err != vc::Error::Success)
        return err;
    commandBuffer.TransitionImageLayout(*this, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    commandBuffer.CopyBufferToImage(stagingBuffer, *this);
    commandBuffer.TransitionImageLayout(*this, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    __layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    return vc::Error::Success;
}

vc::Error Image::Create(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                        VkMemoryPropertyFlags properties, uint32_t width, uint32_t height, uint32_t arrayLayers, uint32_t mipLevels)
{
    // Image
    __imageInfo.extent.width = static_cast<uint32_t>(width);
    __imageInfo.extent.height = static_cast<uint32_t>(height);
    __imageInfo.format = format;
    __imageInfo.tiling = tiling;
    // VK_IMAGE_USAGE_TRANSFER_DST_BIT: Image will be used as a destination for a transfer operation
    // VK_IMAGE_USAGE_SAMPLED_BIT: Image will be used to create a VkImageView suitable for occupying a texture unit
    // and be usable as a sampled image in a shader
    __imageInfo.usage = usage;
    __imageInfo.sharingMode = QueueManager::GetGraphicsComputeTransferSharingMode();
    __imageInfo.mipLevels = mipLevels;
    __imageInfo.arrayLayers = arrayLayers;

    if (VkResult vkErr = vkCreateImage(LogicalDevice::GetVkDevice(), &__imageInfo, Allocator::GetVKAllocationCallbacks(), &__image); vkErr != VK_SUCCESS) {
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
    return vc::Error::Success;
}

void Image::SetSamples(VkSampleCountFlagBits samples)
{
    __imageInfo.samples = samples;
}

void Image::SetImageLayout(VkImageLayout layout)
{
    if (__image == VK_NULL_HANDLE) {
        __imageInfo.initialLayout = __layout = layout;
    } else {
        SingleTimeCommandBuffer commandBuffer;
        if (CommandPoolManager::GetGraphicsCommandPool()->CreateSingleTimeCommandBuffer(commandBuffer) != vc::Error::Success)
            return;
        commandBuffer.TransitionImageLayout(*this, __imageInfo.format, __layout, layout);
        __layout = layout;
    }
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

VkImageLayout Image::GetLayout() const
{
    return __layout;
}
}
}