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
#include <venom/vulkan/plugin/graphics/ShaderPipeline.h>
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
    inline const SwapChain * GetSwapChain() const { return &__swapChain; }
    inline const RenderPass * GetRenderPass() const { return &__skyboxRenderPass; }
    inline const RenderPass * GetGuiRenderPass() const { return &__skyboxRenderPass; }
    inline const DescriptorPool * GetDescriptorPool() const { return &__descriptorPool; }
    inline const RenderPass * GetHDRRenderPass() const { return &__skyboxRenderPass; }
    inline const CommandBuffer * GetCommandBuffer(const int index) const { return __graphicsFirstCheckpointCommandBuffers[index]; }
    inline const CommandBuffer * GetCurrentGraphicsCommandBuffer() const { return __graphicsSecondCheckpointCommandBuffers[GetCurrentFrame()]; }

protected:
    vc::Error _LoadGfxSettings() override;

    vc::Error _SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples) override;
    vc::Vector<MultiSamplingCountOption> _GetAvailableMultisamplingOptions() override;

    vc::Error _SetHDR(bool enable) override;

private:
    void __UpdateUniformBuffers();
    vc::Error __GraphicsOperations();
    vc::Error __ComputeOperations();
    vc::Error __DrawFrame();
    vc::Error __InitVulkan();
    VkPhysicalDeviceFeatures2 __GetPhysicalDeviceFeatures(bool & supported, VkPhysicalDeviceDescriptorIndexingFeatures & descriptorIndexingFeatures, VkPhysicalDeviceFeatures2 & features);

    void __SetGLFWCallbacks();

    vc::Error __InitRenderingPipeline();
    bool __IsDeviceSuitable(const VkDeviceCreateInfo * createInfo);

    vc::Error __CreateInstance();

    void __CreateAttachments();
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
    AttachmentsManager __attachmentsManager;
    RenderPass __skyboxRenderPass, __shadowRenderPass, __guiRenderPass;
    CommandPoolManager __commandPoolManager;
    QueueManager __queueManager;

    UniformBuffer __targetLuminanceBuffer, __screenPropsBuffer;
    UniformBuffer __lightsBuffer;
    UniformBuffer __lightCountBuffer;
    StorageBuffer __forwardPlusPropsBuffer[VENOM_MAX_FRAMES_IN_FLIGHT];

    VertexBuffer __screenQuadVertexBuffer;

    Queue __graphicsQueue, __presentQueue;

    bool __shouldClose;

private:
    // For test
    uint32_t __imageIndex;
    static int __bindlessSupported;
    Sampler __sampler;
    vc::ShaderPipeline __shaderPipeline;

    CommandBuffer * __graphicsFirstCheckpointCommandBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    CommandBuffer * __graphicsSecondCheckpointCommandBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    CommandBuffer * __computeCommandBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];

    Semaphore __imageAvailableSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];
    Semaphore __renderFinishedSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];
    Semaphore __graphicsFirstCheckpointSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];
    Semaphore __computeShadersFinishedSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];

    Fence __graphicsInFlightFences[VENOM_MAX_FRAMES_IN_FLIGHT];
    Fence __computeInFlightFences[VENOM_MAX_FRAMES_IN_FLIGHT];

    bool __framebufferChanged;
    StorageBuffer __objectStorageBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    UniformBuffer __cameraUniformBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
};
}
}
