///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanRenderPass.cc
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanRenderPass.h"

namespace venom
{
VulkanRenderPass::VulkanRenderPass()
    : __renderPass(VK_NULL_HANDLE)
    , __logicalDevice(VK_NULL_HANDLE)
{
}

VulkanRenderPass::~VulkanRenderPass()
{
    if (__renderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(__logicalDevice, __renderPass, nullptr);
}

VulkanRenderPass::VulkanRenderPass(VulkanRenderPass&& other)
    : __renderPass(other.__renderPass)
{
    other.__renderPass = VK_NULL_HANDLE;
}

VulkanRenderPass& VulkanRenderPass::operator=(VulkanRenderPass&& other)
{
    if (this == &other) return *this;
    __renderPass = other.__renderPass;
    other.__renderPass = VK_NULL_HANDLE;
    return *this;
}

Error VulkanRenderPass::InitRenderPass(VkDevice logicalDevice, const VulkanSwapChain* swapChain)
{
    // Render Pass
    VkAttachmentDescription colorAttachment{};
    // Should match the format of the swap chain
    colorAttachment.format = swapChain->activeSurfaceFormat.format;
    colorAttachment.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
    // What to do with the data before rendering
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // OP_LOAD might be useful for deferred shading or temporal AA
    // What to do with the data after rendering
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // OP_STORE might be useful for post-processing
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Layout of the image before and after the render pass
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Color attachment reference
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Subpass
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr; // Input attachments
    subpass.pResolveAttachments = nullptr; // Multisampling
    subpass.pDepthStencilAttachment = nullptr; // Depth and stencil

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &__renderPass) != VK_SUCCESS)
    {
        Log::Error("Failed to create render pass");
        return Error::Failure;
    }
    __logicalDevice = logicalDevice;
    return Error::Success;
}

Error VulkanRenderPass::BeginRenderPass(VulkanSwapChain* swapChain,
    VulkanCommandBuffer* commandBuffer, int framebufferIndex)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = __renderPass;
    renderPassInfo.framebuffer = swapChain->__swapChainFramebuffers[framebufferIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain->extent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer->__commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    return Error::Success;
}

Error VulkanRenderPass::EndRenderPass(VulkanCommandBuffer* commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer->__commandBuffer);
    return Error::Success;
}

VkRenderPass VulkanRenderPass::GetRenderPass() const
{
    return __renderPass;
}
}
