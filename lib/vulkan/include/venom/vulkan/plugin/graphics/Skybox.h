///
/// Project: VenomEngineWorkspace
/// @file Skybox.h
/// @date Oct, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/Skybox.h>
#include <venom/vulkan/CommandPool.h>
#include <venom/vulkan/DescriptorPool.h>

namespace venom
{
namespace vulkan
{
class VulkanSkybox : public vc::SkyboxImpl
{
public:
    VulkanSkybox();
    ~VulkanSkybox() override;

    inline const VertexBuffer & GetVertexBuffer() const { return __vertexBuffer; }
    inline const DescriptorSet& GetPanormaDescriptorSet() const { return __descriptorSet->GetCurrentSet(); }

    vc::Error _LoadSkybox(const vc::Texture & texture) override;
    vc::Error _LoadIrradianceMap(const vc::Texture& texture, vc::Texture & irradianceMap) override;
    vc::Error _LoadRadianceMap(const vc::Texture& texture, vc::Texture& radianceMap) override;
    vc::Error _LoadBlurMap(const common::Texture& texture, common::Texture& blurMap) override;
    vc::Error _ChangeBlurFactor(const float factor) override;
//private:
    VertexBuffer __vertexBuffer;
    UniformBuffer __uniformBuffer;
    DescriptorSetGroup * __descriptorSet;
};
}
}
