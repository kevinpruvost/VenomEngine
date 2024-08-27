///
/// Project: Bazel_Vulkan_Metal
/// @file LogicalDevice.h
/// @date Aug, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>
#include <venom/vulkan/PhysicalDevice.h>

namespace venom
{
namespace vulkan
{
class LogicalDevice
{
private:
    LogicalDevice();
public:
    ~LogicalDevice();
    LogicalDevice(const LogicalDevice&) = delete;
    LogicalDevice& operator=(const LogicalDevice&) = delete;
    LogicalDevice(LogicalDevice&& other);
    LogicalDevice& operator=(LogicalDevice&& other);

    explicit operator VkDevice() const;
    static void CreateInstance(const VkDevice device);
    static LogicalDevice & GetInstance();
    static const VkDevice GetVkDevice();

private:
    VkDevice __device;
};
}
}
