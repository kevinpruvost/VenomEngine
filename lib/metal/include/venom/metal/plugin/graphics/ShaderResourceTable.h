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
namespace metal
{
class MetalShaderResourceTable : public vc::ShaderResourceTable
{
public:
    MetalShaderResourceTable();
    ~MetalShaderResourceTable() override;

private:
    void __UpdateDescriptor(const SetsIndex index, const int binding, const void * data, const size_t size, const size_t offset = 0) override;
    void __UpdateDescriptor(const SetsIndex index, const int binding, vc::Texture * texture) override;
};
}
}