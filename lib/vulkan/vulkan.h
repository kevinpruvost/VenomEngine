///
/// Project: Bazel_Vulkan_Metal
/// File: vulkan.h
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanDebug.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanQueueFamily.h"
#include "VulkanSurface.h"
#include "VulkanSwapChain.h"
#include "VulkanShader.h"
#include "VulkanRenderPass.h"
#include "VulkanCommandPool.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"

#include <common/Application.h>
#include <common/Context.h>

namespace venom
{

class VulkanApplication
    : public ApplicationBackend
    , public VulkanDebugApplication
{
public:
    VulkanApplication();
    ~VulkanApplication();
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
    VulkanPhysicalDevice __physicalDevice;
    MappedVulkanQueueFamilies __queueFamilies;
    VulkanSurface __surface;
    VulkanSwapChain __swapChain;
    VulkanRenderPass __renderPass;
    VulkanCommandPool __commandPool;

    VkQueue __graphicsQueue, __presentQueue;

private:
    // For test
    VulkanShaderPipeline __shaderPipeline;
    static constexpr const int MAX_FRAMES_IN_FLIGHT = 2;
    std::vector<VulkanCommandBuffer *> __commandBuffers;
    std::vector<VulkanSemaphore> __imageAvailableSemaphores;
    std::vector<VulkanSemaphore> __renderFinishedSemaphores;
    std::vector<VulkanFence> __inFlightFences;
    int __currentFrame;
    bool __framebufferChanged;
};
}

extern "C" EXPORT venom::ApplicationBackend* createApplication();