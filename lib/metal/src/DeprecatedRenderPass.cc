///
/// Project: VenomEngineWorkspace
/// @file DeprecatedRenderPass.cc
/// @date Nov, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/RenderPass.h>

namespace venom
{
namespace vulkan
{
//vc::Error RenderPass::__CreateDeferredShadowRenderPass(const SwapChain* swapChain)
//{
//    const bool multisampled = swapChain->GetSamples() != VK_SAMPLE_COUNT_1_BIT;
//
//    // Final Attachment : 0
//    __AddAttachment(swapChain, swapChain->activeSurfaceFormat.format, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_LOAD, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//    // Color Attachment : 1
//    __AddAttachment(swapChain, swapChain->activeSurfaceFormat.format, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED);
//    // Normal Attachment : 2
//    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED);
//    // Metallic Roughness AO Attachment : 3
//    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED);
//    // Position Attachment : 4
//    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED);
//    // Specular Attachment : 5
//    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED);
//    // Depth Attachment : 6
//    __AddAttachment(swapChain, VK_FORMAT_D32_SFLOAT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED);
//
//    // 2 Subpasses for Forward+ Lighting
//    vc::Vector<VkAttachmentReference> gBufferAttachmentDescriptions;
//    gBufferAttachmentDescriptions.emplace_back(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//    gBufferAttachmentDescriptions.emplace_back(1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//    gBufferAttachmentDescriptions.emplace_back(2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//    gBufferAttachmentDescriptions.emplace_back(3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//    gBufferAttachmentDescriptions.emplace_back(4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//    gBufferAttachmentDescriptions.emplace_back(5, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//
//    VkAttachmentReference depthAttachmentRef{6, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
//
//    VkSubpassDescription & subpassGBuffer = __subpassDescriptions.emplace_back();
//    subpassGBuffer.flags = 0;
//    subpassGBuffer.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//    subpassGBuffer.colorAttachmentCount = gBufferAttachmentDescriptions.size();
//    subpassGBuffer.pColorAttachments = gBufferAttachmentDescriptions.data();
//    subpassGBuffer.pDepthStencilAttachment = &depthAttachmentRef;
//    subpassGBuffer.inputAttachmentCount = 0;
//    subpassGBuffer.pInputAttachments = nullptr; // Input attachments
//    subpassGBuffer.preserveAttachmentCount = 0;
//    subpassGBuffer.pPreserveAttachments = nullptr;
//    subpassGBuffer.pResolveAttachments = nullptr;
//
//    vc::Vector<VkAttachmentReference> lightingInputAttachmentDescriptions;
//    lightingInputAttachmentDescriptions.emplace_back(1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//    lightingInputAttachmentDescriptions.emplace_back(2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//    lightingInputAttachmentDescriptions.emplace_back(3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//    lightingInputAttachmentDescriptions.emplace_back(4, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//    lightingInputAttachmentDescriptions.emplace_back(5, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//
//    VkSubpassDescription & subpassLighting = __subpassDescriptions.emplace_back();
//    subpassLighting.flags = 0;
//    subpassLighting.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//    subpassLighting.colorAttachmentCount = 1;
//    subpassLighting.pColorAttachments = gBufferAttachmentDescriptions.data();
//    subpassLighting.pDepthStencilAttachment = &depthAttachmentRef;
//    subpassLighting.inputAttachmentCount = static_cast<uint32_t>(lightingInputAttachmentDescriptions.size());
//    subpassLighting.pInputAttachments = lightingInputAttachmentDescriptions.data(); // Input attachments
//    subpassLighting.preserveAttachmentCount = 0;
//    subpassLighting.pPreserveAttachments = nullptr;
//    subpassLighting.pResolveAttachments = nullptr;
//
//    // Subpass dependencies
//    VkSubpassDependency dependency{};
//    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//    dependency.dstSubpass = 0;
//    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//    dependency.srcAccessMask = 0;
//    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//
//    // Subpass dependencies
//    VkSubpassDependency dependencyGBufferToLighting{};
//    dependencyGBufferToLighting.srcSubpass = 0; // GBuffer subpass
//    dependencyGBufferToLighting.dstSubpass = 1; // Lighting subpass
//    dependencyGBufferToLighting.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//    dependencyGBufferToLighting.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
//    dependencyGBufferToLighting.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//    dependencyGBufferToLighting.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
//    dependencyGBufferToLighting.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//    // Dependencies
//    vc::Vector<VkSubpassDependency> dependencies = {dependency, dependencyGBufferToLighting};
//
//    // Create Info
//    VkRenderPassCreateInfo renderPassInfo{};
//    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//    renderPassInfo.attachmentCount = static_cast<uint32_t>(__attachmentDescriptions.size());
//    renderPassInfo.pAttachments = __attachmentDescriptions.data();
//    renderPassInfo.subpassCount = __subpassDescriptions.size();
//    renderPassInfo.pSubpasses = __subpassDescriptions.data();
//    renderPassInfo.pDependencies = dependencies.data();
//    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
//
//    if (vkCreateRenderPass(LogicalDevice::GetVkDevice(), &renderPassInfo, Allocator::GetVKAllocationCallbacks(), &__renderPass) != VK_SUCCESS)
//    {
//        vc::Log::Error("Failed to create render pass");
//        return vc::Error::Failure;
//    }
//
//    // Clear Values
//    // Final color
//    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 1.0f});
//    // Base color
//    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 1.0f});
//    // Normal
//    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});
//    // Metallic Roughness AO
//    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});
//    // Position
//    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 1.0f});
//    // Specular
//    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 1.0f});
//    // Depth
//    __clearValues.emplace_back(VkClearValue{.depthStencil= {1.0f, 0}});
//
//    // Framebuffers
//    const size_t framebufferCount = swapChain->swapChainImageHandles.size();
//    __attachments.resize(framebufferCount);
//    __framebuffers.resize(framebufferCount);
//    for (int i = 0; i < framebufferCount; ++i) {
//        vc::Vector<VkImageView> attachments(__attachmentDescriptions.size(), VK_NULL_HANDLE);
//
//        // Create Depth Image
//        __attachments[i].resize(attachments.size());
//        vc::Texture & depthTexture = __attachments[i][6];
//        depthTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
//        depthTexture.InitDepthBuffer(swapChain->extent.width, swapChain->extent.height);
//
//        attachments[6] = depthTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
//
//        // Create Base Color Image
//        vc::Texture & colorTexture = __attachments[i][1];
//        colorTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
//        colorTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, 1, vc::ShaderVertexFormat::Vec4);
//        attachments[1] = colorTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
//
//        // Create Normal Image
//        vc::Texture & normalTexture = __attachments[i][2];
//        normalTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
//        normalTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, 1, vc::ShaderVertexFormat::Vec4);
//        attachments[2] = normalTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
//
//        // Create Metallic Roughness Image and AO
//        vc::Texture & metallicRoughnessTexture = __attachments[i][3];
//        metallicRoughnessTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
//        metallicRoughnessTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, 1, vc::ShaderVertexFormat::Vec4);
//        attachments[3] = metallicRoughnessTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
//
//        // Create Position Image
//        vc::Texture & positionTexture = __attachments[i][4];
//        positionTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
//        positionTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, 1, vc::ShaderVertexFormat::Vec4);
//        attachments[4] = positionTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
//
//        // Create Specular Image
//        vc::Texture & specularTexture = __attachments[i][5];
//        specularTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
//        specularTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, 1, vc::ShaderVertexFormat::Vec4);
//        attachments[5] = specularTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
//
//        // Create MultiSampled Image if needed
//        if (multisampled) {
//            attachments[0] = AttachmentsManager::Get()->attachments[i][vc::ColorAttachmentType::Present].GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
//        } else {
//            attachments[0] = swapChain->__swapChainImages[i].GetVkImageView();
//        }
//
//        // Create Framebuffer
//        __framebuffers[i].SetExtent(swapChain->extent);
//        __framebuffers[i].SetAttachments(attachments);
//        __framebuffers[i].SetLayers(1);
//        __framebuffers[i].SetRenderPass(this);
//        if (__framebuffers[i].Init() != vc::Error::Success) {
//            vc::Log::Error("Failed to create framebuffer");
//            return vc::Error::Failure;
//        }
//    }
//
//    // Now linking the input attachments to the descriptor sets
//    for (int i = 0; i < framebufferCount; ++i) {
//        for (int x = 1; x <= 5; ++x) {
//            DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT)
//                .GroupUpdateImageViewPerFrame(i, __attachments[i][x].GetImpl()->As<VulkanTexture>()->GetImageView(), x+2, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1);
//        }
//    }
//    return vc::Error::Success;
//}
}
}