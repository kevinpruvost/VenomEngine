///
/// Project: VenomEngineWorkspace
/// @file Framebuffer.cc
/// @date Nov, 13 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Framebuffer.h>

#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/RenderPass.h>
#include <venom/vulkan/Instance.h>
#include <venom/vulkan/LogicalDevice.h>

namespace venom
{
namespace vulkan
{
Framebuffer::Framebuffer()
    : __framebuffer(VK_NULL_HANDLE)
    , __framebufferCreateInfo{}
{
    __framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    __framebufferCreateInfo.renderPass = VK_NULL_HANDLE;
    __framebufferCreateInfo.attachmentCount = 0;
    __framebufferCreateInfo.pAttachments = nullptr;
    __framebufferCreateInfo.width = 0;
    __framebufferCreateInfo.height = 0;
    __framebufferCreateInfo.layers = 1;
}

Framebuffer::~Framebuffer()
{
    Destroy();
}

Framebuffer::Framebuffer(Framebuffer&& other)
    : __framebuffer(other.__framebuffer)
    , __framebufferCreateInfo(other.__framebufferCreateInfo)
    , __attachments(std::move(other.__attachments))
    , __images(std::move(other.__images))
    , __imageViews(std::move(other.__imageViews))
{
    other.__framebuffer = VK_NULL_HANDLE;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other)
{
    if (this != &other)
    {
        __framebuffer = other.__framebuffer;
        __framebufferCreateInfo = other.__framebufferCreateInfo;
        __attachments = std::move(other.__attachments);
        __images = std::move(other.__images);
        __imageViews = std::move(other.__imageViews);
        other.__framebuffer = VK_NULL_HANDLE;
    }
    return *this;
}

void Framebuffer::Destroy()
{
    if (__framebuffer != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(LogicalDevice::GetVkDevice(), __framebuffer, Allocator::GetVKAllocationCallbacks());
        __framebuffer = VK_NULL_HANDLE;
    }
}

vc::Error Framebuffer::Init()
{
    for (const auto & imageView : __imageViews) {
        __attachments.emplace_back(imageView->GetVkImageView());
    }

    __framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(__attachments.size());
    __framebufferCreateInfo.pAttachments = __attachments.data();
    if (VkResult res = vkCreateFramebuffer(LogicalDevice::GetVkDevice(), &__framebufferCreateInfo, Allocator::GetVKAllocationCallbacks(), &__framebuffer); res != VK_SUCCESS) {
        vc::Log::Error("Failed to create framebuffer: %d", res);
        return vc::Error::InitializationFailed;
    }
    return vc::Error::Success;
}

void Framebuffer::SetRenderPass(const RenderPass* renderPass)
{
    __framebufferCreateInfo.renderPass = renderPass->GetVkRenderPass();
}

void Framebuffer::SetAttachment(int i, const Image& image, const ImageView& imageView)
{
    if (i >= __images.size()) {
        __images.resize(i + 1);
        __imageViews.resize(i + 1);
    }
    __images[i] = &image;
    __imageViews[i] = &imageView;
}

void Framebuffer::SetAttachment(int i, const VulkanTexture* texture)
{
    SetAttachment(i, texture->GetImage(), texture->GetImageView());
}
}
}
