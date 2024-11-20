///
/// Project: VenomEngine
/// @file UniformBuffer.h
/// @date Sep, 15 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Buffer.h>

namespace venom
{
namespace vulkan
{
class UniformBuffer
{
public:
    UniformBuffer();
    ~UniformBuffer();
    UniformBuffer(const UniformBuffer&) = delete;
    UniformBuffer& operator=(const UniformBuffer&) = delete;
    UniformBuffer(UniformBuffer&& other);
    UniformBuffer& operator=(UniformBuffer&& other);

    vc::Error Init(const VkDeviceSize size);
    const VkDeviceMemory & GetVkDeviceMemory() const;
    void * GetMappedData() const;
    VkBuffer GetVkBuffer() const;
    VkDeviceSize GetSize() const;
    template<typename T>
    inline void WriteToBuffer(const T * data, size_t size, size_t offset = 0) { memcpy((char *)__mappedData + offset, data, size); }

private:
    Buffer __buffer;
    void * __mappedData;
};
}
}