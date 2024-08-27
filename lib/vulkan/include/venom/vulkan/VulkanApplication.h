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
    vc::Error Run() override;

private:
    vc::Error __Loop();
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
    CommandPool __commandPool;

    VkQueue __graphicsQueue, __presentQueue;

private:
    // For test
    ShaderPipeline __shaderPipeline;
    static constexpr const int MAX_FRAMES_IN_FLIGHT = 2;
    std::vector<CommandBuffer *> __commandBuffers;
    std::vector<Semaphore> __imageAvailableSemaphores;
    std::vector<Semaphore> __renderFinishedSemaphores;
    std::vector<Fence> __inFlightFences;
    int __currentFrame;
    bool __framebufferChanged;
    std::vector<VkBuffer> __vertexBuffers;
    std::vector<VkDeviceMemory> __vertexBuffersMemory;
    vc::Vec3 __verticesPos[3] = {
        {0, -0.5f, 0},
        {0.5f, 0.5f, 0},
        {-0.5f, 0.5f, 0}
    };
    vc::Vec3 __verticesColor[3] = {
        {1, 1, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
};
}
}
