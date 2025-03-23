///
/// Project: VenomEngineWorkspace
/// @file Buffer.h
/// @date Mar, 19 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPluginObject.h>

namespace venom
{
namespace common
{

enum class BufferType
{
    None = -1,
    Read,
    WriteRead
};

class VENOM_COMMON_API BufferImpl : public GraphicsPluginObject
{
public:
    BufferImpl();
    virtual ~BufferImpl() override;

    inline BufferType GetBufferType() const { return _type; }
    void SetBufferType(BufferType type);
    vc::Error InitWithSize(uint32_t size);
    vc::Error WriteToBuffer(const void* data, uint32_t size, uint32_t offset = 0);

protected:
    virtual vc::Error _InitWithSize(uint32_t size) = 0;
    virtual vc::Error _WriteToBuffer(const void* data, uint32_t size, uint32_t offset) = 0;
    BufferType _type;
    uint32_t _size;
};

class VENOM_COMMON_API ReadBuffer : public PluginObjectWrapper
{
public:
    ReadBuffer();
    ~ReadBuffer();
};

class VENOM_COMMON_API WriteReadBuffer : public PluginObjectWrapper
{
public:
    WriteReadBuffer();
    ~WriteReadBuffer();
};

}
}