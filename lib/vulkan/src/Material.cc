///
/// Project: VenomEngine
/// @file Material.cc
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/DescriptorPool.h>
#include <venom/vulkan/plugin/graphics/Material.h>

namespace venom
{
namespace vulkan
{
VulkanMaterial::VulkanMaterial()
    : __materialDescriptorSet(DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_MATERIAL).AllocateSet())
    , __textureDescriptorSet(
        vc::ShaderResourceTable::UsingLargeBindlessTextures() ?
        nullptr
        :
        DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_TEXTURES).AllocateSet()
    )
{
    __uniformBuffer.Init(sizeof(MaterialResourceTable));
    __materialDescriptorSet->GroupUpdateBuffer(__uniformBuffer, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1);
}

VulkanMaterial::~VulkanMaterial()
{
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_MATERIAL).FreeSet(__materialDescriptorSet);
    if (__textureDescriptorSet) {
        DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_TEXTURES).FreeSet(__textureDescriptorSet);
    }
}

const DescriptorSet& VulkanMaterial::GetMaterialDescriptorSet()
{
    bool wasDirty;
    const MaterialResourceTable & resTable = _GetResourceTable(wasDirty);
    if (wasDirty) {
        // Update material uniform buffer
        __uniformBuffer.WriteToBuffer(&resTable, sizeof(MaterialResourceTable));

        // Update textures
        if (__textureDescriptorSet) {
            for (int i = 0; i < vc::MaterialComponentType::MAX_COMPONENT; i++) {
                const vc::MaterialComponent & component = GetComponent(static_cast<vc::MaterialComponentType>(i));
                if (component.GetValueType() == vc::MaterialComponentValueType::TEXTURE) {
                    const vc::Texture & texture = component.GetTexture();
                    __textureDescriptorSet->GroupUpdateTexture(texture.GetConstImpl()->ConstAs<VulkanTexture>(), 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, i);
                }
            }
        }
    }
    return __materialDescriptorSet->GetCurrentSet();
}

const DescriptorSet& VulkanMaterial::GetTextureDescriptorSet()
{
    return __textureDescriptorSet->GetCurrentSet();
}
}
}
