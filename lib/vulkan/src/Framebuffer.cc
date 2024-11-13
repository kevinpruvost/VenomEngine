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

void Framebuffer::SetAttachments(const std::vector<VkImageView_T*>& arr)
{
    __framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(arr.size());
    __framebufferCreateInfo.pAttachments = arr.data();
}
}
}