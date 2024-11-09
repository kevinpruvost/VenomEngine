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
#include <venom/vulkan/plugin/graphics/ShaderResourceTable.h>

#include <venom/common/FpsCounter.h>

#include <venom/vulkan/plugin/graphics/Texture.h>

#include <venom/common/VenomSettings.h>
#include <venom/vulkan/plugin/graphics/Material.h>

#include "venom/common/ECS.h"
#include <venom/vulkan/plugin/graphics/Skybox.h>

#include "venom/common/plugin/graphics/GUI.h"

namespace venom::vulkan
{
int VulkanApplication::__bindlessSupported = false;

VulkanApplication::VulkanApplication()
    : vc::GraphicsApplication()
    , DebugApplication()
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

bool VulkanApplication::ShouldClose() { return __shouldClose; }

vc::Error VulkanApplication::__Loop()
{
    vc::Error err;
    static vc::FpsCounter fps;
    static vc::Timer timer;

    vc::Timer pollP;
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
    __shouldClose = vc::Context::Get()->ShouldClose();
    if (__shouldClose) {
        vkDeviceWaitIdle(LogicalDevice::GetVkDevice());
    }
    return err;
}

void VulkanApplication::__UpdateUniformBuffers()
{
    static vc::Timer timer_uni;
    float time = timer_uni.GetMilliSeconds();
    timer_uni.Reset();

    vcm::Vec3 cameraInitialPos = {-2, -2, 1};
    vc::ECS::GetECS()->ForEach<vc::Model, vc::Transform3D>([&](vc::Entity entity, vc::Model & model, vc::Transform3D & transform)
    {
        transform.RotateYaw(time / 1000.0f);
        transform.UpdateModelMatrix();
        // Make camera rotate around object
        vcm::Vec3 cameraPos = cameraInitialPos;
        //vc::Camera::GetMainCamera().RotateAround(transform.GetPosition(), {0,1,0}, time * 0.03f);
        // vc::Camera::GetMainCamera()->LookAt(transform.GetPosition());
    });

    // Camera
    vcm::Mat4 viewAndProj[2];
    viewAndProj[0] = vc::Camera::GetMainCamera()->GetViewMatrix();
    viewAndProj[1] = vc::Camera::GetMainCamera()->GetProjectionMatrix();

    // Uniform buffers
    // Model Matrices
#ifndef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
#error ("VENOM_EXTERNAL_PACKED_MODEL_MATRIX must be defined for Vulkan")
#else
    __objectStorageBuffers[_currentFrame].WriteToBuffer(vc::ShaderResourceTable::GetAllModelMatrixBuffer(), vc::ShaderResourceTable::GetAllModelMatrixBytesSize());
    //__objectStorageBuffers[_currentFrame].WriteToBuffer(&model, sizeof(vcm::Mat4));
#endif
    // View and Projection
    __cameraUniformBuffers[_currentFrame].WriteToBuffer(viewAndProj, sizeof(viewAndProj));
    // Push Constants (model)
    // __commandBuffers[_currentFrame]->PushConstants(&__shaderPipeline, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(vcm::Mat4), &model);
}

vc::Error VulkanApplication::__DrawFrame()
{
    // Draw image
    vc::ECS::GetECS()->ForEach<vc::Model>([&](vc::Entity entity, vc::Model & model)
    {
        for (auto & mesh : model.GetImpl()->As<VulkanModel>()->GetMeshes())
            mesh.GetMaterial().GetImpl()->ConstAs<VulkanMaterial>()->GetMaterialDescriptorSet();
    });
    // Wait for the fence to be signaled
    vkWaitForFences(LogicalDevice::GetVkDevice(), 1, __inFlightFences[_currentFrame].GetFence(), VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(LogicalDevice::GetVkDevice(), __swapChain.swapChain, UINT64_MAX, __imageAvailableSemaphores[_currentFrame].GetSemaphore(), VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || __framebufferChanged) {
        __framebufferChanged = false;
        vc::Log::Print("Recreating swap chain");
        return __RecreateSwapChain();
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        vc::Log::Error("Failed to acquire swap chain image");
        return vc::Error::Failure;
    }

    // If we reset before, then it will wait endlessly as no work is done
    vkResetFences(LogicalDevice::GetVkDevice(), 1, __inFlightFences[_currentFrame].GetFence());
    __commandBuffers[_currentFrame]->Reset(0);

    if (auto err = __commandBuffers[_currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __renderPass.BeginRenderPass(&__swapChain, __commandBuffers[_currentFrame], imageIndex);
        __commandBuffers[_currentFrame]->SetViewport(__swapChain.viewport);
        __commandBuffers[_currentFrame]->SetScissor(__swapChain.scissor);
        //__descriptorSets[_currentFrame].UpdateTexture(reinterpret_cast<const VulkanTexture*>(__texture), 2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, 0);
        //__commandBuffers[_currentFrame]->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, __shaderPipeline.GetPipelineLayout(), 0, 1, __descriptorSets[_currentFrame].GetVkDescriptorSet());

        // Update Uniform Buffers
        __UpdateUniformBuffers();

        // Draw Skybox
        vc::ECS::GetECS()->ForEach<vc::Skybox, vc::Shader>([&](vc::Entity entity, vc::Skybox & skybox, vc::Shader & shader)
        {
            shader.GetImpl()->As<VulkanShader>()->SetDepthWrite(false);
            __commandBuffers[_currentFrame]->DrawSkybox(skybox.GetImpl()->As<VulkanSkybox>(), shader.GetImpl()->As<VulkanShader>());
        });

        // Draw Models
        vc::ECS::GetECS()->ForEach<vc::Model, vc::Transform3D, vc::Shader>([&](vc::Entity entity, vc::Model & model, vc::Transform3D & transform, vc::Shader & shader)
        {
            int index;
#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
            index = transform.GetModelMatrixId();
#endif
            if (!__commandBuffers[_currentFrame]->BindPipeline(shader.GetImpl()->As<VulkanShader>()->GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS)) {
                DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_MODEL_MATRICES, *__commandBuffers[_currentFrame], *shader.GetImpl()->As<VulkanShader>(), VK_PIPELINE_BIND_POINT_GRAPHICS);
                DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_CAMERA, *__commandBuffers[_currentFrame], *shader.GetImpl()->As<VulkanShader>(), VK_PIPELINE_BIND_POINT_GRAPHICS);
                DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SAMPLER, *__commandBuffers[_currentFrame], *shader.GetImpl()->As<VulkanShader>(), VK_PIPELINE_BIND_POINT_GRAPHICS);
            }
            __commandBuffers[_currentFrame]->DrawModel(model.GetImpl()->As<VulkanModel>(), index, *shader.GetImpl()->As<VulkanShader>());
        });

        // Draw GUI
        _gui->Render();

        __renderPass.EndRenderPass(__commandBuffers[_currentFrame]);

    if (auto err = __commandBuffers[_currentFrame]->EndCommandBuffer(); err != vc::Error::Success)
        return err;

    // Synchronization between the image being presented and the image being rendered
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {__imageAvailableSemaphores[_currentFrame].GetSemaphore()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffers[] = {__commandBuffers[_currentFrame]->GetVkCommandBuffer()};
    submitInfo.pCommandBuffers = commandBuffers;
    VkSemaphore signalSemaphores[] = {__renderFinishedSemaphores[_currentFrame].GetSemaphore()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vc::Timer theoreticalFpsCounter;
    if (result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, *__inFlightFences[_currentFrame].GetFence()); result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || __framebufferChanged) {
        __framebufferChanged = false;
        return __RecreateSwapChain();
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

    _currentFrame = (_currentFrame + 1) % VENOM_MAX_FRAMES_IN_FLIGHT;
    return vc::Error::Success;
}
}
