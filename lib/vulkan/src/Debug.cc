///
/// Project: VenomEngine
/// @file Debug.cc
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Debug.h>
#include <venom/vulkan/Allocator.h>

#include <venom/vulkan/Instance.h>
#include <vector>

namespace venom::vulkan
{

DebugApplication::DebugApplication()
#ifdef VENOM_DEBUG
    : __validationLayersInUse()
    , __debugMessenger(VK_NULL_HANDLE)
    , __debugMessengerCreateInfo{}
#endif
{
}

DebugApplication::~DebugApplication()
{
}

vc::Error DebugApplication::InitDebug()
{
#ifdef VENOM_DEBUG
    vc::Error res;
    if (res = __InitValidationLayers(); res != vc::Error::Success)
    {
        vc::Log::Error("Failed to initialize validation layers: %d", res);
        return vc::Error::InitializationFailed;
    }
#endif
    return vc::Error::Success;
}

void DebugApplication::DestroyDebug()
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
        vc::Log::Error("[VK_VALIDATION_LAYER] ERROR: %s", pCallbackData->pMessage);
        vc::Log::LogToFile("[VK_VALIDATION_LAYER] ERROR: %s", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        vc::Log::Print("[VK_VALIDATION_LAYER] WARNING: %s", pCallbackData->pMessage);
        vc::Log::LogToFile("[VK_VALIDATION_LAYER] WARNING: %s", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        vc::Log::LogToFile("[VK_VALIDATION_LAYER] INFO: %s", pCallbackData->pMessage);
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        vc::Log::LogToFile("[VK_VALIDATION_LAYER] VERBOSE: %s", pCallbackData->pMessage);
    default: break;
    }
    return VK_FALSE;
}

void DebugApplication::_SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo* createInfos)
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
void DebugApplication::_SetCreateInfoValidationLayers(VkDeviceCreateInfo* createInfos)
{
#ifdef VENOM_DEBUG
    createInfos->enabledLayerCount = static_cast<uint32_t>(__validationLayersInUse.size());
    createInfos->ppEnabledLayerNames = __validationLayersInUse.data();
#else
    createInfos->enabledLayerCount = 0;
    createInfos->ppEnabledLayerNames = nullptr;
#endif
}

vc::Error DebugApplication::_PostInstance_SetDebugParameters()
{
#ifdef VENOM_DEBUG
    // Debug Utils Messenger
    auto createDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(Instance::GetVkInstance(), "vkCreateDebugUtilsMessengerEXT"));
    if (createDebugUtilsMessengerEXT(Instance::GetVkInstance(), &__debugMessengerCreateInfo, Allocator::GetVKAllocationCallbacks(), &__debugMessenger) != VK_SUCCESS) {
        vc::Log::Error("Failed to set up debug messenger!");
        return vc::Error::Failure;
    }
#endif
    return vc::Error::Success;
}

PFN_vkDebugUtilsMessengerCallbackEXT DebugApplication::_GetDebugCallback()
{
    return debugCallback;
}

double DebugApplication::_GetTheoreticalFPS(double fps)
{
    // Average time waited on VSync per frame
    uint64_t timePassed = __fpsTimer.GetMicroSeconds();
    __fpsTimer.Reset();
    uint64_t averageProcessingTimePerFrame = (timePassed - __microsecondsWaitedOnVSync) / fps;
    //printf("Time passed: %lu, Micros; %lu, Average Processing Time Per Frame: %lu\n", timePassed, __microsecondsWaitedOnVSync, averageProcessingTimePerFrame);
    __microsecondsWaitedOnVSync = 0;
    return static_cast<double>(timePassed / averageProcessingTimePerFrame);
}

void DebugApplication::_UpdateTheoreticalFPS(uint32_t microsecondsWaitedOnVSync)
{
    __microsecondsWaitedOnVSync += microsecondsWaitedOnVSync;
}

vc::Error DebugApplication::__InitValidationLayers()
{
#ifdef VENOM_DEBUG
    constexpr const char * validationLayers[] = {
        "VK_LAYER_KHRONOS_validation",
        "VK_LAYER_KHRONOS_shader_object",
        "VK_LAYER_KHRONOS_synchronization2",
    };

    vc::Vector<const char *> validationLayers_finalUse(validationLayers, validationLayers + std::size(validationLayers));
    // Get env variable VK_INSTANCE_LAYERS
    if (const char * envValidationLayers = std::getenv("VK_INSTANCE_LAYERS")) {
        vc::Log::Print("Using VK_INSTANCE_LAYERS: %s", envValidationLayers);
        // Split the string by ';'
        for (const char * layer = strtok(const_cast<char *>(envValidationLayers), ";"); layer; layer = strtok(nullptr, ";")) {
            validationLayers_finalUse.push_back(layer);
        }
    }

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    __validationLayersAvailable = vc::Vector<VkLayerProperties>(layerCount);
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
            vc::Log::Error("Validation Layer not found: %s\n", layerName);
            return vc::Error::Failure;
        }
    }

    // Print validation layers in use
    vc::Log::Print("Validation Layers in use:");
    for (const char * layer : validationLayers_finalUse) {
        vc::Log::Print("\t- %s", layer);
    }

    __validationLayersInUse = {validationLayers, validationLayers + std::size(validationLayers)};
#endif
    return vc::Error::Success;
}

}
