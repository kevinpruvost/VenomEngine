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
    if (vkCreateFramebuffer(LogicalDevice::GetVkDevice(), &__framebufferCreateInfo, Allocator::GetVKAllocationCallbacks(), &__framebuffer) != VK_SUCCESS) {
        vc::Log::Error("Failed to create framebuffer");
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
