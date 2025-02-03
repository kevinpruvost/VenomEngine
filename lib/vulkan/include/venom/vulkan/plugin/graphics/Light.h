///
/// Project: VenomEngineWorkspace
/// @file Light.h
/// @date Jan, 12 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/Light.h>
#include <venom/vulkan/Framebuffer.h>
#include <venom/vulkan/RenderPass.h>
#include <venom/common/DeferredTrash.h>
#include <venom/vulkan/DescriptorPool.h>

namespace venom
{
namespace vulkan
{
class VulkanLight : public vc::LightImpl
{
public:
    VulkanLight();
    virtual ~VulkanLight();

    inline const vc::Vector<Framebuffer> & GetShadowMapFramebuffers(const int frameIndex, const int cascadeIndex) const { return (*__shadowMapFramebuffers)[frameIndex][cascadeIndex]; }
    const DescriptorSet & GetShadowMapDescriptorSet();

protected:
    vc::Error _SetType(const common::LightType type) override;
    void _SetDescriptorsFromCascade(const int cascadeIndex) override;

private:
    vc::Error __CreateFramebuffer(Framebuffer & framebuffer, RenderPass & csmRenderPass, ImageView & imageView, VkExtent2D & extent);
private:
    vc::DeferredTrash<vc::Array2D<vc::Vector<Framebuffer>, VENOM_CSM_TOTAL_CASCADES, VENOM_MAX_FRAMES_IN_FLIGHT>> __shadowMapFramebuffers;
    DescriptorSetGroup * __shadowMapDescriptorSet;
    vc::Array<int, VENOM_MAX_FRAMES_IN_FLIGHT> __lastCascades;
};
}
}
