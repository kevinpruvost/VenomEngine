///
/// Project: VenomEngine
/// @file Image.h
/// @date Sep, 24 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class CommandBuffer;
class Image
{
public:
    Image();
    ~Image();
    Image(const Image& image) = delete;
    Image& operator=(const Image& image) = delete;
    Image(Image&& image) noexcept;
    Image& operator=(Image&& image) noexcept;

    vc::Error Load(unsigned char* pixels, int width, int height, int channels,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    vc::Error Load(uint16_t * pixels, int width, int height, int channels,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    vc::Error Create(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t width, uint32_t height);
    void SetSamples(VkSampleCountFlagBits samples);
    void SetImageLayout(VkImageLayout layout);

    VkImage GetVkImage() const;
    explicit operator VkImage() const;

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    VkImageLayout GetLayout() const;

    friend class CommandBuffer;
private:
    VkImageCreateInfo __imageInfo;
    VkImage __image;
    VkImageLayout __layout;
    VkDeviceMemory __imageMemory;
    uint32_t __width, __height;
};
}
}
