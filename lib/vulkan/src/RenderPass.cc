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
#include <venom/vulkan/DescriptorPool.h>
#include <venom/vulkan/SwapChain.h>

#include "venom/common/plugin/graphics/Light.h"

namespace venom::vulkan
{
static RenderPass* s_mainRenderPass = nullptr;
static vc::Vector<RenderPass *> s_renderPasses(static_cast<int>(vc::RenderingPipelineType::Count), nullptr);

static AttachmentsManager* s_attachmentsManager = nullptr;

AttachmentsManager::AttachmentsManager()
{
    venom_assert(s_attachmentsManager == nullptr, "AttachmentsManager::AttachmentsManager() : AttachmentsManager already exists");
    s_attachmentsManager = this;
}

AttachmentsManager::~AttachmentsManager()
{
}

AttachmentsManager* AttachmentsManager::Get()
{
    return s_attachmentsManager;
}

RenderPass::RenderPass()
    : __renderPass(VK_NULL_HANDLE)
    , __type(vc::RenderingPipelineType::None)
    , __swapChain(nullptr)
{
    if (s_mainRenderPass == nullptr) s_mainRenderPass = this;
}

RenderPass::~RenderPass()
{
    Destroy();
}

RenderPass::RenderPass(RenderPass&& other)
    : __renderPass(std::move(other.__renderPass))
    , __type(std::move(other.__type))
    , __swapChain(std::move(other.__swapChain))
{
}

RenderPass& RenderPass::operator=(RenderPass&& other)
{
    if (this != &other) {
        __renderPass = std::move(other.__renderPass);
        __type = std::move(other.__type);
        __swapChain = std::move(other.__swapChain);
    }
    return *this;
}

void RenderPass::Destroy()
{
    __attachments.clear();
    __framebuffers.clear();
    __attachmentRefs.clear();
    __resolveAttachmentRefs.clear();
    __attachmentDescriptions.clear();
    __resolveAttachmentDescriptions.clear();
    __subpassDescriptions.clear();
    __clearValues.clear();
    if (__renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(LogicalDevice::GetVkDevice(), __renderPass, Allocator::GetVKAllocationCallbacks());
        __renderPass = VK_NULL_HANDLE;
    }
}

void RenderPass::SetRenderingType(const vc::RenderingPipelineType type)
{
    __type = type;
    switch (__type) {
        case vc::RenderingPipelineType::Skybox: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::Skybox)] = this;
            break;
        }
        case vc::RenderingPipelineType::PBRModel: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::PBRModel)] = this;
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::Reflection)] = this;
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::AdditiveLighting)] = this;
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::AdditiveLightingMS)] = this;
            break;
        }
        case vc::RenderingPipelineType::GUI: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::GUI)] = this;
            break;
        }
        case vc::RenderingPipelineType::CascadedShadowMapping: {
            s_renderPasses[static_cast<int>(vc::RenderingPipelineType::CascadedShadowMapping)] = this;
            break;
        }
        case vc::RenderingPipelineType::Text3D: {
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
    Destroy();
    __swapChain = swapChain;
    switch (__type) {
        case vc::RenderingPipelineType::Skybox: {
            err = __CreateNormalRenderPass();
            break;
        }
        case vc::RenderingPipelineType::PBRModel: {
            err = __CreateDeferredShadowRenderPass();
            break;
        }
        case vc::RenderingPipelineType::GUI:
            err = __CreateGuiRenderPass();
            break;
        case vc::RenderingPipelineType::CascadedShadowMapping:
            err = __CreateCSMRenderPass();
            break;
        case vc::RenderingPipelineType::Text3D: {
            vc::Log::Error("Text3D are not supported yet");
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

vc::Error RenderPass::BeginRenderPass(CommandBuffer* commandBuffer, int framebufferIndex)
{
    return BeginRenderPassCustomFramebuffer(commandBuffer, &__framebuffers[framebufferIndex]);
}

vc::Error RenderPass::BeginRenderPassCustomFramebuffer(CommandBuffer* commandBuffer, const Framebuffer * const framebuffer)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = __renderPass;
    renderPassInfo.framebuffer = framebuffer->GetVkFramebuffer();
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = framebuffer->GetFramebufferExtent();

    renderPassInfo.clearValueCount = __clearValues.size();
    renderPassInfo.pClearValues = __clearValues.data();

    vkCmdBeginRenderPass(commandBuffer->_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    return vc::Error::Success;
}

void RenderPass::NextSubpass(CommandBuffer* commandBuffer)
{
    vkCmdNextSubpass(commandBuffer->_commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
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

Framebuffer* RenderPass::GetFramebuffer(const int index)
{
    return &__framebuffers[index];
}

Framebuffer* RenderPass::GetCurrentFramebuffer()
{
    return &__framebuffers[vc::GraphicsApplication::GetCurrentFrameInFlight()];
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

vc::Error RenderPass::__CreateNormalRenderPass()
{
    const bool multisampled = __swapChain->GetSamples() != VK_SAMPLE_COUNT_1_BIT;

    // Render Pass
    VkAttachmentDescription colorAttachment{};
    // Should match the format of the swap chain
    colorAttachment.format = __swapChain->activeSurfaceFormat.format;
    colorAttachment.samples = static_cast<VkSampleCountFlagBits>(__swapChain->GetSamples());
    // What to do with the data before rendering
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // OP_LOAD might be useful for deferred shading or temporal AA
    // What to do with the data after rendering
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // OP_STORE might be useful for post-processing
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Layout of the image before and after the render pass
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Depth attachment
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = VK_FORMAT_D32_SFLOAT;
    depthAttachment.samples = static_cast<VkSampleCountFlagBits>(__swapChain->GetSamples());
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Color attachment reference
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Subpass
    VkSubpassDescription & subpass = __subpassDescriptions.emplace_back();
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef; // Depth and stencil
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

    // Dependencies
    vc::Vector<VkSubpassDependency> dependencies = {dependency};

    // Create Info
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = __subpassDescriptions.size();
    renderPassInfo.pSubpasses = __subpassDescriptions.data();
    renderPassInfo.pDependencies = dependencies.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());

    if (vkCreateRenderPass(LogicalDevice::GetVkDevice(), &renderPassInfo, Allocator::GetVKAllocationCallbacks(), &__renderPass) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create render pass");
        return vc::Error::Failure;
    }

    // Clear Values
    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});
    __clearValues.emplace_back(VkClearValue{.depthStencil ={1.0f, 0}});
    if (multisampled)
        __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});

    // Framebuffers
    const size_t framebufferCount = __swapChain->swapChainImageHandles.size();
    __attachments.resize(framebufferCount);
    __framebuffers.resize(framebufferCount);
    for (int i = 0; i < framebufferCount; ++i) {
        //vc::Vector<VkImageView> attachments(2, VK_NULL_HANDLE);

        // Create Depth Image
        __attachments[i].resize(attachments.size());
        vc::Texture & depthTexture = __attachments[i][0];
        depthTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(__swapChain->GetSamples()));
        depthTexture.InitDepthBuffer(__swapChain->extent.width, __swapChain->extent.height);

        // Create MultiSampled Image if needed
        if (multisampled) {
            __framebuffers[i].SetAttachment(0, AttachmentsManager::Get()->attachments[i][static_cast<size_t>(vc::ColorAttachmentType::Present)].GetImpl()->As<VulkanTexture>());
            //attachments[0] = AttachmentsManager::Get()->attachments[i][static_cast<size_t>(vc::ColorAttachmentType::Present)].GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();
        } else {
            __framebuffers[i].SetAttachment(0, __swapChain->__swapChainImages[i], __swapChain->__swapChainImageViews[i]);
            //attachments[0] = __swapChain->__swapChainImageViews[i].GetVkImageView();
        }
        __framebuffers[i].SetAttachment(1, depthTexture.GetImpl()->As<VulkanTexture>());
        //attachments[1] = depthTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create Framebuffer
        __framebuffers[i].SetExtent(__swapChain->extent);
        __framebuffers[i].SetLayers(1);
        __framebuffers[i].SetRenderPass(this);
        if (__framebuffers[i].Init() != vc::Error::Success) {
            vc::Log::Error("Failed to create framebuffer");
            return vc::Error::Failure;
        }
    }
    return vc::Error::Success;
}

vc::Error RenderPass::__CreateCSMRenderPass()
{
    const bool multisampled = __swapChain->GetSamples() != VK_SAMPLE_COUNT_1_BIT;

    // Depth attachment
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = VK_FORMAT_D16_UNORM;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 0;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Subpass
    VkSubpassDescription & subpass = __subpassDescriptions.emplace_back();
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 0;
    subpass.pColorAttachments = nullptr;
    subpass.pDepthStencilAttachment = &depthAttachmentRef; // Depth and stencil
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr; // Input attachments

    // Subpass dependencies
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    // Create Info
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &depthAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.pDependencies = &dependency;
    renderPassInfo.dependencyCount = 1;

    if (vkCreateRenderPass(LogicalDevice::GetVkDevice(), &renderPassInfo, Allocator::GetVKAllocationCallbacks(), &__renderPass) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create render pass");
        return vc::Error::Failure;
    }

    // Clear Values
    __clearValues.emplace_back(VkClearValue{.depthStencil ={1.0f, 0}});

    // Refresh framebuffers tied to shadow maps
    const auto & lights = vc::Light::GetLights();
    for (const auto & light : lights) {
        if (vc::Error err = light->GetImpl()->As<vc::LightImpl>()->Reinit(); err != vc::Error::Success) {
            vc::Log::Error("Failed to reinit light");
            return vc::Error::Failure;
        }
    }
    return vc::Error::Success;
}

vc::Error RenderPass::__CreateGuiRenderPass()
{
    const bool multisampled = __swapChain->GetSamples() != VK_SAMPLE_COUNT_1_BIT;

    // Render Pass
    //__AddAttachment(__swapChain, __swapChain->activeSurfaceFormat.format, multisampled ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_ATTACHMENT_LOAD_OP_LOAD, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    VkAttachmentDescription colorAttachment{};
    // Should match the format of the swap chain
    colorAttachment.format = __swapChain->activeSurfaceFormat.format;
    colorAttachment.samples = static_cast<VkSampleCountFlagBits>(__swapChain->GetSamples());
    // What to do with the data before rendering
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // OP_LOAD might be useful for deferred shading or temporal AA
    // What to do with the data after rendering
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // OP_STORE might be useful for post-processing
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Layout of the image before and after the render pass
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.finalLayout = multisampled ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Depth attachment
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = VK_FORMAT_D32_SFLOAT;
    depthAttachment.samples = static_cast<VkSampleCountFlagBits>(__swapChain->GetSamples());
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Color attachment resolve
    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = __swapChain->activeSurfaceFormat.format;
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
    VkSubpassDescription & subpass = __subpassDescriptions.emplace_back();
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
    renderPassInfo.subpassCount = __subpassDescriptions.size();
    renderPassInfo.pSubpasses = __subpassDescriptions.data();
    renderPassInfo.pDependencies = dependencies.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());

    if (vkCreateRenderPass(LogicalDevice::GetVkDevice(), &renderPassInfo, Allocator::GetVKAllocationCallbacks(), &__renderPass) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create render pass");
        return vc::Error::Failure;
    }

    // Clear Values
    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});
    __clearValues.emplace_back(VkClearValue{.depthStencil ={1.0f, 0}});
    if (multisampled)
        __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});

    // Framebuffers
    const size_t framebufferCount = __swapChain->swapChainImageHandles.size();
    __attachments.resize(framebufferCount);
    __framebuffers.resize(framebufferCount);
    for (int i = 0; i < framebufferCount; ++i) {
        // Create Depth Image
        __attachments[i].resize(attachments.size() - 1);
        vc::Texture & depthTexture = __attachments[i][0];
        depthTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(__swapChain->GetSamples()));
        depthTexture.InitDepthBuffer(__swapChain->extent.width, __swapChain->extent.height);

        // Create MultiSampled Image if needed
        if (multisampled) {
            __framebuffers[i].SetAttachment(0, AttachmentsManager::Get()->attachments[i][static_cast<size_t>(vc::ColorAttachmentType::Present)].GetImpl()->As<VulkanTexture>());
            __framebuffers[i].SetAttachment(2, __swapChain->__swapChainImages[i], __swapChain->__swapChainImageViews[i]);
        } else {
            __framebuffers[i].SetAttachment(0, __swapChain->__swapChainImages[i], __swapChain->__swapChainImageViews[i]);
        }

        __framebuffers[i].SetAttachment(1, depthTexture.GetImpl()->As<VulkanTexture>());

        // Create Framebuffer
        __framebuffers[i].SetExtent(__swapChain->extent);
        __framebuffers[i].SetLayers(1);
        __framebuffers[i].SetRenderPass(this);
        if (__framebuffers[i].Init() != vc::Error::Success) {
            vc::Log::Error("Failed to create framebuffer");
            return vc::Error::Failure;
        }
    }
    return vc::Error::Success;
}

vc::Error RenderPass::__CreateDeferredShadowRenderPass()
{
    const bool multisampled = __swapChain->GetSamples() != VK_SAMPLE_COUNT_1_BIT;

    // Final Attachment : 0
    __AddAttachment(__swapChain->activeSurfaceFormat.format, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_LOAD, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, true);
    // Lighting Addition Attachment Write : 1
    __AddAttachment(__swapChain->activeSurfaceFormat.format, VK_IMAGE_LAYOUT_GENERAL, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED, true);
    // Lighting Addition Attachment Read : 2
    __AddAttachment(__swapChain->activeSurfaceFormat.format, VK_IMAGE_LAYOUT_GENERAL, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, true);
    // Depth Attachment : 3
    __AddAttachment(VK_FORMAT_D32_SFLOAT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED, false);

    if (multisampled)
        __resolveAttachmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    __SolveAttachmentReferences();

    // 2 Subpasses for Forward+ Lighting
    vc::Vector<VkAttachmentReference> gBufferAttachmentDescriptions;
    gBufferAttachmentDescriptions.emplace_back(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    gBufferAttachmentDescriptions.emplace_back(1, VK_IMAGE_LAYOUT_GENERAL);
    gBufferAttachmentDescriptions.emplace_back(2, VK_IMAGE_LAYOUT_GENERAL);

    VkAttachmentReference depthAttachmentRef{3, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

    VkSubpassDescription & mainSubpass = __subpassDescriptions.emplace_back();
    mainSubpass.flags = 0;
    mainSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    mainSubpass.colorAttachmentCount = 2;
    mainSubpass.pColorAttachments = &gBufferAttachmentDescriptions[0];
    mainSubpass.pDepthStencilAttachment = &depthAttachmentRef;
    mainSubpass.inputAttachmentCount = 1;
    mainSubpass.pInputAttachments = &gBufferAttachmentDescriptions[2];
    mainSubpass.preserveAttachmentCount = 0;
    mainSubpass.pPreserveAttachments = nullptr;
    mainSubpass.pResolveAttachments = nullptr;
    if (multisampled)
        mainSubpass.pResolveAttachments = __resolveAttachmentRefs.data();

    // Subpass dependencies
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    // Dependencies
    vc::Vector<VkSubpassDependency> dependencies = {dependency};

    // Create Info
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(__attachmentDescriptions.size());
    renderPassInfo.pAttachments = __attachmentDescriptions.data();
    renderPassInfo.subpassCount = __subpassDescriptions.size();
    renderPassInfo.pSubpasses = __subpassDescriptions.data();
    renderPassInfo.pDependencies = dependencies.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());

    if (vkCreateRenderPass(LogicalDevice::GetVkDevice(), &renderPassInfo, Allocator::GetVKAllocationCallbacks(), &__renderPass) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create render pass");
        return vc::Error::Failure;
    }

    // Clear Values
    // Final color
    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});
    // Lighting addition
    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});
    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});
    // Depth
    __clearValues.emplace_back(VkClearValue{.depthStencil= {1.0f, 0}});
    // Resolves
    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});
    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});
    __clearValues.emplace_back((VkClearValue){0.0f, 0.0f, 0.0f, 0.0f});

    // Framebuffers
    const size_t framebufferCount = __swapChain->swapChainImageHandles.size();
    __attachments.resize(framebufferCount);
    __framebuffers.resize(framebufferCount);
    for (int i = 0; i < framebufferCount; ++i) {
        vc::Vector<VkImageView> attachments(renderPassInfo.attachmentCount, VK_NULL_HANDLE);

        // Create Depth Image
        __attachments[i].resize(attachments.size());
        vc::Texture & depthTexture = __attachments[i][1];
        depthTexture.GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(static_cast<VkSampleCountFlagBits>(__swapChain->GetSamples()));
        depthTexture.InitDepthBuffer(__swapChain->extent.width, __swapChain->extent.height);

        attachments[1] = depthTexture.GetImpl()->As<VulkanTexture>()->GetImageView().GetVkImageView();

        // Create MultiSampled Image if needed
        if (multisampled) {
            __framebuffers[i].SetAttachment(0, AttachmentsManager::Get()->attachments[i][static_cast<size_t>(vc::ColorAttachmentType::Present)].GetImpl()->As<VulkanTexture>());
            __framebuffers[i].SetAttachment(1, AttachmentsManager::Get()->attachments[i][static_cast<size_t>(vc::ColorAttachmentType::LightingAddition)].GetImpl()->As<VulkanTexture>());
            __framebuffers[i].SetAttachment(2, AttachmentsManager::Get()->attachments[i][static_cast<size_t>(vc::ColorAttachmentType::LightingAddition)].GetImpl()->As<VulkanTexture>());
            __framebuffers[i].SetAttachment(4, __swapChain->__swapChainImages[i], __swapChain->__swapChainImageViews[i]);
            __framebuffers[i].SetAttachment(5, AttachmentsManager::Get()->resolveAttachments[i][static_cast<size_t>(vc::ColorAttachmentType::LightingAddition)].GetImpl()->As<VulkanTexture>());
            __framebuffers[i].SetAttachment(6, AttachmentsManager::Get()->resolveAttachments[i][static_cast<size_t>(vc::ColorAttachmentType::LightingAddition)].GetImpl()->As<VulkanTexture>());
        } else {
            __framebuffers[i].SetAttachment(0, __swapChain->__swapChainImages[i], __swapChain->__swapChainImageViews[i]);
            __framebuffers[i].SetAttachment(1, AttachmentsManager::Get()->attachments[i][static_cast<size_t>(vc::ColorAttachmentType::LightingAddition)].GetImpl()->As<VulkanTexture>());
            __framebuffers[i].SetAttachment(2, AttachmentsManager::Get()->attachments[i][static_cast<size_t>(vc::ColorAttachmentType::LightingAddition)].GetImpl()->As<VulkanTexture>());
        }
        __framebuffers[i].SetAttachment(3, depthTexture.GetImpl()->As<VulkanTexture>());


        // Create Framebuffer
        __framebuffers[i].SetExtent(__swapChain->extent);
        __framebuffers[i].SetLayers(1);
        __framebuffers[i].SetRenderPass(this);
        if (__framebuffers[i].Init() != vc::Error::Success) {
            vc::Log::Error("Failed to create framebuffer");
            return vc::Error::Failure;
        }
    }
    return vc::Error::Success;
}

void RenderPass::__AddAttachment(const VkFormat format, const VkImageLayout layout, const VkAttachmentLoadOp loadOp, const VkImageLayout initalLayout, bool resolve)
{
    const bool multisampled = __swapChain->GetSamples() != VK_SAMPLE_COUNT_1_BIT;

    VkAttachmentDescription & colorAttachment = __attachmentDescriptions.emplace_back();
    // Should match the format of the swap chain
    colorAttachment.format = format;
    colorAttachment.samples = static_cast<VkSampleCountFlagBits>(__swapChain->GetSamples());
    // What to do with the data before rendering
    colorAttachment.loadOp = loadOp; // OP_LOAD might be useful for deferred shading or temporal AA
    // What to do with the data after rendering
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // OP_STORE might be useful for post-processing
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Layout of the image before and after the render pass
    colorAttachment.initialLayout = initalLayout;
    colorAttachment.finalLayout = layout;

    VkAttachmentReference & colorAttachmentRef = __attachmentRefs.emplace_back();
    colorAttachmentRef.attachment = __attachmentRefs.size() - 1;
    colorAttachmentRef.layout = layout;

    if (multisampled && resolve) {
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
