///
/// Project: VenomEngineWorkspace
/// @file ShaderResourceTable.h
/// @date Oct, 24 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/ShaderResourceTable.h>

namespace venom
{
namespace vulkan
{
class VulkanShaderResourceTable : public vc::ShaderResourceTable
{
public:
    VulkanShaderResourceTable();
    ~VulkanShaderResourceTable() override;
};
}
}