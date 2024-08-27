///
/// Project: VenomEngine
/// @file Fence.cc
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Fence.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

namespace venom::vulkan
{
Fence::Fence()
    : __fence(VK_NULL_HANDLE)
{
}

Fence::~Fence()
{
    if (__fence != VK_NULL_HANDLE) {
        vkDestroyFence(LogicalDevice::GetVkDevice(), __fence, Allocator::GetVKAllocationCallbacks());
    }
}

Fence::Fence(Fence&& other)
    : __fence(other.__fence)
{
    other.__fence = VK_NULL_HANDLE;
}

Fence& Fence::operator=(Fence&& other)
{
    if (this == &other) return *this;
    __fence = other.__fence;
    other.__fence = VK_NULL_HANDLE;
    return *this;
}

vc::Error Fence::InitFence(const VkFenceCreateFlags flags)
{
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = flags;

    if (vkCreateFence(LogicalDevice::GetVkDevice(), &fenceCreateInfo, Allocator::GetVKAllocationCallbacks(), &__fence) != VK_SUCCESS) {
        vc::Log::Error("Failed to create fence");
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

const VkFence * Fence::GetFence() const
{
    return &__fence;
}
}
