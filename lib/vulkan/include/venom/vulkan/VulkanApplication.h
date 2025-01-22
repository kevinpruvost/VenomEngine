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

#include "venom/common/plugin/graphics/Light.h"

namespace venom
{
/// @brief Encapsulation of Vulkan for the front end of VenomEngine.
namespace vulkan
{
class VulkanShaderResourceTable;
class VulkanLight;

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
    inline const RenderPass * GetGuiRenderPass() const { return &__guiRenderPass; }
    inline const DescriptorPool * GetDescriptorPool() const { return &__descriptorPool; }
    inline const RenderPass * GetHDRRenderPass() const { return &__skyboxRenderPass; }
    inline const CommandBuffer * GetCommandBuffer(const int index) const { return __graphicsFirstCheckpointCommandBuffers[index]; }
    inline const CommandBuffer * GetCurrentGraphicsCommandBuffer() const { return __graphicsSceneCheckpointCommandBuffers[GetCurrentFrameInFlight()]; }

protected:
    vc::Error _LoadGfxSettings() override;

    vc::Error _SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples) override;
    vc::Vector<MultiSamplingCountOption> _GetAvailableMultisamplingOptions() override;

    vc::Error _SetHDR(bool enable) override;

private:
    void __UpdateUniformBuffers();
    vc::Error __GraphicsOperations();
    vc::Error __GraphicsShadowMapOperations();
    vc::Error __GraphicsShadowMapOperationPerLight(const vc::Light * light, const int lightIndex,
        const vc::LightCascadedShadowMapConstantsStruct & constants,
        const Framebuffer * const framebuffer, Semaphore * semaphore, CommandBuffer * const commandBuffer,
        const vc::ShaderPipeline * const shaderPipeline);
    vc::Error __ComputeOperations();
    vc::Error __DrawFrame();
    vc::Error __InitVulkan();
    VkPhysicalDeviceFeatures2 __GetPhysicalDeviceFeatures(bool & supported, VkPhysicalDeviceDescriptorIndexingFeatures & descriptorIndexingFeatures, VkPhysicalDeviceFeatures2 & features);

    void __SetGLFWCallbacks();

    vc::Error __InitRenderingPipeline();
    bool __IsDeviceSuitable(const VkDeviceCreateInfo * createInfo);

    void __ChangeShadowMapsLayout(VkImageLayout oldLayout, VkImageLayout newLayout, CommandBuffer * commandBuffer);

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
    RenderPass __skyboxRenderPass, __graphicsRenderPass, __guiRenderPass, __shadowMapRenderPass;
    CommandPoolManager __commandPoolManager;
    QueueManager __queueManager;

    UniformBuffer __sceneSettingsBuffer, __graphicsSettingsBuffer;
    UniformBuffer __lightsBuffer;
    UniformBuffer __lightCountBuffer;
    StorageBuffer __forwardPlusPropsBuffer[VENOM_MAX_FRAMES_IN_FLIGHT];
    vc::Texture __brdfLutTexture, __irradianceMapTexture, __radianceMapTexture;

    vc::Texture __shadowMapsDirectional[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_CSM_TOTAL_CASCADES];
    vc::Texture __shadowMapsPoint[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_CSM_TOTAL_CASCADES];
    vc::Texture __shadowMapsSpot[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_CSM_TOTAL_CASCADES];
    ImageView * __shadowMapDirectionalImageViews[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_CSM_TOTAL_CASCADES][VENOM_MAX_DIRECTIONAL_LIGHTS];
    ImageView * __shadowMapPointImageViews[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_CSM_TOTAL_CASCADES][VENOM_MAX_POINT_LIGHTS * 6];
    ImageView * __shadowMapSpotImageViews[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_CSM_TOTAL_CASCADES][VENOM_MAX_SPOT_LIGHTS];

    StorageBuffer __shadowMapsIndicesBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    StorageBuffer __shadowMapDirectionalLightSpaceMatricesBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    StorageBuffer __shadowMapPointLightSpaceMatricesBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    StorageBuffer __shadowMapSpotLightSpaceMatricesBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    int __shadowMapIndices[VENOM_MAX_LIGHTS];
    vcm::Mat4 __shadowMapDirectionalLightSpaceMatrices[VENOM_CSM_TOTAL_CASCADES * VENOM_MAX_DIRECTIONAL_LIGHTS];
    vcm::Mat4 __shadowMapPointLightSpaceMatrices[6 * VENOM_MAX_POINT_LIGHTS];
    vcm::Mat4 __shadowMapSpotLightSpaceMatrices[VENOM_MAX_SPOT_LIGHTS];

    UniformBuffer __radianceRoughness;
    Image __brdfLut, __irradianceMap;
    ImageView __brdfLutView, __irradianceMapView;

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
    CommandBuffer * __graphicsSceneCheckpointCommandBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    CommandBuffer * __computeCommandBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];

    CommandBuffer * __shadowMapDirectionalCommandBuffers[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_MAX_DIRECTIONAL_LIGHTS][VENOM_CSM_TOTAL_CASCADES];
    CommandBuffer * __shadowMapPointCommandBuffers[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_MAX_POINT_LIGHTS][6];
    CommandBuffer * __shadowMapSpotCommandBuffers[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_MAX_SPOT_LIGHTS];
    vc::Vector<CommandBuffer *> __shadowMapCommandBuffersToReset[VENOM_MAX_FRAMES_IN_FLIGHT];

    Semaphore __imageAvailableSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];
    Semaphore __renderFinishedSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];
    Semaphore __graphicsSkyboxDoneSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];
    Semaphore __computeShadersFinishedSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];

    Semaphore __shadowMapsDirectionalFinishedSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_MAX_DIRECTIONAL_LIGHTS][VENOM_CSM_TOTAL_CASCADES];
    Semaphore __shadowMapsPointFinishedSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_MAX_POINT_LIGHTS][6];
    Semaphore __shadowMapsSpotFinishedSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT][VENOM_MAX_SPOT_LIGHTS];
    // Keeps the only ones active
    vc::Vector<Semaphore *> __shadowMapsFinishedSemaphores[VENOM_MAX_FRAMES_IN_FLIGHT];

    Fence __graphicsInFlightFences[VENOM_MAX_FRAMES_IN_FLIGHT];
    Fence __computeInFlightFences[VENOM_MAX_FRAMES_IN_FLIGHT];
    Fence __shadowMapsInFlightFences[VENOM_MAX_FRAMES_IN_FLIGHT];

    bool __framebufferChanged;
    StorageBuffer __modelMatricesStorageBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];
    UniformBuffer __cameraUniformBuffers[VENOM_MAX_FRAMES_IN_FLIGHT];

    friend class VulkanShaderResourceTable;
    friend class VulkanLight;
};
}
}
