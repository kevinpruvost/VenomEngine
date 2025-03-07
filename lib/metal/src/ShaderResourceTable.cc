///
/// Project: VenomEngineWorkspace
/// @file ShaderResourceTable.cc
/// @date Oct, 24 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/ShaderResourceTable.h>

#include <venom/metal/MetalApplication.h>

namespace venom
{
namespace metal
{
MetalShaderResourceTable::MetalShaderResourceTable()
{
}

MetalShaderResourceTable::~MetalShaderResourceTable()
{
}

void MetalShaderResourceTable::__UpdateDescriptor(const SetsIndex index, const int binding, const void* data,
    const size_t size, const size_t offset)
{
    MetalApplication * app = vc::GraphicsApplication::Get()->DAs<MetalApplication>();
    switch (index)
    {
        case SetsIndex::SetsIndex_Scene: {
            switch (binding) {
                case 0: {
                    break;
                }
            }
            break;
        }
        case SetsIndex::SetsIndex_Material: {
            switch (binding) {
                case 3: {
                    // Roughness for calculations of irradiance/radiance maps
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

void MetalShaderResourceTable::__UpdateDescriptor(const SetsIndex index, const int binding, vc::Texture * texture)
{
    MetalApplication * app = vc::GraphicsApplication::Get()->DAs<MetalApplication>();
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
}
}
}
