///
/// Project: VenomEngine
/// @file VulkanInstance.cc
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Instance.h>

namespace venom::vulkan
{

static VkInstance vulkanInstance = VK_NULL_HANDLE;

Instance::Instance()
{
}

Instance::~Instance()
{
    if (vulkanInstance != VK_NULL_HANDLE)
        vkDestroyInstance(vulkanInstance, nullptr);
}

VkInstance & Instance::GetInstance()
{
    return vulkanInstance;
}
}
