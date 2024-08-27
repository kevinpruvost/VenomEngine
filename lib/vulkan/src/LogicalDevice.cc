///
/// Project: Bazel_Vulkan_Metal
/// @file LogicalDevice.cc
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <memory>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

namespace venom
{
namespace vulkan
{
LogicalDevice::LogicalDevice()
    : __device(VK_NULL_HANDLE)
{
}

LogicalDevice::~LogicalDevice()
{
    if (__device != VK_NULL_HANDLE)
        vkDestroyDevice(__device, Allocator::GetVKAllocationCallbacks());
}

LogicalDevice::LogicalDevice(LogicalDevice&& other)
    : __device(other.__device)
{
    other.__device = VK_NULL_HANDLE;
}

LogicalDevice& LogicalDevice::operator=(LogicalDevice&& other)
{
    if (this == &other) return *this;
    __device = other.__device;
    other.__device = VK_NULL_HANDLE;
    return *this;
}

LogicalDevice::operator struct VkDevice_T*() const
{
    return __device;
}

static const LogicalDevice * s_instance = nullptr;

void LogicalDevice::CreateInstance(const LogicalDevice * device)
{
    venom_assert(device != VK_NULL_HANDLE, "LogicalDevice::CreateInstance: device is VK_NULL_HANDLE");
    s_instance = device;
}

const LogicalDevice& LogicalDevice::GetInstance()
{
    return *s_instance;
}

const VkDevice LogicalDevice::GetVkDevice()
{
    return s_instance->__device;
}
}
}
