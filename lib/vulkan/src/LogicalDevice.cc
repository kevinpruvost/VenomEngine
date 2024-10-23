///
/// Project: VenomEngine
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
static const LogicalDevice * s_instance = nullptr;

LogicalDevice::LogicalDevice()
    : __device(VK_NULL_HANDLE)
{
}

LogicalDevice::~LogicalDevice()
{
    if (__device != VK_NULL_HANDLE)
        vkDestroyDevice(__device, Allocator::GetVKAllocationCallbacks());
    s_instance = nullptr;
}

LogicalDevice::LogicalDevice(LogicalDevice&& other)
    : __device(other.__device)
{
    other.__device = VK_NULL_HANDLE;
}

LogicalDevice& LogicalDevice::operator=(LogicalDevice&& other)
{
    if (this != &other) {
        __device = other.__device;
        other.__device = VK_NULL_HANDLE;
    }
    return *this;
}

LogicalDevice::operator struct VkDevice_T*() const
{
    return __device;
}

vc::Error LogicalDevice::Init(const VkDeviceCreateInfo* createInfo)
{
    auto test1 = PhysicalDevice::GetUsedVkPhysicalDevice();
    auto test2 = Allocator::GetVKAllocationCallbacks();
    auto test3 = &__device;
    if (VkResult res = vkCreateDevice(PhysicalDevice::GetUsedVkPhysicalDevice(), createInfo, Allocator::GetVKAllocationCallbacks(), &__device); res != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create logical device, error code: %d", res);
        return vc::Error::InitializationFailed;
    }
    s_instance = this;
    return vc::Error::Success;
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
