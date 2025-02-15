///
/// Project: VenomEngine
/// @file VulkanSemaphore.cc
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Semaphore.h>
#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/LogicalDevice.h>

namespace venom::vulkan
{
Semaphore::Semaphore()
    : __semaphore(VK_NULL_HANDLE)
{
}

Semaphore::~Semaphore()
{
    DestroySemaphore();
}

Semaphore::Semaphore(Semaphore&& other)
    : __semaphore(other.__semaphore)
{
    other.__semaphore = VK_NULL_HANDLE;
}

Semaphore& Semaphore::operator=(Semaphore&& other)
{
    if (this != &other) {
        __semaphore = other.__semaphore;
        other.__semaphore = VK_NULL_HANDLE;
    }
    return *this;
}

void Semaphore::DestroySemaphore()
{
    if (__semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(LogicalDevice::GetVkDevice(), __semaphore, Allocator::GetVKAllocationCallbacks());
        __semaphore = VK_NULL_HANDLE;
    }
}

vc::Error Semaphore::InitSemaphore()
{
    DestroySemaphore();

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    if (VkResult res = vkCreateSemaphore(LogicalDevice::GetVkDevice(), &semaphoreInfo, Allocator::GetVKAllocationCallbacks(), &__semaphore); res != VK_SUCCESS) {
        vc::Log::Error("Failed to create semaphore: %d", res);
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}
}
