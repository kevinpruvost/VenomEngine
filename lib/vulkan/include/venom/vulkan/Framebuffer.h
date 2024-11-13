///
/// Project: VenomEngineWorkspace
/// @file Framebuffer.h
/// @date Nov, 13 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class RenderPass;

class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();

    void Destroy();

    vc::Error Init();
    void SetRenderPass(const RenderPass * renderPass);
    void SetAttachments(const std::vector<VkImageView_T*>& vector);
    inline void SetExtent(const VkExtent2D & extent) { __framebufferCreateInfo.width = extent.width; __framebufferCreateInfo.height = extent.height; }
    inline void SetLayers(const uint32_t layers) { __framebufferCreateInfo.layers = layers; }
    inline VkFramebuffer GetVkFramebuffer() const { return __framebuffer; }
    inline const VkFramebufferCreateInfo * GetVkFramebufferCreateInfo() const { return &__framebufferCreateInfo; }

private:
    VkFramebuffer __framebuffer;
    VkFramebufferCreateInfo __framebufferCreateInfo;
};
}
}