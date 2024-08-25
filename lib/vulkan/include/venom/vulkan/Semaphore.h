///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanSemaphore.h
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class Semaphore
{
public:
    Semaphore();
    ~Semaphore();
    Semaphore(const Semaphore &) = delete;
    Semaphore & operator=(const Semaphore &) = delete;
    Semaphore(Semaphore && other);
    Semaphore & operator=(Semaphore && other);

    void DestroySemaphore();
    Error InitSemaphore(const VkDevice logicalDevice);
    VkSemaphore GetSemaphore() const;

private:
    VkSemaphore __semaphore;
    VkDevice __logicalDevice;
};
}
}