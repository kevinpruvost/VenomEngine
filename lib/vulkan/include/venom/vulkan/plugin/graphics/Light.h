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
#include <venom/vulkan/plugin/graphics/RenderPass.h>
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
    inline const DescriptorSet & GetShadowMapDescriptorSet() { return __shadowMapDescriptorSet->GetCurrentSet(); }

protected:
    vc::Error _SetType(const vc::LightType type) override;
    void _SetDescriptorsFromCascade(const int cascadeIndex) override;

private:
    vc::Error __CreateFramebuffer(Framebuffer & framebuffer, VulkanRenderPass * csmRenderPass, ImageView & imageView, VkExtent2D & extent);
private:
    vc::DeferredTrash<vc::Array2D<vc::Vector<Framebuffer>, VENOM_MAX_FRAMES_IN_FLIGHT, VENOM_CSM_TOTAL_CASCADES>> __shadowMapFramebuffers;
    DescriptorSetGroup * __shadowMapDescriptorSet;
    vc::Array<int, VENOM_MAX_FRAMES_IN_FLIGHT> __lastCascades;
};
}
}
