///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanFence.cc
/// Date: 8/23/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanFence.h"

namespace venom
{
VulkanFence::VulkanFence()
    : __fence(VK_NULL_HANDLE)
    , __logicalDevice(VK_NULL_HANDLE)
{
}

VulkanFence::~VulkanFence()
{
    if (__fence != VK_NULL_HANDLE) {
        vkDestroyFence(__logicalDevice, __fence, nullptr);
    }
}

VulkanFence::VulkanFence(VulkanFence&& other)
    : __fence(other.__fence)
    , __logicalDevice(other.__logicalDevice)
{
    other.__fence = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
}

VulkanFence& VulkanFence::operator=(VulkanFence&& other)
{
    if (this == &other) return *this;
    __fence = other.__fence;
    __logicalDevice = other.__logicalDevice;
    other.__fence = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
    return *this;
}

Error VulkanFence::InitFence(const VkDevice logicalDevice)
{
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = 0;

    if (vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &__fence) != VK_SUCCESS) {
        Log::Error("Failed to create fence");
        return Error::Failure;
    }
    __logicalDevice = logicalDevice;
    return Error::Success;
}
}
