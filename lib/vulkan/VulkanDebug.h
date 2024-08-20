///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanDebug.h
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanInstance.h"

namespace venom
{
class VulkanDebugApplication : public VulkanInstance
{
public:
    VulkanDebugApplication();
    ~VulkanDebugApplication();

    Error InitDebug();
    void DestroyDebug();

protected:
    void _SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo * createInfos);
    template<typename CreateInfo>
    void _SetCreateInfoValidationLayers(CreateInfo * createInfos);
    Error _PostInstance_SetDebugParameters();

private:
    Error __InitValidationLayers();

#ifdef VENOM_DEBUG
    std::vector<const char *>      __validationLayersInUse;
    std::vector<VkLayerProperties> __validationLayersAvailable;

    VkDebugUtilsMessengerEXT __debugMessenger;
    VkDebugUtilsMessengerCreateInfoEXT __debugMessengerCreateInfo;
#endif
};

}
