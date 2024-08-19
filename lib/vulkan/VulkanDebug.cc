///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanDebug.cc
/// Date: 8/19/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanDebug.h"

#include <vector>

namespace venom
{

VulkanDebugApplication::VulkanDebugApplication()
{
}

VulkanDebugApplication::~VulkanDebugApplication()
{
}

Error VulkanDebugApplication::initDebug()
{
    Error res;
    if (res = __initValidationLayers(); res != Error::Success)
    {
        Log::Error("Failed to initialize validation layers: %d", res);
        return Error::InitializationFailed;
    }
    return Error::Success;
}

void VulkanDebugApplication::destroyDebug()
{
}

Error VulkanDebugApplication::__initValidationLayers()
{
    constexpr const char * validationLayers[] = {
        "VK_LAYER_KHRONOS_validation"
    };

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char * layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto & layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            Log::Error("Validation Layer not found: %s\n", layerName);
            return Error::Failure;
        }
    }

    return Error::Success;
}

}
