///
/// Project: VenomEngine
/// @file ImageView.cc
/// @date Sep, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/ImageView.h>

#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

namespace venom
{
namespace vulkan
{
ImageView::ImageView()
    : __imageView(VK_NULL_HANDLE)
    , __image(nullptr)
{
}

ImageView::~ImageView()
{
    if (__imageView != VK_NULL_HANDLE)
        vkDestroyImageView(LogicalDevice::GetVkDevice(), __imageView, Allocator::GetVKAllocationCallbacks());
}

ImageView::ImageView(ImageView&& other)
    : __imageView(other.__imageView)
    , __image(other.__image)
{
    other.__imageView = VK_NULL_HANDLE;
    other.__image = nullptr;
}

ImageView& ImageView::operator=(ImageView&& other)
{
    if (this != &other) {
        __imageView = std::move(other.__imageView);
        __image = std::move(other.__image);
    }
    return *this;
}

vc::Error ImageView::Create(Image& image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType,
    uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    vc::Error error = Create(image.GetVkImage(), format, aspectFlags, viewType, baseMipLevel, levelCount, baseArrayLayer, layerCount);
    if (error != vc::Error::Success) {
        return error;
    }
    __image = &image;
    return error;
}

vc::Error ImageView::Create(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType,
                            uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    if (layerCount > 1) {
        if (viewType == VK_IMAGE_VIEW_TYPE_1D) viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        else if (viewType == VK_IMAGE_VIEW_TYPE_2D) viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        else if (viewType == VK_IMAGE_VIEW_TYPE_3D) viewType = VK_IMAGE_VIEW_TYPE_3D;
    }
    VkImageViewCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = viewType,
        .format = format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .subresourceRange = {
            .aspectMask = aspectFlags,
            .baseMipLevel = baseMipLevel,
            .levelCount = levelCount,
            .baseArrayLayer = baseArrayLayer,
            .layerCount = layerCount
        }
    };
    if (VkResult err = vkCreateImageView(LogicalDevice::GetVkDevice(), &createInfo, Allocator::GetVKAllocationCallbacks(), &__imageView); err != VK_SUCCESS) {
        vc::Log::Error("Failed to create image view: %d", err);
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}
}
}