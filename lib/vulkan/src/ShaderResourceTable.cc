///
/// Project: VenomEngineWorkspace
/// @file ShaderResourceTable.cc
/// @date Oct, 24 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/ShaderResourceTable.h>

#include <venom/vulkan/VulkanApplication.h>

namespace venom
{
namespace vulkan
{
VulkanShaderResourceTable::VulkanShaderResourceTable()
{
}

VulkanShaderResourceTable::~VulkanShaderResourceTable()
{
}

void VulkanShaderResourceTable::__UpdateDescriptor(const SetsIndex index, const int binding, const void* data,
    const size_t size, const size_t offset)
{
    VulkanApplication * app = vc::GraphicsApplication::Get()->DAs<VulkanApplication>();
    switch (index)
    {
        case SetsIndex::SetsIndex_Scene: {
            switch (binding) {
                case 0: {
                    app->__sceneSettingsBuffer.WriteToBuffer(data, size, offset);
                    break;
                }
            }
            break;
        }
        case SetsIndex::SetsIndex_Material: {
            switch (binding) {
                case 3: {
                    // Roughness for calculations of irradiance/radiance maps
                    app->__radianceRoughness.WriteToBuffer(data, size, offset);
                    break;
                }
            }
            break;
        }
        default: {
            vc::Log::Error("Unsupported descriptor set index");
            return;
        }
    }
}

void VulkanShaderResourceTable::__UpdateDescriptor(const SetsIndex index, const int binding, vc::Texture * texture)
{
    VulkanApplication * app = vc::GraphicsApplication::Get()->DAs<VulkanApplication>();
    const vc::TextureMemoryAccess & memoryAccess = texture->GetMemoryAccess();
    VkDescriptorType descType;
    switch (memoryAccess) {
        case vc::TextureMemoryAccess::ReadOnly: {
            descType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            break;
        }
        case vc::TextureMemoryAccess::ReadWrite: {
            descType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            break;
        }
        default: {
            vc::Log::Error("Unsupported memory access for texture");
            return;
        }
    }
    DescriptorPool::GetPool()->GetDescriptorSets(index).GroupUpdateTexture(texture->GetConstImpl()->ConstAs<VulkanTexture>(), binding, descType, 1, 0);
}
}
}
