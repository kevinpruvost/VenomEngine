///
/// Project: VenomEngineWorkspace
/// @file Buffer.h
/// @date Mar, 20 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/Buffer.h>

namespace venom
{
namespace metal
{
class MetalBuffer : public vc::BufferImpl
{
public:
    MetalBuffer();
    ~MetalBuffer();

    vc::Error _InitWithSize(uint32_t size) override;
    vc::Error _WriteToBuffer(const void* data, uint32_t size, uint32_t offset) override;
};
}
}