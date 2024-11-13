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
static vc::Vector<RenderPass *> s_renderPasses(static_cast<int>(vc::RenderingPipelineType::Count), nullptr);
RenderPass::RenderPass()
    : __renderPass(VK_NULL_HANDLE)
    , __type(vc::RenderingPipelineType::None)
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

void RenderPass::SetRenderingType(const vc::RenderingPipelineType type)
{
    __type = type;
    switch (__type) {
        case vc::RenderingPipelineType::BasicModel:
        case vc::RenderingPipelineType::Skybox: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::BasicModel)] = this;
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::Skybox)] = this;
            break;
        }
        case vc::RenderingPipelineType::ShadowModel: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::ShadowModel)] = this;
            break;
        }
        case vc::RenderingPipelineType::Text2D:
        case vc::RenderingPipelineType::Text3D: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::Text2D)] = this;
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::Text3D)] = this;
            break;
        }
        default: {
            vc::Log::Error("Rendering Pipeline Type not supported");
            break;
        }
    }
}

vc::Error RenderPass::InitRenderPass(const SwapChain* swapChain)
{
    vc::Error err = vc::Error::Success;
    __attachmentRefs.clear();
    __resolveAttachmentRefs.clear();
    __attachmentDescriptions.clear();
    __resolveAttachmentDescriptions.clear();
    switch (__type) {
        case vc::RenderingPipelineType::BasicModel:
        case vc::RenderingPipelineType::Skybox: {
            err = __CreateNormalRenderPass(swapChain);
            break;
        }
        case vc::RenderingPipelineType::ShadowModel: {
            err = __CreateDeferredShadowRenderPass(swapChain);
            break;
        }
        case vc::RenderingPipelineType::Text2D:
        case vc::RenderingPipelineType::Text3D: {
            vc::Log::Error("Text2D and Text3D are not supported yet");
            return vc::Error::Failure;
        }
        default: {
            vc::Log::Error("Rendering Pipeline Type not supported");
            return vc::Error::Failure;
        }
    }

    if (err != vc::Error::Success) {
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
    renderPassInfo.framebuffer = __framebuffers[framebufferIndex].GetVkFramebuffer();
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

RenderPass* RenderPass::GetRenderPass(const vc::RenderingPipelineType type)
{
    venom_assert(type != vc::RenderingPipelineType::None && type != vc::RenderingPipelineType::Count, "RenderPass::GetRenderPass() : type is None or Count");
    return s_renderPasses[static_cast<int>(type)];
}

vc::Vector<RenderPass*> RenderPass::GetRenderPasses()
{
    return s_renderPasses;
}

vc::Error RenderPass::__CreateNormalRenderPass(const SwapChain* swapChain)
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

    // Framebuffers
    const size_t framebufferCount = swapChain->swapChainImageHandles.size();
    __attachments.clear();
    __framebuffers.clear();
    __attachments.resize(framebufferCount);
    __framebuffers.resize(framebufferCount);
    for (int i = 0; i < framebufferCount; ++i) {
        vc::Vector<VkImageView> attachments(multisampled ? 3 : 2, VK_NULL_HANDLE);

        // Create Depth Image
        __attachments[i].resize(attachments.size() - 1);
        vc::Texture & depthTexture = __attachments[i][0];
        depthTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
        depthTexture.InitDepthBuffer(swapChain->extent.width, swapChain->extent.height);

        // Create MultiSampled Image if needed
        if (multisampled) {
            vc::Texture & colorTexture = __attachments[i][1];
            VulkanTexture * vkTexture = colorTexture.GetImpl()->As<VulkanTexture>();
            vkTexture->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
            colorTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
            attachments[0] = vkTexture->GetImageView().GetVkImageView();
            attachments[2] = swapChain->__swapChainImages[i].GetVkImageView();
        } else {
            attachments[0] = swapChain->__swapChainImages[i].GetVkImageView();
        }
        attachments[1] = depthTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create Framebuffer
        __framebuffers[i].SetExtent(swapChain->extent);
        __framebuffers[i].SetAttachments(attachments);
        __framebuffers[i].SetLayers(1);
        __framebuffers[i].SetRenderPass(this);
        if (__framebuffers[i].Init() != vc::Error::Success) {
            vc::Log::Error("Failed to create framebuffer");
            return vc::Error::Failure;
        }
    }
    return vc::Error::Success;
}

vc::Error RenderPass::__CreateDeferredShadowRenderPass(const SwapChain* swapChain)
{
    const bool multisampled = swapChain->GetSamples() != VK_SAMPLE_COUNT_1_BIT;

    // Color Attachment : 0
    __AddAttachment(swapChain, swapChain->activeSurfaceFormat.format, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    // Normal Attachment : 1
    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    // Metallic Roughness AO Attachment : 2
    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    // Position Attachment : 3
    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    // Specular Color and Reflectivity Attachment : 4
    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    // Emission Attachment : 5
    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    // Clearcoat and Sheen Attachment : 6
    __AddAttachment(swapChain, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    // Depth Attachment : 7
    __AddAttachment(swapChain, VK_FORMAT_D32_SFLOAT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    // For resolve indices if using multisampling
    __SolveAttachmentReferences();

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
    // Reference
    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 8;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Subpass
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = __attachmentRefs.size() - 1; // Color attachments
    subpass.pColorAttachments = __attachmentRefs.data();
    subpass.pDepthStencilAttachment = &__attachmentRefs[7]; // Depth and stencil
    if (multisampled)
        subpass.pResolveAttachments = __resolveAttachmentRefs.data(); // Multisampling
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

    // Dependencies
    vc::Vector<VkSubpassDependency> dependencies = {dependency};

    // Create Info
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(__attachmentDescriptions.size());
    renderPassInfo.pAttachments = __attachmentDescriptions.data();
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

    // Framebuffers
    const size_t framebufferCount = swapChain->swapChainImageHandles.size();
    __attachments.clear();
    __framebuffers.clear();
    __attachments.resize(framebufferCount);
    __framebuffers.resize(framebufferCount);
    for (int i = 0; i < framebufferCount; ++i) {
        vc::Vector<VkImageView> attachments(multisampled ? 15 : 8, VK_NULL_HANDLE);

        // Create Depth Image
        __attachments[i].resize(attachments.size());
        vc::Texture & depthTexture = __attachments[i][7];
        depthTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
        depthTexture.InitDepthBuffer(swapChain->extent.width, swapChain->extent.height);

        attachments[7] = depthTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create Normal Image
        vc::Texture & normalTexture = __attachments[i][1];
        normalTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
        normalTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
        attachments[1] = normalTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create Metallic Roughness Image and AO
        vc::Texture & metallicRoughnessTexture = __attachments[i][2];
        metallicRoughnessTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
        metallicRoughnessTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
        attachments[2] = metallicRoughnessTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create Position Image
        vc::Texture & positionTexture = __attachments[i][3];
        positionTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
        positionTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
        attachments[3] = positionTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create Specular Color and Reflectivity Image
        vc::Texture & specularColorReflectivityTexture = __attachments[i][4];
        specularColorReflectivityTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
        specularColorReflectivityTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
        attachments[4] = specularColorReflectivityTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create Emission Image
        vc::Texture & emissionTexture = __attachments[i][5];
        emissionTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
        emissionTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
        attachments[5] = emissionTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create Anisotropy and Transmission Image and Clearcoat and Sheen Image
        vc::Texture & clearcoatSheenTexture = __attachments[i][6];
        clearcoatSheenTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
        clearcoatSheenTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
        attachments[6] = clearcoatSheenTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create MultiSampled Image if needed
        if (multisampled) {
            vc::Texture & resolveColorTexture = __attachments[i][8];
            VulkanTexture * vkTexture = resolveColorTexture.GetImpl()->As<VulkanTexture>();
            vkTexture->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(swapChain->GetSamples()));
            resolveColorTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
            attachments[0] = vkTexture->GetImageView().GetVkImageView();
            attachments[8] = swapChain->__swapChainImages[i].GetVkImageView();

            // Create Resolve for every other color attachment
            // Normal
            vc::Texture & resolveNormalTexture = __attachments[i][9];
            resolveNormalTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
            resolveNormalTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
            attachments[9] = resolveNormalTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
            // Metallic Roughness AO
            vc::Texture & resolveMetallicRoughnessTexture = __attachments[i][10];
            resolveMetallicRoughnessTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
            resolveMetallicRoughnessTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
            attachments[10] = resolveMetallicRoughnessTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
            // Position
            vc::Texture & resolvePositionTexture = __attachments[i][11];
            resolvePositionTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
            resolvePositionTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
            attachments[11] = resolvePositionTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
            // Specular Color and Reflectivity
            vc::Texture & resolveSpecularColorReflectivityTexture = __attachments[i][12];
            resolveSpecularColorReflectivityTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
            resolveSpecularColorReflectivityTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
            attachments[12] = resolveSpecularColorReflectivityTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
            // Emission
            vc::Texture & resolveEmissionTexture = __attachments[i][13];
            resolveEmissionTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
            resolveEmissionTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
            attachments[13] = resolveEmissionTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
            // Anisotropy and Transmission and Clearcoat and Sheen
            vc::Texture & resolveClearcoatSheenTexture = __attachments[i][14];
            resolveClearcoatSheenTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(VK_SAMPLE_COUNT_1_BIT);
            resolveClearcoatSheenTexture.CreateAttachment(swapChain->extent.width, swapChain->extent.height, vc::ShaderVertexFormat::Vec4);
            attachments[14] = resolveClearcoatSheenTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
        } else {
            attachments[0] = swapChain->__swapChainImages[i].GetVkImageView();
        }

        // Create Framebuffer
        __framebuffers[i].SetExtent(swapChain->extent);
        __framebuffers[i].SetAttachments(attachments);
        __framebuffers[i].SetLayers(1);
        __framebuffers[i].SetRenderPass(this);
        if (__framebuffers[i].Init() != vc::Error::Success) {
            vc::Log::Error("Failed to create framebuffer");
            return vc::Error::Failure;
        }
    }
    return vc::Error::Success;
}

void RenderPass::__AddAttachment(const SwapChain* swapChain, const VkFormat format, const VkImageLayout layout)
{
    const bool multisampled = swapChain->GetSamples() != VK_SAMPLE_COUNT_1_BIT;

    VkAttachmentDescription & colorAttachment = __attachmentDescriptions.emplace_back();
    // Should match the format of the swap chain
    colorAttachment.format = format;
    colorAttachment.samples = static_cast<VkSampleCountFlagBits>(swapChain->GetSamples());
    // What to do with the data before rendering
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // OP_LOAD might be useful for deferred shading or temporal AA
    // What to do with the data after rendering
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // OP_STORE might be useful for post-processing
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Layout of the image before and after the render pass
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = layout;

    VkAttachmentReference & colorAttachmentRef = __attachmentRefs.emplace_back();
    colorAttachmentRef.attachment = __attachmentRefs.size() - 1;
    colorAttachmentRef.layout = layout;

    if (multisampled) {
        VkAttachmentDescription & resAttachment = __resolveAttachmentDescriptions.emplace_back();
        resAttachment.format = format;
        resAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        resAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        resAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        resAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        resAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        resAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        resAttachment.finalLayout = layout;

        VkAttachmentReference & resAttachmentRef = __resolveAttachmentRefs.emplace_back();
        resAttachmentRef.attachment = 0;
        resAttachmentRef.layout = layout;
    }
}

void RenderPass::__SolveAttachmentReferences()
{
    __attachmentDescriptions.insert(__attachmentDescriptions.end(), __resolveAttachmentDescriptions.begin(), __resolveAttachmentDescriptions.end());
    for (int i = 0; i < __resolveAttachmentRefs.size(); ++i) {
        __resolveAttachmentRefs[i].attachment = i + __attachmentRefs.size();
    }
}
}
