///
/// Project: VenomEngine
/// @file ImageView.h
/// @date Sep, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>
#include <venom/vulkan/Image.h>

namespace venom
{
namespace vulkan
{
class ImageView
{
public:
    ImageView();
    ~ImageView();
    ImageView(const ImageView & other) = delete;
    ImageView & operator=(const ImageView & other) = delete;
    ImageView(ImageView && other);
    ImageView & operator=(ImageView && other);

    vc::Error Create(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
        VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D,
        uint32_t baseMipLevel = 0, uint32_t levelCount = 1,
        uint32_t baseArrayLayer = 0, uint32_t layerCount = 1);

    VkImageView GetVkImageView() const;
    explicit operator VkImageView() const;

private:
    VkImageView __imageView;
};

static_assert(sizeof(ImageView) == sizeof(VkImageView), "ImageView size doesn't align with VkImageView");
}
}