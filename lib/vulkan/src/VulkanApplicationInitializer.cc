///
/// Project: VenomEngineWorkspace
/// @file VulkanApplicationInitializer.cc
/// @date Oct, 22 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>
#include <venom/common/plugin/graphics/GUI.h>
#if defined(VENOM_PLATFORM_APPLE)
#include <vulkan/vulkan_metal.h>
#endif

namespace venom
{
namespace vulkan
{
/// @brief Device extensions to use
static constexpr const char * s_deviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_EXT_ROBUSTNESS_2_EXTENSION_NAME,
    VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
    VK_KHR_MAINTENANCE3_EXTENSION_NAME,
    VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME,
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
#if !defined(VENOM_PLATFORM_IOS)
    VK_EXT_HDR_METADATA_EXTENSION_NAME,
#endif
#if defined(VENOM_PLATFORM_APPLE) // Maybe Linux ?
    "VK_KHR_portability_subset",
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    VK_EXT_METAL_OBJECTS_EXTENSION_NAME,
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
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Camera).GroupUpdateSampler(__repeatSampler, 1, VK_DESCRIPTOR_TYPE_SAMPLER, 1, 0);
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Camera).GroupUpdateSampler(__clampSampler, 2, VK_DESCRIPTOR_TYPE_SAMPLER, 1, 0);
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
#if !defined(VENOM_DISABLE_GLFW)
    glfwSetWindowUserPointer((GLFWwindow*)vc::Context::Get()->GetWindow(), this);
    glfwSetFramebufferSizeCallback((GLFWwindow*)vc::Context::Get()->GetWindow(), [](GLFWwindow * window, int width, int height) {
        auto app = reinterpret_cast<VulkanApplication*>(glfwGetWindowUserPointer(window));
        app->__framebufferChanged = true;
    });
#endif
}

vc::Error VulkanApplication::__InitRenderingPipeline()
{
    vc::Error err;
    vc::Vector<PhysicalDevice> physicalDevices = PhysicalDevice::GetVulkanPhysicalDevices();

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
    createInfo.enabledExtensionCount = std::size(s_deviceExtensions);
    createInfo.ppEnabledExtensionNames = s_deviceExtensions;

    // All Features
    bool physicalDeviceFeaturesSupported = true;
    VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT, nullptr };
    // Activate Fragment Shader Interlock
    VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT fragmentShaderInterlockFeatures{};
    fragmentShaderInterlockFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT;
    fragmentShaderInterlockFeatures.fragmentShaderPixelInterlock = VK_TRUE;
    descriptorIndexingFeatures.pNext = &fragmentShaderInterlockFeatures;
    VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = &descriptorIndexingFeatures
    };
#if defined(VENOM_PLATFORM_IOS)
    physicalDeviceFeatures2.features.vertexPipelineStoresAndAtomics = VK_TRUE;
    physicalDeviceFeatures2.features.fragmentStoresAndAtomics = VK_TRUE;
#endif
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

    __CreateAttachments();

    // Initialize Screen Square Vertex Buffer
    {
        const vcm::Vec3 vertices[] = {
            vcm::Vec3(-1.0f,  1.0f, 0.0f),
            vcm::Vec3(-1.0f, -1.0f, 0.0f),
            vcm::Vec3( 1.0f, -1.0f, 0.0f),

            vcm::Vec3( 1.0f,  1.0f, 0.0f),
            vcm::Vec3(-1.0f,  1.0f, 0.0f),
            vcm::Vec3( 1.0f, -1.0f, 0.0f),
        };
        if (__screenQuadVertexBuffer.Init(6, sizeof(vcm::Vec3), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertices) != vc::Error::Success)
        {
            vc::Log::Error("Failed to create vertex buffer for screen draw operations");
            return vc::Error::Failure;
        }
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
                if (err = graphicsCommandPool->CreateCommandBuffer(&__shadowMapDirectionalCommandBuffers[i][j][k]); err != vc::Error::Success)
                    return err;
            }
        }
        for (int j = 0; j < std::size(__shadowMapPointCommandBuffers[i]); ++j) {
            for (int k = 0; k < std::size(__shadowMapPointCommandBuffers[i][j]); ++k) {
                if (err = graphicsCommandPool->CreateCommandBuffer(&__shadowMapPointCommandBuffers[i][j][k]); err != vc::Error::Success)
                    return err;
            }
        }
        for (int j = 0; j < std::size(__shadowMapSpotCommandBuffers[i]); ++j) {
            if (err = graphicsCommandPool->CreateCommandBuffer(&__shadowMapSpotCommandBuffers[i][j]); err != vc::Error::Success)
                return err;
        }
    }

    // Create Sampler
    __repeatSampler.SetCreateInfo({
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
    if (err = __repeatSampler.Create(); err != vc::Error::Success)
        return err;
    __repeatSampler.SetAsMainSampler();

    __clampSampler.SetCreateInfo({
    .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .magFilter = VK_FILTER_LINEAR,
    .minFilter = VK_FILTER_LINEAR,
    .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
    .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    .mipLodBias = 0.0f,
    .anisotropyEnable = VK_FALSE,
    .maxAnisotropy = PhysicalDevice::GetUsedPhysicalDevice().GetProperties().limits.maxSamplerAnisotropy,
    .compareEnable = VK_FALSE,
    .compareOp = VK_COMPARE_OP_ALWAYS,
    .minLod = 0.0f,
    .maxLod = 1.0f,
    .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    .unnormalizedCoordinates = VK_FALSE
    });
    if (err = __clampSampler.Create(); err != vc::Error::Success)
        return err;

    // Init Queue Order Pool
    __queueOrderPool.reset(new QueueOrderPool());
    return vc::Error::Success;
}

bool VulkanApplication::__IsDeviceSuitable(const VkDeviceCreateInfo * createInfo)
{
    // Check if the device supports the extensions
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(__physicalDevice.GetVkPhysicalDevice(), nullptr, &extensionCount, nullptr);
    vc::Vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(__physicalDevice.GetVkPhysicalDevice(), nullptr, &extensionCount, availableExtensions.data());

    vc::Set<vc::String> requiredExtensions(createInfo->ppEnabledExtensionNames, createInfo->ppEnabledExtensionNames + createInfo->enabledExtensionCount);
    for (const auto& extension : availableExtensions) {
#if defined(VENOM_DEBUG)
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

void VulkanApplication::__ChangeShadowMapsLayout(VkImageLayout oldLayout, VkImageLayout newLayout, CommandBuffer* commandBuffer)
{
    // for (int i = 0; i < std::size(__shadowMapsDirectional[_currentFrame]); ++i) {
    //     __shadowMapsDirectional[_currentFrame][i].GetImpl()->As<VulkanTexture>()->GetImage().SetImageLayout(newLayout);
    // }
    // for (int i = 0; i < std::size(__shadowMapsPoint[_currentFrame]); ++i) {
    //     __shadowMapsPoint[_currentFrame][i].GetImpl()->As<VulkanTexture>()->GetImage().SetImageLayout(newLayout);
    // }
}

vc::Error VulkanApplication::__CreateInstance()
{
    vc::Error err;
#if defined(VENOM_DEBUG)
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

            // Update Descriptor Sets
            switch (j) {
                case static_cast<int>(vc::ColorAttachmentType::LightingAddition): {
                    AttachmentsManager::Get()->attachments[i][j].GetImpl()->As<VulkanTexture>()->GetImage().SetImageLayout(VK_IMAGE_LAYOUT_GENERAL);
//                    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light)
//                        .GroupUpdateImageViewPerFrame(i, AttachmentsManager::Get()->attachments[i][j].GetImpl()->As<VulkanTexture>()->GetImageView(), 10, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, 0);
                    break;
                }
                default: break;
            }
        }
    }
}

vc::Error VulkanApplication::__RecreateSwapChain()
{
    vc::Error err;
    vkDeviceWaitIdle(LogicalDevice::GetVkDevice());
    _currentFrame = 0;
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
    for (const auto renderPass : VulkanRenderPass::GetRenderPasses()) {
        if (renderPass && renderPass->Init() != vc::Error::Success)
            return vc::Error::InitializationFailed;
    }
    // Reset Light Pass Image
//    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
//        __colorPassImage[i] = vc::Texture();
//        __colorPassImage[i].CreateReadWriteTexture(__swapChain.extent.width, __swapChain.extent.height, vc::ShaderVertexFormat::Vec4, 1);
//        DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateTexturePerFrame(i, __colorPassImage[i], 4, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, 0);
//    }
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
