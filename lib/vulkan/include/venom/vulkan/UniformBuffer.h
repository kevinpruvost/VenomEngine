///
/// Project: Bazel_Vulkan_Metal
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

private:
    Buffer __buffer;
    void * __mappedData;
};
}
}