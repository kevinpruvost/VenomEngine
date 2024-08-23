///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanSemaphore.cc
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanSemaphore.h"

namespace venom
{
VulkanSemaphore::VulkanSemaphore()
    : __semaphore(VK_NULL_HANDLE)
    , __logicalDevice(VK_NULL_HANDLE)
{
}

VulkanSemaphore::~VulkanSemaphore()
{
    if (__semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(__logicalDevice, __semaphore, nullptr);
    }
}

VulkanSemaphore::VulkanSemaphore(VulkanSemaphore&& other)
    : __semaphore(other.__semaphore)
    , __logicalDevice(other.__logicalDevice)
{
    other.__semaphore = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
}

VulkanSemaphore& VulkanSemaphore::operator=(VulkanSemaphore&& other)
{
    if (this == &other) return *this;
    __semaphore = other.__semaphore;
    __logicalDevice = other.__logicalDevice;
    other.__semaphore = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
}

Error VulkanSemaphore::InitSemaphore(const VkDevice logicalDevice)
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &__semaphore) != VK_SUCCESS) {
        Log::Error("Failed to create semaphore");
        return Error::Failure;
    }
    __logicalDevice = logicalDevice;
    return Error::Success;
}
}