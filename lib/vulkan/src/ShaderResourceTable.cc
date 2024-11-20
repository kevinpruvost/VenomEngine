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
        case SetsIndex::SETS_INDEX_SCENE: {
            switch (binding) {
                case 0: break;
                case 1: {
                    app->__screenPropsBuffer.WriteToBuffer(data, size, offset);
                    break;
                }
            }
            break;
        }
    }
}
}
}