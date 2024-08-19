///
/// Project: Bazel_Vulkan_Metal
/// File: vulkan.cc
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "Vulkan.h"

#include <vector>

namespace venom
{
VulkanApplication::VulkanApplication()
    : __context()
{
}

VulkanApplication::~VulkanApplication()
{
}

Error VulkanApplication::run()
{
    Error res;

    printf("Hello, Vulkan!\n");
    if (res = __context.initContext(); res != Error::Success)
    {
        Log::Error("Failed to initialize context: %d", res);
        return Error::InitializationFailed;
    }

    if (res = __initVulkan(); res != Error::Success)
    {
        Log::Error("Failed to initialize Vulkan: %d", res);
        return Error::InitializationFailed;
    }
    return venom::Error::Success;
}

Error VulkanApplication::__loop()
{
    while (!__context.shouldClose())
    {
        __context.pollEvents();
    }
    return Error::Success;
}

Error VulkanApplication::__initVulkan()
{
    Error res = Error::Success;

    // Debug Initialization first
    DEBUG_CODE(if (res = initDebug(); res != Error::Success) return res);

    // Create Vulkan instance
    if (res = __createInstance(); res != Error::Success) return res;

    // Debug Code for Vulkan Instance
    DEBUG_CODE(if (res = _postInstance_setDebugParameters(); res != Error::Success) return res);

    // Get Physical Devices
    if (res = __initPhysicalDevices(); res != Error::Success) return res;

    return res;
}

Error VulkanApplication::__initPhysicalDevices()
{
    auto physicalDevices = GetVulkanPhysicalDevices();


    return Error::Success;
}

Error VulkanApplication::__createInstance()
{
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
    __instance_getRequiredExtensions(&createInfo);

    // Set Validation Layers parameters in VulkanDebugApplication
    _preInstance_setDebugParameters(&createInfo);

    if (auto res = vkCreateInstance(&createInfo, nullptr, &VulkanInstance::GetInstance()); res != VK_SUCCESS)
    {
        Log::Error("Failed to create Vulkan instance, error code: %d", res);
#ifdef VENOM_DEBUG
        if (res == VK_ERROR_EXTENSION_NOT_PRESENT) {
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
            Log::Print("Available Extensions:");
            for (const auto& extension : extensions) {
                Log::Print("\t%s", extension.extensionName);
            }
            Log::Print("Extensions passed:");
            for (uint32_t i = 0; i < createInfo.enabledExtensionCount; ++i) {
                Log::Print("\t%s", createInfo.ppEnabledExtensionNames[i]);
            }
        }
#endif
        return Error::InitializationFailed;
    }
    return Error::Success;
}

void VulkanApplication::__instance_getRequiredExtensions(VkInstanceCreateInfo * createInfo)
{
    // We are only using GLFW anyway for Windows, Linux & MacOS and next to Vulkan will only be Metal
    // DX12 will be for another standalone project
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    __instanceExtensions = std::vector<const char *>(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef __APPLE__
    // Might have a bug with MoltenVK
    __instanceExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    createInfo->flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

#ifdef VENOM_DEBUG
    __instanceExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    createInfo->enabledExtensionCount = __instanceExtensions.size();
    createInfo->ppEnabledExtensionNames = __instanceExtensions.data();
}
}

extern "C" venom::ApplicationBackend* createApplication()
{
    return new venom::VulkanApplication();
}