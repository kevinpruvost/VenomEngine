///
/// Project: VenomEngine
/// @file VulkanSemaphore.cc
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Semaphore.h>

namespace venom::vulkan
{
Semaphore::Semaphore()
    : __semaphore(VK_NULL_HANDLE)
    , __logicalDevice(VK_NULL_HANDLE)
{
}

Semaphore::~Semaphore()
{
    DestroySemaphore();
}

Semaphore::Semaphore(Semaphore&& other)
    : __semaphore(other.__semaphore)
    , __logicalDevice(other.__logicalDevice)
{
    other.__semaphore = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
}

Semaphore& Semaphore::operator=(Semaphore&& other)
{
    if (this == &other) return *this;
    __semaphore = other.__semaphore;
    __logicalDevice = other.__logicalDevice;
    other.__semaphore = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
}

void Semaphore::DestroySemaphore()
{
    if (__semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(__logicalDevice, __semaphore, nullptr);
        __semaphore = VK_NULL_HANDLE;
    }
}

vc::Error Semaphore::InitSemaphore(const VkDevice logicalDevice)
{
    DestroySemaphore();

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &__semaphore) != VK_SUCCESS) {
        vc::Log::Error("Failed to create semaphore");
        return vc::Error::Failure;
    }
    __logicalDevice = logicalDevice;
    return vc::Error::Success;
}

VkSemaphore Semaphore::GetSemaphore() const
{
    return __semaphore;
}
}
