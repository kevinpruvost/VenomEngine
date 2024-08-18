///
/// Project: Bazel_Vulkan_Metal
/// File: vulkan.cc
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "vulkan.h"

#include "common/Log.h"

namespace venom
{
VulkanApplication::VulkanApplication()
    : __context()
    , __instance(VK_NULL_HANDLE)
{
}

VulkanApplication::~VulkanApplication()
{
    if (__instance != VK_NULL_HANDLE)
        vkDestroyInstance(__instance, nullptr);
}

Error VulkanApplication::run()
{
    printf("Hello, Vulkan!\n");
    __context.initContext();
    __initVulkan();
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
    // Create Vulkan instance
    __createInstance();

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

    // We are only using GLFW anyway for Windows, Linux & MacOS and next to Vulkan will only be Metal
    // DX12 will be for another standalone project
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

#ifdef __APPLE__
    // Might have a bug with MoltenVK
    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;
    if (auto res = vkCreateInstance(&createInfo, nullptr, &__instance); res != VK_SUCCESS)
    {
        Log::Error("Failed to create Vulkan instance, error code: %d", res);
#ifdef _DEBUG
        if (res == VK_ERROR_EXTENSION_NOT_PRESENT) {
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
            Log::Print("Available Extensions:");
            for (const auto& extension : extensions) {
                Log::Print("\t%s\n", extension.extensionName);
            }
            Log::Print("Extensions passed:");
            for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
                Log::Print("\t%s\n", glfwExtensions[i]);
            }
        }
#endif
        return Error::InitializationFailed;
    }
    return Error::Success;
}
}

extern "C" venom::ApplicationBackend* createApplication()
{
    return new venom::VulkanApplication();
}