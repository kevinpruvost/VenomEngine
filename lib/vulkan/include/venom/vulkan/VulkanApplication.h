///
/// Project: VenomEngine
/// @file GraphicsApplication.h
/// @date Aug, 18 2024
/// @brief Vulkan Encapsulation of the VulkanApplication class.
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>
#include <venom/vulkan/PhysicalDevice.h>
#include <venom/vulkan/QueueFamily.h>
#include <venom/vulkan/Surface.h>
#include <venom/vulkan/SwapChain.h>
#include <venom/vulkan/plugin/graphics/Shader.h>
#include <venom/vulkan/RenderPass.h>
#include <venom/vulkan/CommandPool.h>
#include <venom/vulkan/Semaphore.h>
#include <venom/vulkan/Fence.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/plugin/graphics/Model.h>
#include <venom/vulkan/CommandPoolManager.h>
#include <venom/vulkan/QueueManager.h>
#include <venom/vulkan/UniformBuffer.h>
#include <venom/vulkan/DescriptorPool.h>
#include <venom/vulkan/StorageBuffer.h>

#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/Context.h>

#include <venom/common/VenomSettings.h>
#include <venom/common/math/Vector.h>

#include <venom/vulkan/plugin/graphics/Camera.h>

namespace venom
{
/// @brief Encapsulation of Vulkan for the front end of VenomEngine.
namespace vulkan
{
class VulkanApplication
    : public vc::GraphicsApplication
    , public DebugApplication
{
public:
    VulkanApplication();
    ~VulkanApplication() override;
    vc::Error __Init() override;
    vc::Error __PostInit() override;
    vc::Error __Loop() override;
    bool ShouldClose() override;
public:
    inline static int IsBindlessSupported() { return __bindlessSupported; }

protected:
    vc::Error _SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples) override;
    vc::Error _LoadGfxSettings() override;
    vc::Vector<MultiSamplingCountOption> _GetAvailableMultisamplingOptions() override;

private:
    void __UpdateUniformBuffers();
    vc::Error __DrawFrame();
    vc::Error __InitVulkan();
    VkPhysicalDeviceFeatures2 __GetPhysicalDeviceFeatures(bool & supported, VkPhysicalDeviceDescriptorIndexingFeatures & descriptorIndexingFeatures, VkPhysicalDeviceFeatures2 & features);

    void __SetGLFWCallbacks();

    vc::Error __InitRenderingPipeline();
    bool __IsDeviceSuitable(const VkDeviceCreateInfo * createInfo);

    vc::Error __CreateInstance();

    vc::Error __RecreateSwapChain();
    vc::Error __InitializeSets();

private:
    Instance __instance;
    LogicalDevice __logicalDevice;
    DescriptorPool __descriptorPool;
    std::vector<const char *> __instanceExtensions;
    PhysicalDevice __physicalDevice;
    MappedQueueFamilies __queueFamilies;
    Surface __surface;
    SwapChain __swapChain;
    RenderPass __renderPass;
    CommandPoolManager __commandPoolManager;
    QueueManager __queueManager;

    Queue __graphicsQueue, __presentQueue;

    bool __shouldClose;

private:
    // For test
    static int __bindlessSupported;
    Sampler __sampler;
    vc::Shader __shaderPipeline;
    CommandBuffer * __commandBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    Semaphore __imageAvailableSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];
    Semaphore __renderFinishedSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];
    Fence __inFlightFences[VENOM_MAX_FRAMES_IN_FLIGHT];
    bool __framebufferChanged;
    StorageBuffer __objectStorageBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    UniformBuffer __cameraUniformBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
};
}
}
