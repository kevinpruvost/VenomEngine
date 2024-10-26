///
/// Project: VenomEngineWorkspace
/// @file VulkanApplicationInitializer.cc
/// @date Oct, 22 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>

namespace venom
{
namespace vulkan
{
/// @brief Device extensions to use
static constexpr std::array s_deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_EXT_ROBUSTNESS_2_EXTENSION_NAME,
    VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
    VK_KHR_MAINTENANCE3_EXTENSION_NAME,
#ifdef __APPLE__ // Maybe Linux ?
    "VK_KHR_portability_subset",
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
#endif
};

vc::Error VulkanApplication::__Init()
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
    return vc::Error::Success;
}

vc::Error VulkanApplication::__PostInit()
{
    // Separate Sampled Image & Sampler
  //  __texture.reset(new vc::Texture("random.png"));
    for (int i = 0; i < VENOM_MAX_DYNAMIC_TEXTURES; ++i) {
        DescriptorPool::GetPool()->GetDescriptorSets(2).GroupUpdateTexture(_dummyTexture->GetImpl()->As<VulkanTexture>(), 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, i);
//        DescriptorPool::GetPool()->GetDescriptorSets(2).GroupUpdateTexture(__texture->GetImpl()->As<VulkanTexture>(), 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, i);
    }
    DescriptorPool::GetPool()->GetDescriptorSets(3).GroupUpdateSampler(__sampler, 0, VK_DESCRIPTOR_TYPE_SAMPLER, 1, 0);
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

VkPhysicalDeviceFeatures2 VulkanApplication::__GetPhysicalDeviceFeatures(bool & supported, VkPhysicalDeviceDescriptorIndexingFeatures & descriptorIndexingFeatures, VkPhysicalDeviceFeatures2 & features)
{
    // Features
    features.features.samplerAnisotropy = VK_TRUE;

    // Null Descriptor (enable if possible), don't try, it gives errors for some reason
    // VkPhysicalDeviceRobustness2FeaturesEXT robustness2Features{};
    // robustness2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;
    // robustness2Features.nullDescriptor = VK_TRUE;  // Enable null descriptor feature
    // features.pNext = &robustness2Features;

    // Query physical device properties and features
    vkGetPhysicalDeviceFeatures2(__physicalDevice.GetVkPhysicalDevice(), &features);

    // DEBUG_PRINT("Robustness2: NullDescriptor: %s be enabled!", robustness2Features.nullDescriptor ? "Can" : "Cannot");

    // Check if the device supports anisotropy
    if (features.features.samplerAnisotropy != VK_TRUE) {
        vc::Log::Error("Device does not support anisotropy");
        supported = false;
    }

#ifdef VENOM_BINDLESS_TEXTURES
    if (vc::ShaderResourceTable::UsingBindlessTextures()) {
        supported = __bindlessSupported = descriptorIndexingFeatures.descriptorBindingPartiallyBound && descriptorIndexingFeatures.runtimeDescriptorArray;
        if (!supported) {
            vc::Log::Error("Device does not support bindless textures");
            return features;
        }
    }
#endif

    return features;
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

#ifdef VENOM_BINDLESS_TEXTURES
    // Set max textures
    vc::ShaderResourceTable::SetMaxTextures(__physicalDevice.GetProperties().limits.maxPerStageDescriptorSampledImages);
#endif

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
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

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

    // Extensions
    createInfo.enabledExtensionCount = s_deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = s_deviceExtensions.data();

    // All Features
    bool physicalDeviceFeaturesSupported = true;
    VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT, nullptr };
    VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = &descriptorIndexingFeatures
    };
    __GetPhysicalDeviceFeatures(physicalDeviceFeaturesSupported, descriptorIndexingFeatures, physicalDeviceFeatures2);
    createInfo.pNext = &physicalDeviceFeatures2;
    if (!physicalDeviceFeaturesSupported) {
        vc::Log::Error("Physical device does not support all required physical features");
        return vc::Error::InitializationFailed;
    }

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
    //__shaderPipeline.AddVertexBufferToLayout(sizeof(vcm::Vec4), 2, 2, 0, VK_FORMAT_R32G32B32A32_SFLOAT);
    /// UV
    __shaderPipeline.AddVertexBufferToLayout(sizeof(vcm::Vec2), 3, 3, 0, VK_FORMAT_R32G32_SFLOAT);

    // Descriptor Set Layout

    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(1, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
#ifdef VENOM_BINDLESS_TEXTURES
    if (__bindlessSupported) {
        DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(2, 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, vc::ShaderResourceTable::GetMaxTextures(), VK_SHADER_STAGE_FRAGMENT_BIT);
        DescriptorPool::GetPool()->SetDescriptorSetLayoutBindless(2);
        // Using uniform buffer dynamic for texture IDs (4.1)
        DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(4, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    }
    else
#endif
    {
        DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(2, 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_MAX_DYNAMIC_TEXTURES, VK_SHADER_STAGE_FRAGMENT_BIT);
        DescriptorPool::GetPool()->SetDescriptorSetLayoutMaxSets(2, VENOM_MAX_ENTITIES);
    }
    // Enabling update after bind pool for textures, dynamic or bindless
    DescriptorPool::GetPool()->SetDescriptorSetLayoutCreateFlags(2, VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT);
    // Sampler
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(3, 0, VK_DESCRIPTOR_TYPE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    // Material properties (4.0)
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(4, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    DescriptorPool::GetPool()->SetDescriptorSetLayoutMaxSets(4, VENOM_MAX_ENTITIES);

    // Makes the pool able to allocate descriptor sets that can be updated after binding
    if (DescriptorPool::GetPool()->Create(VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT) != vc::Error::Success)
        return vc::Error::Failure;
    __model.ImportModel("eye/eye.obj");
    __shaderPipeline.LoadShaders(&__swapChain, &__renderPass, {
        "shader_mesh.ps",
        "shader_mesh.vs"
    });

    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        // Model & PBR info
        DescriptorPool::GetPool()->GetDescriptorSets(0).GroupUpdateBuffer(__objectStorageBuffers[i], 0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
        // View & Projection
        DescriptorPool::GetPool()->GetDescriptorSets(1).GroupUpdateBuffer(__cameraUniformBuffers[i], 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);
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
    __imageAvailableSemaphores[_currentFrame].InitSemaphore();
}
}
}