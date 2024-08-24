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
    : VulkanInstance()
#ifdef VENOM_DEBUG
    , __validationLayersInUse()
    , __debugMessenger(VK_NULL_HANDLE)
    , __debugMessengerCreateInfo{}
#endif
{
}

VulkanDebugApplication::~VulkanDebugApplication()
{
#ifdef VENOM_DEBUG
    if (__debugMessenger) {
        auto destroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(VulkanInstance::GetInstance(), "vkDestroyDebugUtilsMessengerEXT"));
        destroyDebugUtilsMessengerEXT(VulkanInstance::GetInstance(), __debugMessenger, nullptr);
    }
#endif
}

Error VulkanDebugApplication::InitDebug()
{
#ifdef VENOM_DEBUG
    Error res;
    if (res = __InitValidationLayers(); res != Error::Success)
    {
        Log::Error("Failed to initialize validation layers: %d", res);
        return Error::InitializationFailed;
    }
#endif
    return Error::Success;
}

void VulkanDebugApplication::DestroyDebug()
{
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    switch (messageSeverity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        Log::Error("[VK_VALIDATION_LAYER] ERROR: %s", pCallbackData->pMessage);
        Log::LogToFile("[VK_VALIDATION_LAYER] ERROR: %s", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        Log::Print("[VK_VALIDATION_LAYER] WARNING: %s", pCallbackData->pMessage);
        Log::LogToFile("[VK_VALIDATION_LAYER] WARNING: %s", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        Log::LogToFile("[VK_VALIDATION_LAYER] INFO: %s", pCallbackData->pMessage);
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        Log::LogToFile("[VK_VALIDATION_LAYER] VERBOSE: %s", pCallbackData->pMessage);
    default: break;
    }
    return VK_FALSE;
}

void VulkanDebugApplication::_SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo* createInfos)
{
    // Validation Layers
#ifdef VENOM_DEBUG
    createInfos->enabledLayerCount = static_cast<uint32_t>(__validationLayersInUse.size());
    createInfos->ppEnabledLayerNames = __validationLayersInUse.data();

    __debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    __debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    __debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    __debugMessengerCreateInfo.pfnUserCallback = debugCallback;
    __debugMessengerCreateInfo.pNext = __debugMessengerCreateInfo.pUserData = nullptr;
    createInfos->pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&__debugMessengerCreateInfo);
#else
    createInfos->enabledLayerCount = 0;
    createInfos->ppEnabledLayerNames = nullptr;
#endif
}

template <>
void VulkanDebugApplication::_SetCreateInfoValidationLayers(VkDeviceCreateInfo* createInfos)
{
#ifdef VENOM_DEBUG
    createInfos->enabledLayerCount = static_cast<uint32_t>(__validationLayersInUse.size());
    createInfos->ppEnabledLayerNames = __validationLayersInUse.data();
#else
    createInfos->enabledLayerCount = 0;
    createInfos->ppEnabledLayerNames = nullptr;
#endif
}

Error VulkanDebugApplication::_PostInstance_SetDebugParameters()
{
#ifdef VENOM_DEBUG
    // Debug Utils Messenger
    auto createDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(VulkanInstance::GetInstance(), "vkCreateDebugUtilsMessengerEXT"));
    if (createDebugUtilsMessengerEXT(VulkanInstance::GetInstance(), &__debugMessengerCreateInfo, nullptr, &__debugMessenger) != VK_SUCCESS) {
        Log::Error("Failed to set up debug messenger!");
        return Error::Failure;
    }
#endif
    return Error::Success;
}

Error VulkanDebugApplication::__InitValidationLayers()
{
#ifdef VENOM_DEBUG
    constexpr const char * validationLayers[] = {
        "VK_LAYER_KHRONOS_validation",
        "VK_LAYER_KHRONOS_shader_object",
        "VK_LAYER_KHRONOS_synchronization2",
    };

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    __validationLayersAvailable = std::vector<VkLayerProperties>(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, __validationLayersAvailable.data());

    for (const char * layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto & layerProperties : __validationLayersAvailable) {
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

    __validationLayersInUse = {validationLayers, validationLayers + std::size(validationLayers)};
#endif
    return Error::Success;
}

}
