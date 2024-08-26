///
/// Project: VenomEngine
/// @file GraphicsApplication.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>

#include <array>
#include <vector>

namespace venom::vulkan
{
/// @brief Device extensions to use
static constexpr std::array s_deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VulkanApplication::VulkanApplication()
    : __context()
    , __currentFrame(0)
    , __framebufferChanged(false)
{
}

VulkanApplication::~VulkanApplication()
{
    vc::Log::Print("Destroying Vulkan app...");
}

vc::Error VulkanApplication::Run()
{
    vc::Error res;

    printf("Hello, Vulkan!\n");
    if (res = __context.InitContext(); res != vc::Error::Success)
    {
        vc::Log::Error("Failed to initialize context: %d", res);
        return vc::Error::InitializationFailed;
    }

    __SetGLFWCallbacks();

    if (res = __InitVulkan(); res != vc::Error::Success)
    {
        vc::Log::Error("Failed to initialize Vulkan: %d", res);
        return vc::Error::InitializationFailed;
    }

    // Test
    __shaderPipeline.LoadShaders(__physicalDevice.logicalDevice, &__swapChain, &__renderPass, {
        "shader.ps",
        "shader.vs"
    });

    if (res = __Loop(); res != vc::Error::Success)
    {
        vc::Log::Error("Failed to run loop: %d", res);
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error VulkanApplication::__Loop()
{
    vc::Error err;
    while (!__context.ShouldClose())
    {
        __context.PollEvents();
        if (err = __DrawFrame(); err != vc::Error::Success)
            return err;
    }

    vkDeviceWaitIdle(__physicalDevice.logicalDevice);
    return vc::Error::Success;
}

vc::Error VulkanApplication::__DrawFrame()
{
    // Draw image
    vkWaitForFences(__physicalDevice.logicalDevice, 1, __inFlightFences[__currentFrame].GetFence(), VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(__physicalDevice.logicalDevice, __swapChain.swapChain, UINT64_MAX, __imageAvailableSemaphores[__currentFrame].GetSemaphore(), VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || __framebufferChanged) {
        __framebufferChanged = false;
        vc::Log::Print("Recreating swap chain");
        __RecreateSwapChain();
        return vc::Error::Success;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        vc::Log::Error("Failed to acquire swap chain image");
        return vc::Error::Failure;
    }

    // If we reset before, then it will wait endlessly as no work is done
    vkResetFences(__physicalDevice.logicalDevice, 1, __inFlightFences[__currentFrame].GetFence());
    __commandBuffers[__currentFrame]->Reset(0);

    if (auto err = __commandBuffers[__currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __renderPass.BeginRenderPass(&__swapChain, __commandBuffers[__currentFrame], imageIndex);
        __commandBuffers[__currentFrame]->BindPipeline(__shaderPipeline.GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
        __commandBuffers[__currentFrame]->SetViewport(__swapChain.viewport);
        __commandBuffers[__currentFrame]->SetScissor(__swapChain.scissor);
        __commandBuffers[__currentFrame]->Draw(3, 1, 0, 0);
        __renderPass.EndRenderPass(__commandBuffers[__currentFrame]);

    if (auto err = __commandBuffers[__currentFrame]->EndCommandBuffer(); err != vc::Error::Success)
        return err;

    // Synchronization between the image being presented and the image being rendered
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {__imageAvailableSemaphores[__currentFrame].GetSemaphore()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffers[] = {*reinterpret_cast<VkCommandBuffer*>(__commandBuffers[__currentFrame])};
    submitInfo.pCommandBuffers = commandBuffers;
    VkSemaphore signalSemaphores[] = {__renderFinishedSemaphores[__currentFrame].GetSemaphore()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (result = vkQueueSubmit(__graphicsQueue, 1, &submitInfo, *__inFlightFences[__currentFrame].GetFence()); result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || __framebufferChanged) {
        __framebufferChanged = false;
        __RecreateSwapChain();
        return vc::Error::Success;
    } else if (result != VK_SUCCESS) {
        vc::Log::Error("Failed to submit draw command buffer");
        return vc::Error::Failure;
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {__swapChain.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(__presentQueue, &presentInfo);

    __currentFrame = (__currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    return vc::Error::Success;
}

vc::Error VulkanApplication::__InitVulkan()
{
    vc::Error res = vc::Error::Success;

    // Debug Initialization first
    DEBUG_CODE(if (res = InitDebug(); res != vc::Error::Success) return res);

    // Create Vulkan instance
    if (res = __CreateInstance(); res != vc::Error::Success) return res;

    // Debug Code for Vulkan Instance
    DEBUG_CODE(if (res = _PostInstance_SetDebugParameters(); res != vc::Error::Success) return res);

    // Get Physical Devices
    if (res = __InitPhysicalDevices(); res != vc::Error::Success) return res;

    return res;
}

void VulkanApplication::__SetGLFWCallbacks()
{
    glfwSetWindowUserPointer(__context.GetWindow(), this);
    glfwSetFramebufferSizeCallback(__context.GetWindow(), [](GLFWwindow * window, int width, int height) {
        auto app = reinterpret_cast<VulkanApplication*>(glfwGetWindowUserPointer(window));
        app->__framebufferChanged = true;
    });
}

vc::Error VulkanApplication::__InitPhysicalDevices()
{
    vc::Error err;
    std::vector<PhysicalDevice> physicalDevices = GetVulkanPhysicalDevices();

    if (physicalDevices.empty())
    {
        vc::Log::Error("Failed to find GPUs with Vulkan support");
        return vc::Error::InitializationFailed;
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

    // Create Surface
    __surface.CreateSurface(&__context);

    // Check if the device supports the surface for presentation and which queue family supports it
    if (err = __queueFamilies.SetPresentQueueFamilyIndices(__physicalDevice, __surface); err != vc::Error::Success)
        return err;

    // Create Logical device
    // Create Queue Create Infos
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    float queuePriority = 1.0f;
    // Graphics Queue
    queueCreateInfos.emplace_back(
        VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = __queueFamilies.graphicsQueueFamilyIndices[0],
            .queueCount = 1,
            .pQueuePriorities = new float(1.0f)
        }
    );
    queueCreateInfos[0].pQueuePriorities = &queuePriority;

    // Present Queue
    if (queueCreateInfos[0].queueFamilyIndex != __queueFamilies.presentQueueFamilyIndices[0]) {
        queueCreateInfos.emplace_back(
            VkDeviceQueueCreateInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = __queueFamilies.presentQueueFamilyIndices[0],
                .queueCount = 1,
                .pQueuePriorities = new float(1.0f)
            }
        );
        queueCreateInfos[1].pQueuePriorities = &queuePriority;
    }

    VkDeviceCreateInfo createInfo{};
    VkPhysicalDeviceFeatures deviceFeatures{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;

    // Extensions
    createInfo.enabledExtensionCount = s_deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = s_deviceExtensions.data();

    // Validation Layers
    _SetCreateInfoValidationLayers(&createInfo);

    // Create Swap Chain
    if (err = __swapChain.InitSwapChainSettings(&__physicalDevice, &__surface, &__context); err != vc::Error::Success)
        return err;

    // Verify if the device is suitable
    if (!__IsDeviceSuitable(&createInfo))
    {
        vc::Log::Error("Physical Device not suitable for Vulkan");
        return vc::Error::InitializationFailed;
    }

    if (VkResult res = vkCreateDevice(__physicalDevice.physicalDevice, &createInfo, nullptr, &__physicalDevice.logicalDevice); res != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create logical device, error code: %d", res);
        return vc::Error::InitializationFailed;
    }

    // Create SwapChain
    if (err = __swapChain.InitSwapChain(&__physicalDevice, &__surface, &__context, &__queueFamilies); err != vc::Error::Success)
        return err;

    // Create Graphics Queue
    __physicalDevice.GetDeviceQueue(&__graphicsQueue, __queueFamilies.graphicsQueueFamilyIndices[0], 0);

    // Create Present Queue
    __physicalDevice.GetDeviceQueue(&__presentQueue, __queueFamilies.presentQueueFamilyIndices[0], 0);

    // Create Render Pass
    if (err = __renderPass.InitRenderPass(__physicalDevice.logicalDevice, &__swapChain); err != vc::Error::Success)
        return err;

    // Init Render Pass Framebuffers
    if (err = __swapChain.InitSwapChainFramebuffers(&__renderPass); err != vc::Error::Success)
        return err;

    // Create Graphics Command Pool
    if (err = __commandPool.InitCommandPool(__physicalDevice.logicalDevice, __queueFamilies.graphicsQueueFamilyIndices[0]); err != vc::Error::Success)
        return err;

    // Create Command Buffers
    __commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __commandPool.CreateCommandBuffer(&__commandBuffers[i]); err != vc::Error::Success)
            return err;
    }

    // Create Semaphores & Fences
    __imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    __renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    __inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __imageAvailableSemaphores[i].InitSemaphore(__physicalDevice.logicalDevice); err != vc::Error::Success)
            return err;
        if (err =__renderFinishedSemaphores[i].InitSemaphore(__physicalDevice.logicalDevice); err != vc::Error::Success)
            return err;
        if (err =__inFlightFences[i].InitFence(__physicalDevice.logicalDevice, VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT); err != vc::Error::Success)
            return err;
    }

    return vc::Error::Success;
}

bool VulkanApplication::__IsDeviceSuitable(const VkDeviceCreateInfo * createInfo)
{
    // Check if the device supports the extensions
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(__physicalDevice.physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(__physicalDevice.physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(createInfo->ppEnabledExtensionNames, createInfo->ppEnabledExtensionNames + createInfo->enabledExtensionCount);
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    if (!requiredExtensions.empty()) {
        vc::Log::Error("Missing required extensions:");
        for (const auto& extension : requiredExtensions) {
            vc::Log::Error("\t%s", extension);
        }
        return false;
    }

    // Check if the device's swap chain is ok
    if (__swapChain.presentModes.empty() || __swapChain.surfaceFormats.empty()) {
        vc::Log::Error("Failed to get surface formats or present modes for swap chain");
        return false;
    }
    return true;
}

vc::Error VulkanApplication::__CreateInstance()
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
    _SetInstanceCreateInfoValidationLayers(&createInfo);

    if (auto res = vkCreateInstance(&createInfo, nullptr, &Instance::GetInstance()); res != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create Vulkan instance, error code: %d", res);
#ifdef VENOM_DEBUG
        if (res == VK_ERROR_EXTENSION_NOT_PRESENT) {
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
            vc::Log::Print("Available Extensions:");
            for (const auto& extension : extensions) {
                vc::Log::Print("\t%s", extension.extensionName);
            }
            vc::Log::Print("Extensions passed:");
            for (uint32_t i = 0; i < createInfo.enabledExtensionCount; ++i) {
                vc::Log::Print("\t%s", createInfo.ppEnabledExtensionNames[i]);
            }
        }
#endif
        return vc::Error::InitializationFailed;
    }
    return vc::Error::Success;
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
    vc::Log::Print("Instance Extensions:");
    for (const char * extension : __instanceExtensions) {
        vc::Log::Print("\t-%s", extension);
    }
#endif

    createInfo->enabledExtensionCount = __instanceExtensions.size();
    createInfo->ppEnabledExtensionNames = __instanceExtensions.data();
}

void VulkanApplication::__RecreateSwapChain()
{
    vkDeviceWaitIdle(__physicalDevice.logicalDevice);
    __swapChain.InitSwapChainSettings(&__physicalDevice, &__surface, &__context);
    __swapChain.InitSwapChain(&__physicalDevice, &__surface, &__context, &__queueFamilies);
    __swapChain.InitSwapChainFramebuffers(&__renderPass);

    // We also need to reset the last used semaphore
    __imageAvailableSemaphores[__currentFrame].InitSemaphore(__physicalDevice.logicalDevice);
}
}