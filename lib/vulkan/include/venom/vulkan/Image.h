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
class SwapChain;
class Image
{
public:
    Image();
    ~Image();
    Image(const Image& image) = delete;
    Image& operator=(const Image& image) = delete;
    Image(Image&& image) noexcept;
    Image& operator=(Image&& image) noexcept;

    void CreateFromSwapChainImage(VkImage img, const VkSwapchainCreateInfoKHR & swapChainInfo);
    vc::Error Load(unsigned char* pixels, int width, int height, int channels,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    vc::Error Load(uint16_t * pixels, int width, int height, int channels,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    vc::Error Create(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t width, uint32_t height, uint32_t arrayLevels = 1, uint32_t mipLevels = 1);
    void SetSamples(VkSampleCountFlagBits samples);
    void SetSamples(int samples);
    void SetImageLayout(VkImageLayout layout);
    void SetImageLayout(VkImageLayout layout, CommandBuffer & commandBuffer);
    void SetAspectMask(VkImageAspectFlags aspectMask);

    VkImageAspectFlags GetAspectMask() const { return __aspectMask; }

    inline VkFormat GetFormat() const { return __imageInfo.format; }
    inline VkImage GetVkImage() const { return __image; }

    inline uint32_t GetWidth() const { return __width; }
    inline uint32_t GetHeight() const { return __height; }
    inline VkImageLayout GetLayout() const { return __layout; }
    inline uint32_t GetMipLevels() const { return __mipLevels; }
    inline uint32_t GetArrayLayers() const { return __imageInfo.arrayLayers; }

    friend class CommandBuffer;
private:
    VkImageCreateInfo __imageInfo;
    VkImage __image;
    VkImageLayout __layout;
    VkImageAspectFlags __aspectMask;
    VkDeviceMemory __imageMemory;
    uint32_t __width, __height, __mipLevels;
    bool __noDestroy;

    friend class SwapChain;
};
}
}
