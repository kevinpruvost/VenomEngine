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
    vc::Error EndRenderPass(CommandBuffer * commandBuffer);
    VkRenderPass GetVkRenderPass() const;

    static RenderPass * GetRenderPass(const vc::RenderingPipelineType type);
    static vc::Vector<RenderPass *> GetRenderPasses();

private:
    vc::Error __CreateNormalRenderPass(const SwapChain * swapChain);
    vc::Error __CreateDeferredShadowRenderPass(const SwapChain * swapChain);

    void __AddAttachment(const SwapChain* swapChain, const VkFormat format, const VkImageLayout layout);
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
};
}
}