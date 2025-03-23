///
/// Project: VenomEngineWorkspace
/// @file Buffer.mm
/// @date Mar, 20 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/Buffer.h>

namespace venom
{
namespace metal
{
MetalBuffer::MetalBuffer()
{
}

MetalBuffer::~MetalBuffer()
{
}

vc::Error MetalBuffer::_InitWithSize(uint32_t size)
{
    return vc::Error::Success;
}

vc::Error MetalBuffer::_WriteToBuffer(const void* data, uint32_t size, uint32_t offset)
{
    return vc::Error::Success;
}
}
}