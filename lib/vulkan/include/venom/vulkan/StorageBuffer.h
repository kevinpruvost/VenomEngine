///
/// Project: VenomEngineWorkspace
/// @file StorageBuffer.h
/// @date Oct, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/vulkan/Buffer.h>

namespace venom
{
namespace vulkan
{
class StorageBuffer
{
public:
    StorageBuffer();
    ~StorageBuffer();
    StorageBuffer(const StorageBuffer&) = delete;
    StorageBuffer& operator=(const StorageBuffer&) = delete;
    StorageBuffer(StorageBuffer&& other);
    StorageBuffer& operator=(StorageBuffer&& other);

    vc::Error Init(const VkDeviceSize size);
    const VkDeviceMemory & GetVkDeviceMemory() const;
    void * GetMappedData() const;
    VkBuffer GetVkBuffer() const;
    VkDeviceSize GetSize() const;

private:
    Buffer __buffer;
    void * __mappedData;
};
}
}