///
/// Project: VenomEngine
/// @file VulkanRenderPass.cc
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/RenderPass.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

namespace venom::vulkan
{
RenderPass::RenderPass()
    : __renderPass(VK_NULL_HANDLE)
{
}

RenderPass::~RenderPass()
{
    if (__renderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(LogicalDevice::GetVkDevice(), __renderPass, Allocator::GetVKAllocationCallbacks());
}

RenderPass::RenderPass(RenderPass&& other)
    : __renderPass(other.__renderPass)
{
    other.__renderPass = VK_NULL_HANDLE;
}

RenderPass& RenderPass::operator=(RenderPass&& other)
{
    if (this != &other) {
        __renderPass = other.__renderPass;
        other.__renderPass = VK_NULL_HANDLE;
    }
    return *this;
}

vc::Error RenderPass::InitRenderPass(const SwapChain* swapChain)
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

    // Subpass dependencies
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.pDependencies = &dependency;
    renderPassInfo.dependencyCount = 1;

    if (vkCreateRenderPass(LogicalDevice::GetVkDevice(), &renderPassInfo, Allocator::GetVKAllocationCallbacks(), &__renderPass) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create render pass");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error RenderPass::BeginRenderPass(SwapChain* swapChain,
    CommandBuffer* commandBuffer, int framebufferIndex)
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

    vkCmdBeginRenderPass(commandBuffer->_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    return vc::Error::Success;
}

vc::Error RenderPass::EndRenderPass(CommandBuffer* commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer->_commandBuffer);
    return vc::Error::Success;
}

VkRenderPass RenderPass::GetRenderPass() const
{
    return __renderPass;
}
}
