///
/// Project: VenomEngine
/// @file Debug.cc
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/Debug.h>

#include <vector>

namespace venom::metal
{

DebugApplication::DebugApplication()
{
}

DebugApplication::~DebugApplication()
{
}

vc::Error DebugApplication::InitDebug()
{
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
}

template <>
void DebugApplication::_SetCreateInfoValidationLayers(VkDeviceCreateInfo* createInfos)
{
}

vc::Error DebugApplication::_PostInstance_SetDebugParameters()
{
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
    return vc::Error::Success;
}

}
