///
/// Project: VenomEngine
/// @file Fence.h
/// @date Aug, 23 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class Fence
{
public:
    Fence();
    ~Fence();
    Fence(const Fence &) = delete;
    Fence & operator=(const Fence &) = delete;
    Fence(Fence && other);
    Fence & operator=(Fence && other);

    vc::Error InitFence(const VkDevice logicalDevice, const VkFenceCreateFlags flags = VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT);
    const VkFence * GetFence() const;

private:
    VkFence __fence;
    VkDevice __logicalDevice;
};
}
}