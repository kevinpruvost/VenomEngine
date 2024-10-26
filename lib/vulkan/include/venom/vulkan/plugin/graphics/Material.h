///
/// Project: VenomEngine
/// @file Material.h
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Material.h>
#include <venom/vulkan/DescriptorSet.h>

namespace venom
{
namespace vulkan
{

class VulkanMaterial : public vc::MaterialImpl
{
public:
    VulkanMaterial();
    ~VulkanMaterial();

    const DescriptorSet & GetMaterialDescriptorSet();
    const DescriptorSet & GetTextureDescriptorSet();

private:
    DescriptorSetGroup * __materialDescriptorSet, * __textureDescriptorSet;
    UniformBuffer __uniformBuffer;
};

}
}