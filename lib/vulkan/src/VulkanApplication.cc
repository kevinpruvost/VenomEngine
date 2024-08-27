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
    for (int i = 0; i < 2; ++i) {
        vkDestroyBuffer(LogicalDevice::GetVkDevice(), __vertexBuffers[i], Allocator::GetVKAllocationCallbacks());
    }
    for (int i = 0; i < 2; ++i) {
        vkFreeMemory(LogicalDevice::GetVkDevice(), __vertexBuffersMemory[i], Allocator::GetVKAllocationCallbacks());
    }
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
    __shaderPipeline.AddVertexBufferToLayout(3, sizeof(vc::Vec3),
        VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
        VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        __verticesPos, 0, 0, 0);
    __shaderPipeline.AddVertexBufferToLayout(3, sizeof(vc::Vec3),
        VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
        VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        __verticesColor, 1, 1, 0);
    __shaderPipeline.LoadShaders(&__swapChain, &__renderPass, {
        "shader.ps",
        "shader.vs"
    });

    __vertexBuffers.resize(2);
    __vertexBuffersMemory.resize(2);
    for (int i = 0; i < 2; ++i) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(vc::Vec3) * 3;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(LogicalDevice::GetVkDevice(), &bufferInfo, Allocator::GetVKAllocationCallbacks(), &__vertexBuffers[i]) != VK_SUCCESS) {
            vc::Log::Error("Failed to create vertex buffer");
            return vc::Error::Failure;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(LogicalDevice::GetVkDevice(), __vertexBuffers[i], &memRequirements);

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(__physicalDevice.physicalDevice, &memProperties);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        auto findMemoryType = [&](uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t {
            for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }
            return -1;
        };
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(LogicalDevice::GetVkDevice(), &allocInfo, Allocator::GetVKAllocationCallbacks(), &__vertexBuffersMemory[i]) != VK_SUCCESS) {
            vc::Log::Error("Failed to allocate vertex buffer memory");
            return vc::Error::Failure;
        }
    }

    vkBindBufferMemory(LogicalDevice::GetVkDevice(), __vertexBuffers[0], __vertexBuffersMemory[0], 0);

    void* data;
    vkMapMemory(LogicalDevice::GetVkDevice(), __vertexBuffersMemory[0], 0, sizeof(vc::Vec3) * 3, 0, &data);
    memcpy(data, __verticesPos, sizeof(vc::Vec3) * 3);
    vkUnmapMemory(LogicalDevice::GetVkDevice(), __vertexBuffersMemory[0]);

    vkBindBufferMemory(LogicalDevice::GetVkDevice(), __vertexBuffers[1], __vertexBuffersMemory[1], 0);

    vkMapMemory(LogicalDevice::GetVkDevice(), __vertexBuffersMemory[1], 0, sizeof(vc::Vec3) * 3, 0, &data);
    memcpy(data, __verticesColor, sizeof(vc::Vec3) * 3);
    vkUnmapMemory(LogicalDevice::GetVkDevice(), __vertexBuffersMemory[1]);

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

    vkDeviceWaitIdle(LogicalDevice::GetVkDevice());
    return vc::Error::Success;
}

vc::Error VulkanApplication::__DrawFrame()
{
    // Draw image
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
        vkCmdBindVertexBuffers(__commandBuffers[__currentFrame]->__commandBuffer, 0, 2, __vertexBuffers.data(), offsets);
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
    // Create Queue Create Infos
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    float queuePriority = 1.0f;
    // Graphics Queue
    queueCreateInfos.emplace_back(
        VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = __queueFamilies.graphicsQueueFamilyIndices[0],
            .queueCount = 1,
            .pQueuePriorities = &queuePriority
        }
    );

    // Present Queue
    if (queueCreateInfos[0].queueFamilyIndex != __queueFamilies.presentQueueFamilyIndices[0]) {
        queueCreateInfos.emplace_back(
            VkDeviceQueueCreateInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = __queueFamilies.presentQueueFamilyIndices[0],
                .queueCount = 1,
                .pQueuePriorities = &queuePriority
            }
        );
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

    if (VkResult res = vkCreateDevice(__physicalDevice.physicalDevice, &createInfo, Allocator::GetVKAllocationCallbacks(), &__logicalDevice.__device); res != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create logical device, error code: %d", res);
        return vc::Error::InitializationFailed;
    }
    LogicalDevice::CreateInstance(&__logicalDevice);

    // Create SwapChain
    if (err = __swapChain.InitSwapChain(&__surface, &__context, &__queueFamilies); err != vc::Error::Success)
        return err;

    // Create Graphics Queue
    __physicalDevice.GetDeviceQueue(&__graphicsQueue, __queueFamilies.graphicsQueueFamilyIndices[0], 0);

    // Create Present Queue
    __physicalDevice.GetDeviceQueue(&__presentQueue, __queueFamilies.presentQueueFamilyIndices[0], 0);

    // Create Render Pass
    if (err = __renderPass.InitRenderPass(&__swapChain); err != vc::Error::Success)
        return err;

    // Init Render Pass Framebuffers
    if (err = __swapChain.InitSwapChainFramebuffers(&__renderPass); err != vc::Error::Success)
        return err;

    // Create Graphics Command Pool
    if (err = __commandPool.InitCommandPool(__queueFamilies.graphicsQueueFamilyIndices[0]); err != vc::Error::Success)
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
