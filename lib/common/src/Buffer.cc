///
/// Project: VenomEngineWorkspace
/// @file Buffer.cc
/// @date Mar, 19 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Buffer.h>

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

namespace venom
{
namespace common
{
BufferImpl::BufferImpl()
    : _type(BufferType::None)
    , _size(0)
{
}

BufferImpl::~BufferImpl()
{
}

void BufferImpl::SetBufferType(BufferType type)
{
    venom_assert(_type == BufferType::None, "Buffer type already set");
    _type = type;
}

vc::Error BufferImpl::InitWithSize(uint32_t size)
{
    const vc::Error err = _InitWithSize(size);
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to initialize buffer");
    }
    _size = size;
    return err;
}


vc::Error BufferImpl::WriteToBuffer(const void* data, uint32_t size, uint32_t offset)
{
    venom_assert(_type != BufferType::None, "Buffer type not set");
    venom_assert(offset + size <= _size, "Offset + size is greater than buffer size");
    const vc::Error err = _WriteToBuffer(data, size, offset);
    if (err != vc::Error::Success) {
        vc::Log::Error("BufferImpl::WriteToBuffer() : Failed to write to buffer");
    }
    return err;
}

ReadBuffer::ReadBuffer()
    : PluginObjectWrapper(GraphicsPlugin::Get()->CreateBuffer())
{
    _impl->As<BufferImpl>()->SetBufferType(BufferType::Read);
}

ReadBuffer::~ReadBuffer()
{
}

WriteReadBuffer::WriteReadBuffer()
    : PluginObjectWrapper(GraphicsPlugin::Get()->CreateBuffer())
{
    _impl->As<BufferImpl>()->SetBufferType(BufferType::WriteRead);
}

WriteReadBuffer::~WriteReadBuffer()
{
}
}
}
