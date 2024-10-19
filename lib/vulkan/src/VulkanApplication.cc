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

#include <venom/vulkan/plugin/graphics/Texture.h>

#include <venom/common/VenomSettings.h>

namespace venom::vulkan
{
/// @brief Device extensions to use
static constexpr std::array s_deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef __APPLE__ // Maybe Linux ?
    "VK_KHR_portability_subset",
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
#endif
};

int VulkanApplication::__currentFrame = 0;

VulkanApplication::VulkanApplication()
    : vc::GraphicsApplication()
    , DebugApplication()
    , __context()
    , __framebufferChanged(false)
    , __shouldClose(false)
{
    __currentFrame = 0;
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

vc::Error VulkanApplication::Init()
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

    __texture = vc::Texture::Create("random.png");
    __camera = vc::Camera::Create()->As<VulkanCamera>();
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        // Separate Sampled Image & Sampler
        __shaderPipeline.GetDescriptorSets(2)[i].UpdateTexture(reinterpret_cast<VulkanTexture*>(__texture), 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, 0);
        __shaderPipeline.GetDescriptorSets(3)[i].UpdateSampler(__sampler, 0, VK_DESCRIPTOR_TYPE_SAMPLER, 1, 0);
    }
    return vc::Error::Success;
}

vc::Error VulkanApplication::Loop()
{
    return __Loop();
}

bool VulkanApplication::ShouldClose() { return __shouldClose; }

int VulkanApplication::GetCurrentFrame()
{
    return __currentFrame;
}

vc::Error VulkanApplication::__Loop()
{
    vc::Error err;
    static vc::FpsCounter fps;
    static vc::Timer timer;

    vc::Timer pollP;
    __context.PollEvents();
    //vc::Log::Print("Poll time: %lu", pollP.GetMicroSeconds());
    if (err = __DrawFrame(); err != vc::Error::Success)
        return err;
    fps.RegisterFrame();
    auto duration = timer.GetMilliSeconds();
    if (duration >= 1000) {
        int fpsCount = fps.GetFps();
            vc::Log::Print("FPS: %u, Theoretical FPS: %.2f", fpsCount, _GetTheoreticalFPS(fpsCount));
        timer.Reset();
    }
    __shouldClose = __context.ShouldClose();
    if (__shouldClose) {
        vkDeviceWaitIdle(LogicalDevice::GetVkDevice());
    }
    return err;
}

void VulkanApplication::__UpdateUniformBuffers()
{
    static vc::Timer timer_uni;
    float time = timer_uni.GetMilliSeconds();

    vcm::Mat4 models[VENOM_MAX_ENTITIES];
    models[0] = models[1] = vcm::Identity();
    vcm::RotateMatrix(models[0], {0.0f, 0.0f, 1.0f}, 1.0f);
    vcm::RotateMatrix(models[0], {0.0f, 1.0f, 0.0f}, time / 1000.0f);

    // Camera
    vcm::Mat4 viewAndProj[2];
    vcm::Vec3 cameraPos = {2.0f, 2.0f, 1.0f};
    __camera->SetPosition(cameraPos);
    __camera->LookAt({0,0,0});
    viewAndProj[0] = __camera->GetViewMatrix();
    viewAndProj[1] = __camera->GetProjectionMatrix();

    // Uniform buffers (view and projection)
    memcpy(__objectStorageBuffers[__currentFrame].GetMappedData(), models, sizeof(models));
    memcpy(__cameraUniformBuffers[__currentFrame].GetMappedData(), viewAndProj, sizeof(viewAndProj));
    // Push Constants (model)
    // __commandBuffers[__currentFrame]->PushConstants(&__shaderPipeline, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(vcm::Mat4), &model);
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

        // Update Uniform Buffers
        __UpdateUniformBuffers();

        __renderPass.BeginRenderPass(&__swapChain, __commandBuffers[__currentFrame], imageIndex);
        __commandBuffers[__currentFrame]->BindPipeline(__shaderPipeline.GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
        __commandBuffers[__currentFrame]->SetViewport(__swapChain.viewport);
        __commandBuffers[__currentFrame]->SetScissor(__swapChain.scissor);
        //__descriptorSets[__currentFrame].UpdateTexture(reinterpret_cast<const VulkanTexture*>(__texture), 2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, 0);
        //__commandBuffers[__currentFrame]->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, __shaderPipeline.GetPipelineLayout(), 0, 1, __descriptorSets[__currentFrame].GetVkDescriptorSet());
        __shaderPipeline.BindDescriptorSets(*__commandBuffers[__currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS);
        __commandBuffers[__currentFrame]->DrawMesh(__mesh, 0);
        // TODO: Create Descriptor Set for each mesh
        __commandBuffers[__currentFrame]->DrawModel(__model, 1);
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

    vc::Timer theoreticalFpsCounter;
    if (result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, *__inFlightFences[__currentFrame].GetFence()); result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || __framebufferChanged) {
        __framebufferChanged = false;
        __RecreateSwapChain();
        return vc::Error::Success;
    } else if (result != VK_SUCCESS) {
        vc::Log::Error("Failed to submit draw command buffer");
        return vc::Error::Failure;
    }
    _UpdateTheoreticalFPS(theoreticalFpsCounter.GetMicroSeconds());

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {__swapChain.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(__presentQueue.GetVkQueue(), &presentInfo);

    __currentFrame = (__currentFrame + 1) % VENOM_MAX_FRAMES_IN_FLIGHT;
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
        DEBUG_LOG("-%s:", physicalDevices[i].GetProperties().deviceName);
        DEBUG_LOG("\tType: %s", physicalDevices[i].GetProperties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Discrete" : physicalDevices[i].GetProperties().deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "Integrated" : "Other Type of GPU");
        DEBUG_LOG("\tAPI Version: %u.%u.%u", VK_VERSION_MAJOR(physicalDevices[i].GetProperties().apiVersion), VK_VERSION_MINOR(physicalDevices[i].GetProperties().apiVersion), VK_VERSION_PATCH(physicalDevices[i].GetProperties().apiVersion));
        DEBUG_LOG("\tDriver Version: %u", physicalDevices[i].GetProperties().driverVersion);
        DEBUG_LOG("\tVendor ID: %u", physicalDevices[i].GetProperties().vendorID);
        DEBUG_LOG("\tDevice ID: %u", physicalDevices[i].GetProperties().deviceID);
        DEBUG_LOG("\tGeometry Shader: %s", physicalDevices[i].GetFeatures().geometryShader ? "Yes" : "No");
        DEBUG_LOG("\tTesselation Shader: %s", physicalDevices[i].GetFeatures().tessellationShader ? "Yes" : "No");
        for (int j = 0; j < physicalDevices[i].GetMemoryProperties().memoryHeapCount; ++j) {
            DEBUG_LOG("\tHeap %d: %luMB", j, physicalDevices[i].GetMemoryProperties().memoryHeaps[j].size / (1024 * 1024));
        }
        // Select GPU
        if (physicalDevices[i].GetFeatures().tessellationShader) {
         //   int vram1 = physicalDevices[i].GetDeviceLocalVRAMAmount();
       //     int vram2 = PhysicalDevice::GetUsedPhysicalDevice().GetDeviceLocalVRAMAmount();
            if (__physicalDevice.GetVkPhysicalDevice() == VK_NULL_HANDLE || __physicalDevice.GetDeviceLocalVRAMAmount() < physicalDevices[i].GetDeviceLocalVRAMAmount()) {
                __physicalDevice = physicalDevices[i];
            }
        }
    }
    DEBUG_LOG("Chosen phyiscal device:");
    DEBUG_LOG("-%s:", __physicalDevice.GetProperties().deviceName);
    DEBUG_LOG("Device Local VRAM: %luMB", __physicalDevice.GetDeviceLocalVRAMAmount() / (1024 * 1024));

    // Set global physical device
    PhysicalDevice::SetUsedPhysicalDevice(&__physicalDevice);

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

    // Chose features
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    //deviceFeatures.textureCompressionBC = VK_TRUE;

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

    // Init Command Pool Manager (inits 1 pool per queue family)
    if (err = __commandPoolManager.Init(); err != vc::Error::Success)
        return err;

    // Init Queue Manager
    if (err = __queueManager.Init(); err != vc::Error::Success)
        return err;

    // Create SwapChain
    if (err = __swapChain.InitSwapChain(&__surface, &__context, &__queueFamilies); err != vc::Error::Success)
        return err;

    // Get Graphics Queue
    __graphicsQueue = QueueManager::GetGraphicsQueue();

    // Get Present Queue
    __presentQueue = QueueManager::GetPresentQueue();

    // Create Render Pass
    if (err = __renderPass.InitRenderPass(&__swapChain); err != vc::Error::Success)
        return err;

    // Create Graphics Command Pool
    CommandPool * graphicsCommandPool = __commandPoolManager.GetGraphicsCommandPool();

    // Create Command Buffers
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = graphicsCommandPool->CreateCommandBuffer(&__commandBuffers[i]); err != vc::Error::Success)
            return err;
    }

    // Create Semaphores & Fences
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __imageAvailableSemaphores[i].InitSemaphore(); err != vc::Error::Success)
            return err;
        if (err =__renderFinishedSemaphores[i].InitSemaphore(); err != vc::Error::Success)
            return err;
        if (err =__inFlightFences[i].InitFence(VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT); err != vc::Error::Success)
            return err;
    }

    // Create Uniform Buffers
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __objectStorageBuffers[i].Init(VENOM_MAX_ENTITIES * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
        if (err = __cameraUniformBuffers[i].Init(2 * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
    }

    // Init Render Pass Framebuffers
    if (err = __swapChain.InitSwapChainFramebuffers(&__renderPass); err != vc::Error::Success)
        return err;

    // VertexBuffer Layout
    /// Position
    __shaderPipeline.AddVertexBufferToLayout(sizeof(vcm::Vec3), 0, 0, 0, VK_FORMAT_R32G32B32_SFLOAT);
    /// Normal
    __shaderPipeline.AddVertexBufferToLayout(sizeof(vcm::Vec3), 1, 1, 0, VK_FORMAT_R32G32B32_SFLOAT);
    /// Color
    __shaderPipeline.AddVertexBufferToLayout(sizeof(vcm::Vec4), 2, 2, 0, VK_FORMAT_R32G32B32A32_SFLOAT);
    /// UV
    __shaderPipeline.AddVertexBufferToLayout(sizeof(vcm::Vec2), 3, 3, 0, VK_FORMAT_R32G32_SFLOAT);

    // Descriptor Set Layout
    __shaderPipeline.AddDescriptorSetLayoutBinding(0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
    __shaderPipeline.AddDescriptorSetLayoutBinding(1, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
    __shaderPipeline.AddDescriptorSetLayoutBinding(2, 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    __shaderPipeline.AddDescriptorSetLayoutBinding(3, 0, VK_DESCRIPTOR_TYPE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    __model = reinterpret_cast<VulkanModel*>(vc::Model::Create("eye/eye.obj"));
    __mesh = reinterpret_cast<VulkanMesh*>(vc::Mesh::Create());
    __mesh->AddVertexBuffer(__verticesPos, sizeof(__verticesPos) / sizeof(vcm::Vec3), sizeof(vcm::Vec3), 0);
    __mesh->AddVertexBuffer(__verticesPos, sizeof(__verticesPos) / sizeof(vcm::Vec3), sizeof(vcm::Vec3), 1);
    __mesh->AddVertexBuffer(__verticesColor, sizeof(__verticesColor) / sizeof(vcm::Vec4), sizeof(vcm::Vec4), 2);
    __mesh->AddVertexBuffer(__verticesUV, sizeof(__verticesUV) / sizeof(vcm::Vec2), sizeof(vcm::Vec2), 3);
    __mesh->AddIndexBuffer(__indices, sizeof(__indices) / sizeof(uint32_t), sizeof(uint32_t));
    __shaderPipeline.LoadShaders(&__swapChain, &__renderPass, {
        "shader_mesh.ps",
        "shader_mesh.vs"
    });

    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        // Model & PBR info
        __shaderPipeline.GetDescriptorSets(0)[i].UpdateBuffer(__objectStorageBuffers[i], 0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
        // View & Projection
        __shaderPipeline.GetDescriptorSets(1)[i].UpdateBuffer(__cameraUniformBuffers[i], 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);
    }

    // Create Sampler
    __sampler.SetCreateInfo({
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .mipLodBias = 0.0f,
        .anisotropyEnable = VK_TRUE,
        .maxAnisotropy = PhysicalDevice::GetUsedPhysicalDevice().GetProperties().limits.maxSamplerAnisotropy,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .minLod = 0.0f,
        .maxLod = 0.0f,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE
    });
    if (err = __sampler.Create(); err != vc::Error::Success)
        return err;

    return vc::Error::Success;
}

bool VulkanApplication::__IsDeviceSuitable(const VkDeviceCreateInfo * createInfo)
{
    // Check if the device supports the extensions
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(__physicalDevice.GetVkPhysicalDevice(), nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(__physicalDevice.GetVkPhysicalDevice(), nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(createInfo->ppEnabledExtensionNames, createInfo->ppEnabledExtensionNames + createInfo->enabledExtensionCount);
    for (const auto& extension : availableExtensions) {
#ifdef VENOM_DEBUG
        vc::Log::LogToFile("Available extension: %s", extension.extensionName);
#endif
        requiredExtensions.erase(extension.extensionName);
    }
    if (!requiredExtensions.empty()) {
        vc::Log::Error("Missing required extensions:");
        for (const auto& extension : requiredExtensions) {
            vc::Log::Error("\t%s", extension.c_str());
        }
        return false;
    }

    // Check if the device's swap chain is ok
    if (__swapChain.presentModes.empty() || __swapChain.surfaceFormats.empty()) {
        vc::Log::Error("Failed to get surface formats or present modes for swap chain");
        return false;
    }

    // Check if the device supports anisotropy
    if (createInfo->pEnabledFeatures->samplerAnisotropy != VK_TRUE) {
        vc::Log::Error("Device does not support anisotropy");
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
