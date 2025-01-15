///
/// Project: VenomEngineWorkspace
/// @file VulkanApplicationInitializer.cc
/// @date Oct, 22 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>
#include <venom/common/plugin/graphics/GUI.h>

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
    VK_EXT_HDR_METADATA_EXTENSION_NAME,
#ifdef __APPLE__ // Maybe Linux ?
    "VK_KHR_portability_subset",
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
#endif
};

vc::Error VulkanApplication::__Init()
{
    vc::Error res;

    printf("Hello, Vulkan!\n");

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

    supported = descriptorIndexingFeatures.descriptorBindingPartiallyBound && descriptorIndexingFeatures.runtimeDescriptorArray;
    if (!supported) {
        vc::Log::Error("Device does not support bindless textures");
        return features;
    }

    return features;
}

void VulkanApplication::__SetGLFWCallbacks()
{
    glfwSetWindowUserPointer(vc::Context::Get()->GetWindow(), this);
    glfwSetFramebufferSizeCallback(vc::Context::Get()->GetWindow(), [](GLFWwindow * window, int width, int height) {
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

    // Set max textures
    vc::ShaderResourceTable::SetMaxTextures(__physicalDevice.GetProperties().limits.maxPerStageDescriptorSampledImages);

    // Set global physical device
    PhysicalDevice::SetUsedPhysicalDevice(&__physicalDevice);

    // Get Queue Families
    __queueFamilies = getVulkanQueueFamilies(__physicalDevice);

    // Create Surface
    __surface.CreateSurface(vc::Context::Get());

    // Check if HDR is possible
    for (const auto& format : __surface.GetSurfaceFormats()) {
        if (format.format == VK_FORMAT_R16G16B16A16_SFLOAT) {
            if (format.colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT) {
                _isHdrSupported = true;
                //SetHDR(true);
                break;
            }
        }
    }

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
    // Only with VK_ENABLE_BETA_EXTENSIONS
    //VkPhysicalDevicePortabilitySubsetFeaturesKHR portabilityFeatures{};
    //portabilityFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR;
    //descriptorIndexingFeatures.pNext = &portabilityFeatures;
    __GetPhysicalDeviceFeatures(physicalDeviceFeaturesSupported, descriptorIndexingFeatures, physicalDeviceFeatures2);
    createInfo.pNext = &physicalDeviceFeatures2;
    if (!physicalDeviceFeaturesSupported) {
        vc::Log::Error("Physical device does not support all required physical features");
        return vc::Error::InitializationFailed;
    }

    // Validation Layers
    _SetCreateInfoValidationLayers(&createInfo);

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

    // Create Swap Chain
    if (err = __swapChain.InitSwapChainSettings(&__surface); err != vc::Error::Success)
        return err;

    // Create SwapChain
    __swapChain.SetSamples(GetActiveSamplesMultisampling());
    if (err = __swapChain.InitSwapChain(); err != vc::Error::Success)
        return err;
    _currentExtent = {__swapChain.extent.width, __swapChain.extent.height};

    // Get Graphics Queue
    __graphicsQueue = QueueManager::GetGraphicsQueue();

    // Get Present Queue
    __presentQueue = QueueManager::GetPresentQueue();

    // Init descriptor sets parameters
    if (err = __InitializeSets(); err != vc::Error::Success)
        return err;

    // Create Render Pass
    __skyboxRenderPass.SetRenderingType(vc::RenderingPipelineType::Skybox);
    __graphicsRenderPass.SetRenderingType(vc::RenderingPipelineType::PBRModel);
    __shadowMapRenderPass.SetRenderingType(vc::RenderingPipelineType::CascadedShadowMapping);
    __guiRenderPass.SetRenderingType(vc::RenderingPipelineType::GUI);
    __CreateAttachments();
    for (const auto renderPass : RenderPass::GetRenderPasses()) {
        if (renderPass == nullptr) continue;
        if (err = renderPass->InitRenderPass(&__swapChain); err != vc::Error::Success)
            return err;
    }

    // Create Graphics Command Pool
    CommandPool * graphicsCommandPool = __commandPoolManager.GetGraphicsCommandPool();
    CommandPool * computeCommandPool = __commandPoolManager.GetComputeCommandPool();

    // Create Command Buffers
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = graphicsCommandPool->CreateCommandBuffer(&__graphicsFirstCheckpointCommandBuffers[i]); err != vc::Error::Success)
            return err;
        if (err = graphicsCommandPool->CreateCommandBuffer(&__graphicsSceneCheckpointCommandBuffers[i]); err != vc::Error::Success)
            return err;
        if (err = computeCommandPool->CreateCommandBuffer(&__computeCommandBuffers[i]); err != vc::Error::Success)
            return err;
        for (int j = 0; j < std::size(__shadowMapDirectionalCommandBuffers[i]); ++j) {
            for (int k = 0; k < std::size(__shadowMapDirectionalCommandBuffers[i][j]); ++k) {
                if (err = computeCommandPool->CreateCommandBuffer(&__shadowMapDirectionalCommandBuffers[i][j][k]); err != vc::Error::Success)
                    return err;
            }
        }
        for (int j = 0; j < std::size(__shadowMapPointCommandBuffers[i]); ++j) {
            for (int k = 0; k < std::size(__shadowMapPointCommandBuffers[i][j]); ++k) {
                if (err = computeCommandPool->CreateCommandBuffer(&__shadowMapPointCommandBuffers[i][j][k]); err != vc::Error::Success)
                    return err;
            }
        }
        for (int j = 0; j < std::size(__shadowMapSpotCommandBuffers[i]); ++j) {
            if (err = computeCommandPool->CreateCommandBuffer(&__shadowMapSpotCommandBuffers[i][j]); err != vc::Error::Success)
                return err;
        }
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
        .maxLod = 10.0f,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE
    });
    if (err = __sampler.Create(); err != vc::Error::Success)
        return err;
    __sampler.SetAsMainSampler();
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
    if (__surface.GetPresentModes().empty() || __surface.GetSurfaceFormats().empty()) {
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

void VulkanApplication::__CreateAttachments()
{
    AttachmentsManager::Get()->attachments.clear();
    AttachmentsManager::Get()->attachments.resize(VENOM_MAX_FRAMES_IN_FLIGHT);
    AttachmentsManager::Get()->resolveAttachments.clear();
    AttachmentsManager::Get()->resolveAttachments.resize(VENOM_MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        AttachmentsManager::Get()->attachments[i].reserve(static_cast<size_t>(vc::ColorAttachmentType::Count));
        AttachmentsManager::Get()->resolveAttachments[i].reserve(static_cast<size_t>(vc::ColorAttachmentType::Count));
        for (int j = 0; j < static_cast<int>(vc::ColorAttachmentType::Count); ++j) {
            AttachmentsManager::Get()->attachments[i].emplace_back().GetImpl()->As<VulkanTexture>()->GetImage().SetSamples(__swapChain.GetSamples());
            AttachmentsManager::Get()->attachments[i][j].CreateAttachment(__swapChain.extent.width, __swapChain.extent.height, 1, vc::ShaderVertexFormat::Vec4);
            AttachmentsManager::Get()->resolveAttachments[i].emplace_back().CreateAttachment(__swapChain.extent.width, __swapChain.extent.height, 1, vc::ShaderVertexFormat::Vec4);
        }
    }
}

vc::Error VulkanApplication::__RecreateSwapChain()
{
    vc::Error err;
    vkDeviceWaitIdle(LogicalDevice::GetVkDevice());
    __swapChain.CleanSwapChain();
    // Create Surface
    __surface.CreateSurface(vc::Context::Get());
    if (err = __swapChain.InitSwapChainSettings(&__surface); err != vc::Error::Success)
        return err;
    if (err = __swapChain.InitSwapChain(); err != vc::Error::Success)
        return err;
    _currentExtent = {__swapChain.extent.width, __swapChain.extent.height};
    // ReCreate Attachments and Render Pass
    __CreateAttachments();
    for (const auto renderPass : RenderPass::GetRenderPasses()) {
        if (renderPass && renderPass->InitRenderPass(&__swapChain) != vc::Error::Success)
            return vc::Error::InitializationFailed;
    }
    // We also need to reset the last used semaphore
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        __imageAvailableSemaphores[i].InitSemaphore();
        __renderFinishedSemaphores[i].InitSemaphore();
        __graphicsSkyboxDoneSemaphores[i].InitSemaphore();
        __computeShadersFinishedSemaphores[i].InitSemaphore();
        for (int j = 0; j < std::size(__shadowMapsDirectionalFinishedSemaphores[i]); ++j) {
            for (int k = 0; k < std::size(__shadowMapsDirectionalFinishedSemaphores[i][j]); ++k) {
                __shadowMapsDirectionalFinishedSemaphores[i][j][k].InitSemaphore();
            }
        }
        for (int j = 0; j < std::size(__shadowMapsPointFinishedSemaphores[i]); ++j) {
            for (int k = 0; k < std::size(__shadowMapsPointFinishedSemaphores[i][j]); ++k) {
                __shadowMapsPointFinishedSemaphores[i][j][k].InitSemaphore();
            }
        }
        for (int j = 0; j < std::size(__shadowMapsSpotFinishedSemaphores[i]); ++j) {
            __shadowMapsSpotFinishedSemaphores[i][j].InitSemaphore();
        }
    }
    // GUI
    if (err = vc::GUI::Get()->Reset(); err != vc::Error::Success)
        return err;

    return vc::Error::Success;
}
}
}
