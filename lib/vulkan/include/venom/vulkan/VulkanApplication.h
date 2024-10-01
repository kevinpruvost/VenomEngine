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
#include <venom/vulkan/Shader.h>
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

#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/Context.h>

#include "venom/common/math/Vector.h"

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
    vc::Error Init() override;
    vc::Error Loop() override;
    bool ShouldClose() override;

private:
    vc::Error __Loop();
    void __UpdateUniformBuffers();
    vc::Error __DrawFrame();
    vc::Error __InitVulkan();

    void __SetGLFWCallbacks();

    vc::Error __InitRenderingPipeline();
    bool __IsDeviceSuitable(const VkDeviceCreateInfo * createInfo);

    vc::Error __CreateInstance();

    void __RecreateSwapChain();

private:
    Instance __instance;
    LogicalDevice __logicalDevice;
    std::vector<const char *> __instanceExtensions;
    vc::Context __context;
    PhysicalDevice __physicalDevice;
    MappedQueueFamilies __queueFamilies;
    Surface __surface;
    SwapChain __swapChain;
    RenderPass __renderPass;
    CommandPoolManager __commandPoolManager;
    QueueManager __queueManager;

    Queue __graphicsQueue, __presentQueue;

    DescriptorPool __descriptorPool;
    std::vector<DescriptorSet> __descriptorSets;

    bool __shouldClose;

private:
    // For test
    Sampler __sampler;
    ShaderPipeline __shaderPipeline;
    static constexpr const int MAX_FRAMES_IN_FLIGHT = 2;
    std::vector<CommandBuffer *> __commandBuffers;
    std::vector<Semaphore> __imageAvailableSemaphores;
    std::vector<Semaphore> __renderFinishedSemaphores;
    std::vector<Fence> __inFlightFences;
    int __currentFrame;
    bool __framebufferChanged;
    VulkanModel * __model;
    VulkanMesh * __mesh;
    UniformBuffer __uniformBuffers[2];
    vcm::Vec3 __verticesPos[8] = {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f},
        {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f}
    };

    vcm::Vec4 __verticesColor[8] = {
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 0, 1, 1},
        {1, 1, 1, 1},
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 0, 1, 1},
        {1, 1, 1, 1}
    };

    vcm::Vec2 __verticesUV[8] = {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1},
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    };

    uint32_t __indices[12] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4
    };
};
}
}
