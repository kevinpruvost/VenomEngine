///
/// Project: VenomEngine
/// @file RenderPass.h
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/RenderingPipelineType.h>
#include <venom/vulkan/SwapChain.h>
#include <venom/vulkan/CommandPool.h>
#include <venom/vulkan/Framebuffer.h>

namespace venom
{
namespace vulkan
{
class AttachmentsManager
{
public:
    AttachmentsManager();
    ~AttachmentsManager();
    void RecreateAttachments();

    static AttachmentsManager * Get();

public:
    vc::Vector<vc::Vector<vc::Texture>> attachments;
    vc::Vector<vc::Vector<vc::Texture>> resolveAttachments;
};

class RenderPass
{
public:
    RenderPass();
    ~RenderPass();
    RenderPass(const RenderPass&) = delete;
    RenderPass& operator=(const RenderPass&) = delete;
    RenderPass(RenderPass&& other);
    RenderPass& operator=(RenderPass&& other);

    void Destroy();

    void SetRenderingType(const vc::RenderingPipelineType type);
    vc::Error InitRenderPass(const SwapChain * swapChain);
    vc::Error BeginRenderPass(SwapChain * swapChain, CommandBuffer * commandBuffer, int framebufferIndex);
    void NextSubpass(CommandBuffer * commandBuffer);
    vc::Error EndRenderPass(CommandBuffer * commandBuffer);
    VkRenderPass GetVkRenderPass() const;

    inline const vc::Vector<VkSubpassDescription> & GetSubpassDescriptions() const { return __subpassDescriptions; }
    inline vc::Vector<vc::Vector<vc::Texture>> & GetAttachments() { return __attachments; }

    static RenderPass * GetRenderPass(const vc::RenderingPipelineType type);
    static vc::Vector<RenderPass *> GetRenderPasses();

private:
    vc::Error __CreateNormalRenderPass(const SwapChain * swapChain);
    vc::Error __CreateGuiRenderPass(const SwapChain * swapChain);
    vc::Error __CreateDeferredShadowRenderPass(const SwapChain * swapChain);

    void __AddAttachment(const SwapChain* swapChain, const VkFormat format, const VkImageLayout layout, const VkAttachmentLoadOp loadOp,  const VkImageLayout initalLayout);
    void __SolveAttachmentReferences();

private:
    VkRenderPass __renderPass;
    vc::RenderingPipelineType __type;
    // By image count then attachments
    vc::Vector<vc::Vector<vc::Texture>> __attachments;
    vc::Vector<Framebuffer> __framebuffers;

    vc::Vector<VkAttachmentDescription> __attachmentDescriptions;
    vc::Vector<VkAttachmentDescription> __resolveAttachmentDescriptions;
    vc::Vector<VkAttachmentReference> __attachmentRefs;
    vc::Vector<VkAttachmentReference> __resolveAttachmentRefs;

    vc::Vector<VkSubpassDescription> __subpassDescriptions;

    vc::Vector<VkClearValue> __clearValues;
};
}
}