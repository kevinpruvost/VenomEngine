///
/// Project: Bazel_Vulkan_Metal
/// @file Buffer.h
/// @date Aug, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class Buffer
{
public:
    Buffer();
    ~Buffer();
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&& other);
    Buffer& operator=(Buffer&& other);

    vc::Error CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags flags, const VkSharingMode sharingMode, const VkMemoryPropertyFlags memoryProperties);
    vc::Error WriteBuffer(const void* data);
    VkBuffer GetVkBuffer() const;
    const VkDeviceMemory & GetVkDeviceMemory() const;

private:
    vc::Error __BindBufferMemory();

private:
    VkBufferCreateInfo __bufferCreateInfo;
    VkBuffer __buffer;
    VkDeviceMemory __memory;
};
}
}