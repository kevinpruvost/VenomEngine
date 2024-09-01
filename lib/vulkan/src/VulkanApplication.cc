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

#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

#include <venom/common/FpsCounter.h>

namespace venom::vulkan
{
/// @brief Device extensions to use
static constexpr std::array s_deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VulkanApplication::VulkanApplication()
    : vc::GraphicsApplication()
    , DebugApplication()
    , __context()
    , __currentFrame(0)
    , __framebufferChanged(false)
{
    Allocator::SetVKAllocationCallbacks();
}

VulkanApplication::~VulkanApplication()
{
    vc::Log::Print("Destroying Vulkan app...");
    // Set global physical device back to nullptr
    PhysicalDevice::SetUsedPhysicalDevice(nullptr);
#ifdef VENOM_DEBUG
    if (__debugMessenger) {
        auto destroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(Instance::GetVkInstance(), "vkDestroyDebugUtilsMessengerEXT"));
        destroyDebugUtilsMessengerEXT(Instance::GetVkInstance(), __debugMessenger, Allocator::GetVKAllocationCallbacks());
    }
#endif
    vc::Log::Print("Vulkan app succesfully destroyed.");
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
    __shaderPipeline.AddVertexBufferToLayout(sizeof(vc::Vec3), 0, 0, 0);
    __shaderPipeline.AddVertexBufferToLayout(sizeof(vc::Vec3), 1, 1, 0);
    __mesh.AddVertexBuffer(3, sizeof(vc::Vec3), VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
        VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        __verticesPos);
    __mesh.AddVertexBuffer(3, sizeof(vc::Vec3), VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
        VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        __verticesColor);
    __shaderPipeline.LoadShaders(&__swapChain, &__renderPass, {
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
    vc::FpsCounter fps;
    vc::Timer timer;
    while (!__context.ShouldClose())
    {
        __context.PollEvents();
        if (err = __DrawFrame(); err != vc::Error::Success)
            return err;
        fps.RegisterFrame();
        auto duration = timer.GetMilliSeconds();
        if (duration >= 1000) {
            vc::Log::Print("FPS: %d", fps.GetFps());
            timer.Reset();
        }
    }

    vkDeviceWaitIdle(LogicalDevice::GetVkDevice());
    return vc::Error::Success;
}

vc::Error VulkanApplication::__DrawFrame()
{
    // Draw image

    // Wait for the fence to be signaled
    vkWaitForFences(LogicalDevice::GetVkDevice(), 1, __inFlightFences[__currentFrame].GetFence(), VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(LogicalDevice::GetVkDevice(), __swapChain.swapChain, UINT64_MAX, __imageAvailableSemaphores[__currentFrame].GetSemaphore(), VK_NULL_HANDLE, &imageIndex);
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
    vkResetFences(LogicalDevice::GetVkDevice(), 1, __inFlightFences[__currentFrame].GetFence());
    __commandBuffers[__currentFrame]->Reset(0);

    if (auto err = __commandBuffers[__currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __renderPass.BeginRenderPass(&__swapChain, __commandBuffers[__currentFrame], imageIndex);
        __commandBuffers[__currentFrame]->BindPipeline(__shaderPipeline.GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
        __commandBuffers[__currentFrame]->SetViewport(__swapChain.viewport);
        __commandBuffers[__currentFrame]->SetScissor(__swapChain.scissor);
        VkDeviceSize offsets[] = {0, 0};
        vkCmdBindVertexBuffers(__commandBuffers[__currentFrame]->__commandBuffer, 0, 2, __mesh.GetVkVertexBuffers(), offsets);
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

    if (result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, *__inFlightFences[__currentFrame].GetFence()); result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || __framebufferChanged) {
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

    vkQueuePresentKHR(__presentQueue.GetVkQueue(), &presentInfo);

    __currentFrame = (__currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    return vc::Error::Success;
}

vc::Error VulkanApplication::__InitVulkan()
{
    vc::Error res;

    // Debug Initialization first
    DEBUG_CODE(if (res = InitDebug(); res != vc::Error::Success) return res);

    // Create Vulkan instance
    if (res = __CreateInstance(); res != vc::Error::Success) return res;

    // Debug Code for Vulkan Instance
    DEBUG_CODE(if (res = _PostInstance_SetDebugParameters(); res != vc::Error::Success) return res);

    // Get Physical Devices
    if (res = __InitRenderingPipeline(); res != vc::Error::Success) return res;

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

vc::Error VulkanApplication::__InitRenderingPipeline()
{
    vc::Error err;
    std::vector<PhysicalDevice> physicalDevices = PhysicalDevice::GetVulkanPhysicalDevices();

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

    // Set global physical device
    PhysicalDevice::SetUsedPhysicalDevice(&__physicalDevice.physicalDevice);

    // Get Queue Families
    __queueFamilies = getVulkanQueueFamilies(__physicalDevice);

    // Create Surface
    __surface.CreateSurface(&__context);

    // Check if the device supports the surface for presentation and which queue family supports it
    if (err = __queueFamilies.SetPresentQueueFamilyIndices(__physicalDevice, __surface); err != vc::Error::Success)
        return err;

    // Create Logical device
    VkDeviceCreateInfo createInfo{};

    // Queue Create Infos
    // Create Queue Create Infos
    QueueManagerSettings settings {
        .graphicsQueueCount = 1,
        .graphicsQueuePriority = 1.0f,
        .computeQueueCount = 1,
        .computeQueuePriority = 0.9f,
        .transferQueueCount = 1,
        .transferQueuePriority = 0.7f,
        .sparseBindingQueueCount = 1,
        .sparseBindingQueuePriority = 0.6f,
        .protectedQueueCount = 0,
        .protectedQueuePriority = 0.0f,
        .videoDecodeQueueCount = 1,
        .videoDecodeQueuePriority = 0.5f,
        .videoEncodeQueueCount = 1,
        .videoEncodeQueuePriority = 0.5f,
        .presentQueueCount = 1,
        .presentQueuePriority = 1.0f
    };
    __queueManager.SetQueueManagerSettings(settings);
    if (err = __queueManager.SetLogicalDeviceQueueCreateInfos(__queueFamilies, &createInfo); err != vc::Error::Success)
        return err;

    VkPhysicalDeviceFeatures deviceFeatures{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
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

    // Create Logical Device
    if (err = __logicalDevice.Init(&createInfo); err != vc::Error::Success)
        return err;

    // Create SwapChain
    if (err = __swapChain.InitSwapChain(&__surface, &__context, &__queueFamilies); err != vc::Error::Success)
        return err;

    // Init Command Pool Manager (inits 1 pool per queue family)
    if (err = __commandPoolManager.Init(__queueFamilies); err != vc::Error::Success)
        return err;

    // Init Queue Manager
    if (err = __queueManager.Init(); err != vc::Error::Success)
        return err;

    // Get Graphics Queue
    __graphicsQueue = QueueManager::GetGraphicsQueue();

    // Get Present Queue
    __presentQueue = QueueManager::GetPresentQueue();

    // Create Render Pass
    if (err = __renderPass.InitRenderPass(&__swapChain); err != vc::Error::Success)
        return err;

    // Init Render Pass Framebuffers
    if (err = __swapChain.InitSwapChainFramebuffers(&__renderPass); err != vc::Error::Success)
        return err;

    // Create Graphics Command Pool
    CommandPool * graphicsCommandPool = __commandPoolManager.GetGraphicsCommandPool();

    // Create Command Buffers
    __commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = graphicsCommandPool->CreateCommandBuffer(&__commandBuffers[i]); err != vc::Error::Success)
            return err;
    }

    // Create Semaphores & Fences
    __imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    __renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    __inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __imageAvailableSemaphores[i].InitSemaphore(); err != vc::Error::Success)
            return err;
        if (err =__renderFinishedSemaphores[i].InitSemaphore(); err != vc::Error::Success)
            return err;
        if (err =__inFlightFences[i].InitFence(VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT); err != vc::Error::Success)
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
    vc::Error err;
#ifdef VENOM_DEBUG
    err = Instance::CreateInstance(&__instance, this);
#else
    err = Instance::CreateInstance(&__instance);
#endif
    if (err != vc::Error::Success) {
        vc::Log::Error("Failed to create Vulkan instance: %d", err);
        return vc::Error::InitializationFailed;
    }
    return vc::Error::Success;
}

void VulkanApplication::__RecreateSwapChain()
{
    vkDeviceWaitIdle(LogicalDevice::GetVkDevice());
    __swapChain.InitSwapChainSettings(&__physicalDevice, &__surface, &__context);
    __swapChain.InitSwapChain(&__surface, &__context, &__queueFamilies);
    __swapChain.InitSwapChainFramebuffers(&__renderPass);

    // We also need to reset the last used semaphore
    __imageAvailableSemaphores[__currentFrame].InitSemaphore();
}
}
