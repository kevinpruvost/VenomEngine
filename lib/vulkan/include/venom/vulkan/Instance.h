///
/// Project: VenomEngine
/// File: VulkanInstance.h
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

namespace venom
{
namespace vulkan
{
class Instance
{
public:
    Instance();
    ~Instance();
    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;
    Instance(Instance&&) = delete;
    Instance& operator=(Instance&&) = delete;

    void __Instance_GetRequiredExtensions(VkInstanceCreateInfo* create_info);
#ifdef VENOM_DEBUG
    void __SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo* create_info, DebugApplication * debugApp);
    static vc::Error CreateInstance(Instance * instance, DebugApplication * debugApp);
#else
    void __SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo* create_info);
    static vc::Error CreateInstance(Instance * instance);
#endif

    static const VkInstance & GetVkInstance();
    static const Instance & GetInstance();

private:
    VkInstance __instance;
    vc::Vector<const char *> __instanceExtensions;
};
}
}