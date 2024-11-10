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
static RenderPass* s_mainRenderPass = nullptr;
RenderPass::RenderPass()
    : __renderPass(VK_NULL_HANDLE)
{
    if (s_mainRenderPass == nullptr) s_mainRenderPass = this;
}

RenderPass::~RenderPass()
{
    Destroy();
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

void RenderPass::Destroy()
{
    if (__renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(LogicalDevice::GetVkDevice(), __renderPass, Allocator::GetVKAllocationCallbacks());
        __renderPass = VK_NULL_HANDLE;
    }
}

vc::Error RenderPass::InitRenderPass(const SwapChain* swapChain)
{
    const bool multisampled = swapChain->GetSamples() != VK_SAMPLE_COUNT_1_BIT;

    // Render Pass
    VkAttachmentDescription colorAttachment{};
    // Should match the format of the swap chain
    colorAttachment.format = swapChain->activeSurfaceFormat.format;
    colorAttachment.samples = static_cast<VkSampleCountFlagBits>(swapChain->GetSamples());
    // What to do with the data before rendering
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // OP_LOAD might be useful for deferred shading or temporal AA
    // What to do with the data after rendering
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // OP_STORE might be useful for post-processing
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Layout of the image before and after the render pass
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = multisampled ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Depth attachment
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = VK_FORMAT_D32_SFLOAT;
    depthAttachment.samples = static_cast<VkSampleCountFlagBits>(swapChain->GetSamples());
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Color attachment resolve
    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = swapChain->activeSurfaceFormat.format;
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Color attachment reference
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Color attachment resolve reference
    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Subpass
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef; // Depth and stencil
    if (multisampled)
        subpass.pResolveAttachments = &colorAttachmentResolveRef; // Multisampling
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr; // Input attachments

    // Subpass dependencies
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    // Attachments
    vc::Vector<VkAttachmentDescription> attachments = {colorAttachment, depthAttachment};
    if (multisampled)
        attachments.emplace_back(colorAttachmentResolve);

    // Dependencies
    vc::Vector<VkSubpassDependency> dependencies = {dependency};

    // Create Info
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.pDependencies = dependencies.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());

    Destroy();
    if (vkCreateRenderPass(LogicalDevice::GetVkDevice(), &renderPassInfo, Allocator::GetVKAllocationCallbacks(), &__renderPass) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create render pass");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error RenderPass::BeginRenderPass(SwapChain* swapChain, CommandBuffer* commandBuffer, int framebufferIndex)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = __renderPass;
    renderPassInfo.framebuffer = swapChain->__swapChainFramebuffers[framebufferIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain->extent;

    VkClearValue clearColor[] = {
        {{0.0f, 0.0f, 0.0f, 1.0f}}, // color
        {{1.0f, 0}} // depth
    };
    clearColor[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
    clearColor[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = std::size(clearColor);
    renderPassInfo.pClearValues = clearColor;

    vkCmdBeginRenderPass(commandBuffer->_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    return vc::Error::Success;
}

vc::Error RenderPass::EndRenderPass(CommandBuffer* commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer->_commandBuffer);
    return vc::Error::Success;
}

VkRenderPass RenderPass::GetVkRenderPass() const
{
    return __renderPass;
}

RenderPass* RenderPass::MainRenderPass()
{
    return s_mainRenderPass;
}
}
