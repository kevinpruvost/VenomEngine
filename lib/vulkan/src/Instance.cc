///
/// Project: VenomEngine
/// @file VulkanInstance.cc
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Instance.h>
#include <venom/vulkan/Allocator.h>

#if defined(VENOM_PLATFORM_APPLE)
#include <vulkan/vulkan_metal.h>
#include <vulkan/vulkan_macos.h>
#endif

#include <memory>

namespace venom::vulkan
{

// s_instace MUST NOT OWN the instance, only the VulkanApplication should
static Instance * s_instance(nullptr);

Instance::Instance()
    : __instance(VK_NULL_HANDLE)
{
}

Instance::~Instance()
{
    auto get = Allocator::GetVKAllocationCallbacks();
    if (__instance != VK_NULL_HANDLE)
        vkDestroyInstance(__instance, Allocator::GetVKAllocationCallbacks());
}

void Instance::__Instance_GetRequiredExtensions(VkInstanceCreateInfo* createInfos)
{
    // We are only using GLFW anyway for Windows, Linux & MacOS and next to Vulkan will only be Metal
    // DX12 will be for another standalone project
#if !defined(VENOM_DISABLE_GLFW)
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    __instanceExtensions = vc::Vector<const char *>(glfwExtensions, glfwExtensions + glfwExtensionCount);
#endif

    __instanceExtensions.emplace_back(VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME);
#if defined(VENOM_PLATFORM_APPLE)
    // Might have a bug with MoltenVK
    __instanceExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
    __instanceExtensions.emplace_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
    __instanceExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#if defined(VENOM_PLATFORM_MACOS)
    __instanceExtensions.emplace_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#endif
    __instanceExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    createInfos->flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

#if defined(VENOM_DEBUG)
    __instanceExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    vc::Log::Print("Instance Extensions:");
    for (const char * extension : __instanceExtensions) {
        vc::Log::Print("\t-%s", extension);
    }
#endif

    createInfos->enabledExtensionCount = __instanceExtensions.size();
    createInfos->ppEnabledExtensionNames = __instanceExtensions.data();
}

#if defined(VENOM_DEBUG)
void Instance::__SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo* createInfos, DebugApplication * debugApp)
#else
void Instance::__SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo* createInfos)
#endif
{
    // Validation Layers
#if defined(VENOM_DEBUG)
    createInfos->enabledLayerCount = static_cast<uint32_t>(debugApp->__validationLayersInUse.size());
    createInfos->ppEnabledLayerNames = debugApp->__validationLayersInUse.data();

    debugApp->__debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugApp->__debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugApp->__debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugApp->__debugMessengerCreateInfo.pfnUserCallback = debugApp->_GetDebugCallback();
    debugApp->__debugMessengerCreateInfo.pNext = debugApp->__debugMessengerCreateInfo.pUserData = nullptr;
    createInfos->pNext = &debugApp->__debugMessengerCreateInfo;
#else
    createInfos->enabledLayerCount = 0;
    createInfos->ppEnabledLayerNames = nullptr;
#endif
}

#if defined(VENOM_DEBUG)
vc::Error Instance::CreateInstance(Instance * instance, DebugApplication * debugApp)
#else
vc::Error Instance::CreateInstance(Instance * instance)
#endif
{
    s_instance = instance;

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "VenomEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Get required extensions
    s_instance->__Instance_GetRequiredExtensions(&createInfo);

    // Set Validation Layers parameters in VulkanDebugApplication
#if defined(VENOM_DEBUG)
    s_instance->__SetInstanceCreateInfoValidationLayers(&createInfo, debugApp);
#else
    s_instance->__SetInstanceCreateInfoValidationLayers(&createInfo);
#endif

    if (auto res = vkCreateInstance(&createInfo, Allocator::GetVKAllocationCallbacks(), &s_instance->__instance); res != VK_SUCCESS)
    {
        vc::Log::Error("Error during Vulkan instance creation, error code: %d", res);
#if defined(VENOM_DEBUG)
        if (res == VK_ERROR_EXTENSION_NOT_PRESENT) {
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            vc::Vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
            vc::Log::Print("Available Extensions:");
            for (const auto& extension : extensions) {
                vc::Log::Print("\t%s", extension.extensionName);
            }
            vc::Log::Print("Extensions passed:");
            for (uint32_t i = 0; i < createInfo.enabledExtensionCount; ++i) {
                vc::Log::Print("\t%s", createInfo.ppEnabledExtensionNames[i]);
            }
        } else if (res == VK_ERROR_LAYER_NOT_PRESENT) {
            uint32_t layerCount = 0;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
            vc::Vector<VkLayerProperties> layers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, layers.data());
            vc::Log::Print("Available Layers:");
            for (const auto& layer : layers) {
                vc::Log::Print("\t%s", layer.layerName);
            }
            vc::Log::Print("Layers passed:");
            for (uint32_t i = 0; i < createInfo.enabledLayerCount; ++i) {
                vc::Log::Print("\t%s", createInfo.ppEnabledLayerNames[i]);
            }
        } else {
            vc::Log::Print("Error code: %d", res);
        }
#endif
        return vc::Error::InitializationFailed;
    }
    return vc::Error::Success;
}

const VkInstance & Instance::GetVkInstance()
{
    return s_instance->__instance;
}

const Instance& Instance::GetInstance()
{
    return *s_instance;
}
}
