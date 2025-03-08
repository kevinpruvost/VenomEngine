///
/// Project: VenomEngineWorkspace
/// @file Framebuffer.h
/// @date Nov, 13 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/vulkan/Debug.h>

#include "ImageView.h"
#include "plugin/graphics/Texture.h"
#include "venom/common/Containers.h"

namespace venom
{
namespace vulkan
{
class VulkanRenderPass;

class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();
    Framebuffer(const Framebuffer &) = delete;
    Framebuffer & operator=(const Framebuffer &) = delete;
    Framebuffer(Framebuffer && other);
    Framebuffer & operator=(Framebuffer && other);

    void Destroy();

    vc::Error Init();
    void SetRenderPass(const VulkanRenderPass * renderPass);
    const vc::Vector<VkImageView>& GetAttachmentVkImageViews() const { return __attachments; }
    const vc::Vector<const Image *>& GetAttachmentImages() const { return __images; }
    const vc::Vector<const ImageView *>& GetAttachmentImageViews() const { return __imageViews  ; }
    void SetAttachment(int i, const Image & image, const ImageView & imageView);
    void SetAttachment(int i, const VulkanTexture * texture);
    inline void SetExtent(const VkExtent2D & extent) { __framebufferCreateInfo.width = extent.width; __framebufferCreateInfo.height = extent.height; }
    inline void SetLayers(const uint32_t layers) { __framebufferCreateInfo.layers = layers; }
    inline VkFramebuffer GetVkFramebuffer() const { return __framebuffer; }
    inline const VkFramebufferCreateInfo * GetVkFramebufferCreateInfo() const { return &__framebufferCreateInfo; }
    inline VkExtent2D GetFramebufferExtent() const { return {__framebufferCreateInfo.width, __framebufferCreateInfo.height}; }

private:
    VkFramebuffer __framebuffer;
    VkFramebufferCreateInfo __framebufferCreateInfo;
    vc::Vector<VkImageView> __attachments;
    vc::Vector<const Image *> __images;
    vc::Vector<const ImageView *> __imageViews;
};
}
}
