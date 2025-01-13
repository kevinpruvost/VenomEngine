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
    vc::Error BeginRenderPass(CommandBuffer * commandBuffer, int framebufferIndex);
    vc::Error BeginRenderPassCustomFramebuffer(CommandBuffer * commandBuffer, Framebuffer * framebuffer);
    void NextSubpass(CommandBuffer * commandBuffer);
    vc::Error EndRenderPass(CommandBuffer * commandBuffer);
    VkRenderPass GetVkRenderPass() const;

    Framebuffer * GetFramebuffer(const int index);
    Framebuffer * GetCurrentFramebuffer();

    inline const vc::Vector<VkSubpassDescription> & GetSubpassDescriptions() const { return __subpassDescriptions; }
    inline vc::Vector<vc::Vector<vc::Texture>> & GetAttachments() { return __attachments; }

    static RenderPass * GetRenderPass(const vc::RenderingPipelineType type);
    static vc::Vector<RenderPass *> GetRenderPasses();

private:
    vc::Error __CreateNormalRenderPass();
    vc::Error __CreateCSMRenderPass();
    vc::Error __CreateGuiRenderPass();
    vc::Error __CreateDeferredShadowRenderPass();

    void __AddAttachment(const VkFormat format, const VkImageLayout layout, const VkAttachmentLoadOp loadOp,  const VkImageLayout initalLayout, bool resolve);
    void __SolveAttachmentReferences();

private:
    const SwapChain * __swapChain;
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