///
/// Project: VenomEngine
/// @file GraphicsApplication.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>

#include <array>
#include <vector>

#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/Allocator.h>

#include <venom/common/FpsCounter.h>

#include <venom/vulkan/plugin/graphics/Texture.h>

#include <venom/common/VenomSettings.h>

#include "venom/common/ECS.h"

namespace venom::vulkan
{
int VulkanApplication::__currentFrame = 0;
bool VulkanApplication::__bindlessSupported = false;

VulkanApplication::VulkanApplication()
    : vc::GraphicsApplication()
    , DebugApplication()
    , __context()
    , __framebufferChanged(false)
    , __shouldClose(false)
{
    Allocator::SetVKAllocationCallbacks();
}

VulkanApplication::~VulkanApplication()
{
    vc::Log::Print("Destroying Vulkan app...");
    // Set global physical device back to nullptr
    PhysicalDevice::SetUsedPhysicalDevice(nullptr);
#ifdef VENOM_DEBUG
    if (__debugMessenger) {
        auto destroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(Instance::GetVkInstance(), "vkDestroyDebugUtilsMessengerEXT"));
        destroyDebugUtilsMessengerEXT(Instance::GetVkInstance(), __debugMessenger, Allocator::GetVKAllocationCallbacks());
    }
#endif
    vc::Log::Print("Vulkan app succesfully destroyed.");
}

vc::Error VulkanApplication::Loop() { return __Loop(); }
bool VulkanApplication::ShouldClose() { return __shouldClose; }

vc::Error VulkanApplication::__Loop()
{
    vc::Error err;
    static vc::FpsCounter fps;
    static vc::Timer timer;

    vc::Timer pollP;
    __context.PollEvents();
    //vc::Log::Print("Poll time: %lu", pollP.GetMicroSeconds());
    if (err = __DrawFrame(); err != vc::Error::Success)
        return err;
    fps.RegisterFrame();
    auto duration = timer.GetMilliSeconds();
    if (duration >= 1000) {
        int fpsCount = fps.GetFps();
            vc::Log::Print("FPS: %u, Theoretical FPS: %.2f", fpsCount, _GetTheoreticalFPS(fpsCount));
        timer.Reset();
    }
    __shouldClose = __context.ShouldClose();
    if (__shouldClose) {
        vkDeviceWaitIdle(LogicalDevice::GetVkDevice());
    }
    return err;
}

void VulkanApplication::__UpdateUniformBuffers()
{
    static vc::Timer timer_uni;
    float time = timer_uni.GetMilliSeconds();

    vc::ECS::GetECS()->ForEach<vc::Model, vc::Transform3D>([&](vc::Entity entity, vc::Model & model, vc::Transform3D & transform)
    {
        transform.SetPosition({8,8,0});
        transform.Rotate({0,1,0}, time / 1000.0f);
        transform.UpdateModelMatrix();
    });

    // Camera
    vcm::Mat4 viewAndProj[2];
    vcm::Vec3 cameraPos = {-2.0f, -2.0f, 1.0f};
    __camera.SetPosition(cameraPos);
    __camera.LookAt({8,8,0});
    viewAndProj[0] = __camera.GetViewMatrix();
    viewAndProj[1] = __camera.GetProjectionMatrix();

    // Uniform buffers
    // Model Matrices
#ifndef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
#error ("VENOM_EXTERNAL_PACKED_MODEL_MATRIX must be defined for Vulkan")
#else
    memcpy(__objectStorageBuffers[__currentFrame].GetMappedData(), vc::ShaderResourceTable::GetAllModelMatrixBuffer(), vc::ShaderResourceTable::GetAllModelMatrixBytesSize());
#endif
    // View and Projection
    memcpy(__cameraUniformBuffers[__currentFrame].GetMappedData(), viewAndProj, sizeof(viewAndProj));
    // Push Constants (model)
    // __commandBuffers[__currentFrame]->PushConstants(&__shaderPipeline, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(vcm::Mat4), &model);
}

vc::Error VulkanApplication::__DrawFrame()
{
    // Draw image

    // Wait for the fence to be signaled
    vkWaitForFences(LogicalDevice::GetVkDevice(), 1, __inFlightFences[__currentFrame].GetFence(), VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(LogicalDevice::GetVkDevice(), __swapChain.swapChain, UINT64_MAX, __imageAvailableSemaphores[__currentFrame].GetSemaphore(), VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || __framebufferChanged) {
        __framebufferChanged = false;
        vc::Log::Print("Recreating swap chain");
        __RecreateSwapChain();
        return vc::Error::Success;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        vc::Log::Error("Failed to acquire swap chain image");
        return vc::Error::Failure;
    }

    // If we reset before, then it will wait endlessly as no work is done
    vkResetFences(LogicalDevice::GetVkDevice(), 1, __inFlightFences[__currentFrame].GetFence());
    __commandBuffers[__currentFrame]->Reset(0);

    if (auto err = __commandBuffers[__currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __renderPass.BeginRenderPass(&__swapChain, __commandBuffers[__currentFrame], imageIndex);
        __commandBuffers[__currentFrame]->BindPipeline(__shaderPipeline.GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
        __commandBuffers[__currentFrame]->SetViewport(__swapChain.viewport);
        __commandBuffers[__currentFrame]->SetScissor(__swapChain.scissor);
        //__descriptorSets[__currentFrame].UpdateTexture(reinterpret_cast<const VulkanTexture*>(__texture), 2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, 0);
        //__commandBuffers[__currentFrame]->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, __shaderPipeline.GetPipelineLayout(), 0, 1, __descriptorSets[__currentFrame].GetVkDescriptorSet());

        // Update Uniform Buffers
        __UpdateUniformBuffers();

        DescriptorPool::GetPool()->BindDescriptorSets(*__commandBuffers[__currentFrame], __shaderPipeline, VK_PIPELINE_BIND_POINT_GRAPHICS);
        __commandBuffers[__currentFrame]->DrawModel(__model.GetImpl()->As<VulkanModel>(), 1);
        __renderPass.EndRenderPass(__commandBuffers[__currentFrame]);

    if (auto err = __commandBuffers[__currentFrame]->EndCommandBuffer(); err != vc::Error::Success)
        return err;

    // Synchronization between the image being presented and the image being rendered
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {__imageAvailableSemaphores[__currentFrame].GetSemaphore()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffers[] = {*reinterpret_cast<VkCommandBuffer*>(__commandBuffers[__currentFrame])};
    submitInfo.pCommandBuffers = commandBuffers;
    VkSemaphore signalSemaphores[] = {__renderFinishedSemaphores[__currentFrame].GetSemaphore()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vc::Timer theoreticalFpsCounter;
    if (result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, *__inFlightFences[__currentFrame].GetFence()); result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || __framebufferChanged) {
        __framebufferChanged = false;
        __RecreateSwapChain();
        return vc::Error::Success;
    } else if (result != VK_SUCCESS) {
        vc::Log::Error("Failed to submit draw command buffer");
        return vc::Error::Failure;
    }
    _UpdateTheoreticalFPS(theoreticalFpsCounter.GetMicroSeconds());

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {__swapChain.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(__presentQueue.GetVkQueue(), &presentInfo);

    __currentFrame = (__currentFrame + 1) % VENOM_MAX_FRAMES_IN_FLIGHT;
    return vc::Error::Success;
}
}
