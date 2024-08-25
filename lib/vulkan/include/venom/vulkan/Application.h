///
/// Project: Bazel_Vulkan_Metal
/// File: vulkan.h
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
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

#include <common/Application.h>
#include <common/Context.h>

namespace venom
{
namespace vulkan
{
class Application
    : public ApplicationBackend
    , public DebugApplication
{
public:
    Application();
    ~Application();
    Error Run() override;

private:
    Error __Loop();
    Error __DrawFrame();
    Error __InitVulkan();

    void __SetGLFWCallbacks();

    Error __InitPhysicalDevices();
    bool __IsDeviceSuitable(const VkDeviceCreateInfo * createInfo);

    Error __CreateInstance();
    void __Instance_GetRequiredExtensions(VkInstanceCreateInfo * createInfo);

    void __RecreateSwapChain();

private:
    std::vector<const char *> __instanceExtensions;
    Context __context;
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
};
}
}
extern "C" EXPORT venom::ApplicationBackend* createApplication();