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

Error VulkanApplication::Run()
{
    Error res;

    printf("Hello, Vulkan!\n");
    if (res = __context.InitContext(); res != Error::Success)
    {
        Log::Error("Failed to initialize context: %d", res);
        return Error::InitializationFailed;
    }

    if (res = __InitVulkan(); res != Error::Success)
    {
        Log::Error("Failed to initialize Vulkan: %d", res);
        return Error::InitializationFailed;
    }
    return venom::Error::Success;
}

Error VulkanApplication::__Loop()
{
    while (!__context.ShouldClose())
    {
        __context.PollEvents();
    }
    return Error::Success;
}

Error VulkanApplication::__InitVulkan()
{
    Error res = Error::Success;

    // Debug Initialization first
    DEBUG_CODE(if (res = InitDebug(); res != Error::Success) return res);

    // Create Vulkan instance
    if (res = __CreateInstance(); res != Error::Success) return res;

    // Debug Code for Vulkan Instance
    DEBUG_CODE(if (res = _PostInstance_SetDebugParameters(); res != Error::Success) return res);

    // Get Physical Devices
    if (res = __InitPhysicalDevices(); res != Error::Success) return res;

    // Get Queue Families


    return res;
}

Error VulkanApplication::__InitPhysicalDevices()
{
    std::vector<VulkanPhysicalDevice> physicalDevices = GetVulkanPhysicalDevices();

    if (physicalDevices.empty())
    {
        Log::Error("Failed to find GPUs with Vulkan support");
        return Error::InitializationFailed;
    }

    DEBUG_LOG("Physical Devices:");
    for (int i = 0; i < physicalDevices.size(); ++i) {
        DEBUG_LOG("-%s:", physicalDevices[i].properties.deviceName);
        DEBUG_LOG("\tType: %s", physicalDevices[i].properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Discrete" : physicalDevices[i].properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "Integrated" : "Other Type of GPU");
        DEBUG_LOG("\tAPI Version: %u", physicalDevices[i].properties.apiVersion);
        DEBUG_LOG("\tDriver Version: %u", physicalDevices[i].properties.driverVersion);
        DEBUG_LOG("\tVendor ID: %u", physicalDevices[i].properties.vendorID);
        DEBUG_LOG("\tDevice ID: %u", physicalDevices[i].properties.deviceID);
        DEBUG_LOG("\tGeometry Shader: %s", physicalDevices[i].features.geometryShader ? "Yes" : "No");
        DEBUG_LOG("\tTesselation Shader: %s", physicalDevices[i].features.tessellationShader ? "Yes" : "No");
        for (int j = 0; j < physicalDevices[i].memoryProperties.memoryHeapCount; ++j) {
            DEBUG_LOG("\tHeap %d: %luMB", j, physicalDevices[i].memoryProperties.memoryHeaps[j].size / (1024 * 1024));
        }
        // Select GPU
        if (physicalDevices[i].properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
         && physicalDevices[i].features.geometryShader
         && physicalDevices[i].features.tessellationShader) {
            if (__physicalDevice.physicalDevice == VK_NULL_HANDLE || __physicalDevice.GetDeviceLocalVRAMAmount() < physicalDevices[i].GetDeviceLocalVRAMAmount()) {
                __physicalDevice = physicalDevices[i];
            }
        }
    }
    DEBUG_LOG("Chosen phyiscal device:");
    DEBUG_LOG("-%s:", __physicalDevice.properties.deviceName);
    DEBUG_LOG("Device Local VRAM: %luMB", __physicalDevice.GetDeviceLocalVRAMAmount() / (1024 * 1024));

    // Get Queue Families
    __queueFamilies = getVulkanQueueFamilies(__physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = __queueFamilies.graphicsQueueFamilyIndices[0];
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &__physicalDevice.features;

    if (auto res = vkCreateDevice(__physicalDevice.physicalDevice, &createInfo, nullptr, &__physicalDevice.logicalDevice); res != VK_SUCCESS)
    {
        Log::Error("Failed to create logical device, error code: %d", res);
        return Error::InitializationFailed;
    }

    VkQueue queue;
    __physicalDevice.GetDeviceQueue(&queue, __queueFamilies.graphicsQueueFamilyIndices[0], 0);

    return Error::Success;
}

Error VulkanApplication::__CreateInstance()
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
    __Instance_GetRequiredExtensions(&createInfo);

    // Set Validation Layers parameters in VulkanDebugApplication
    _PreInstance_SetDebugParameters(&createInfo);

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

void VulkanApplication::__Instance_GetRequiredExtensions(VkInstanceCreateInfo * createInfo)
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