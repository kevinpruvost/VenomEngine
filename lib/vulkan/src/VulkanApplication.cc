///
/// Project: VenomEngine
/// @file GraphicsApplication.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>

#include <array>
#include <thread>
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

#include "venom/common/Light.h"
#include "venom/common/plugin/graphics/GUI.h"
#include "venom/common/plugin/graphics/RenderingPipeline.h"

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

    vc::ECS::GetECS()->ForEach<vc::Model, vc::Transform3D>([&](vc::Entity entity, vc::Model & model, vc::Transform3D & transform)
    {
        transform.UpdateModelMatrix();
    });

    // Camera
    struct CameraData
    {
        vcm::Mat4 viewAndProj[2];
        vcm::Vec3 cameraPos;
    };
    CameraData camProps;
    camProps.viewAndProj[0] = vc::Camera::GetMainCamera()->GetViewMatrix();
    camProps.viewAndProj[1] = vc::Camera::GetMainCamera()->GetProjectionMatrix();
    camProps.cameraPos = vc::Camera::GetMainCamera()->GetPosition();

    // Uniform buffers
    // Model Matrices
#ifndef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
#error ("VENOM_EXTERNAL_PACKED_MODEL_MATRIX must be defined for Vulkan")
#else
    __objectStorageBuffers[_currentFrame].WriteToBuffer(vc::ShaderResourceTable::GetAllModelMatrixBuffer(), vc::ShaderResourceTable::GetAllModelMatrixBytesSize());
    //__objectStorageBuffers[_currentFrame].WriteToBuffer(&model, sizeof(vcm::Mat4));
#endif
    // View and Projection
    __cameraUniformBuffers[_currentFrame].WriteToBuffer(&camProps, sizeof(CameraData));
    // Push Constants (model)
    // __graphicsFirstCheckpointCommandBuffers[_currentFrame]->PushConstants(&__shaderPipeline, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(vcm::Mat4), &model);

    // Lights
    static vc::Array<vc::LightShaderStruct, VENOM_MAX_LIGHTS> lightShaderStructs;
    const auto & lights = vc::LightManager::GetLights();
    for (int i = 0; i < lights.size(); ++i) {
        lightShaderStructs[i] = lights[i]->GetShaderStruct();
    }
    __lightsBuffer.WriteToBuffer(lightShaderStructs.data(), sizeof(vc::LightShaderStruct) * lightShaderStructs.size());
    uint32_t lightCount = lights.size();
    __lightCountBuffer.WriteToBuffer(&lightCount, sizeof(uint32_t));
}

vc::Error VulkanApplication::__GraphicsOperations()
{
    if (auto err = __graphicsFirstCheckpointCommandBuffers[_currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __graphicsFirstCheckpointCommandBuffers[_currentFrame]->SetViewport(__swapChain.viewport);
        __graphicsFirstCheckpointCommandBuffers[_currentFrame]->SetScissor(__swapChain.scissor);

        // Update Uniform Buffers
        __UpdateUniformBuffers();

        // Draw Skybox
        __skyboxRenderPass.BeginRenderPass(&__swapChain, __graphicsFirstCheckpointCommandBuffers[_currentFrame], __imageIndex);
            vc::ECS::GetECS()->ForEach<vc::Skybox, vc::RenderingPipeline>([&](vc::Entity entity, vc::Skybox & skybox, vc::RenderingPipeline & pipeline)
            {
                const auto & shaders = pipeline.GetRenderingPipelineCache();
                shaders[0].GetConstImpl()->ConstAs<VulkanShaderPipeline>()->SetDepthWrite(false);
                __graphicsFirstCheckpointCommandBuffers[_currentFrame]->DrawSkybox(skybox.GetImpl()->As<VulkanSkybox>(), shaders[0].GetConstImpl()->ConstAs<VulkanShaderPipeline>());
            });
        __skyboxRenderPass.EndRenderPass(__graphicsFirstCheckpointCommandBuffers[_currentFrame]);

    if (auto err = __graphicsFirstCheckpointCommandBuffers[_currentFrame]->EndCommandBuffer(); err != vc::Error::Success)
        return err;

    {
        // Synchronization between the image being presented and the image being rendered
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = {__imageAvailableSemaphores[_currentFrame].GetSemaphore()};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = std::size(waitSemaphores);
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        VkCommandBuffer commandBuffers[] = {__graphicsFirstCheckpointCommandBuffers[_currentFrame]->GetVkCommandBuffer()};
        submitInfo.pCommandBuffers = commandBuffers;
        VkSemaphore signalSemaphores[] = {__graphicsFirstCheckpointSemaphores[_currentFrame].GetSemaphore()};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VkResult result;
        vc::Timer theoreticalFpsCounter;
        if (result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, VK_NULL_HANDLE); result != VK_SUCCESS) {
            vc::Log::Error("Failed to submit draw command buffer");
            return vc::Error::Failure;
        }
    }

    if (auto err = __graphicsSecondCheckpointCommandBuffers[_currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __graphicsSecondCheckpointCommandBuffers[_currentFrame]->SetViewport(__swapChain.viewport);
        __graphicsSecondCheckpointCommandBuffers[_currentFrame]->SetScissor(__swapChain.scissor);

        // Draw Shadowed Models (Forward+)
        const auto & shadowRenderingPipeline = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::ShadowModel);
        __shadowRenderPass.BeginRenderPass(&__swapChain, __graphicsSecondCheckpointCommandBuffers[_currentFrame], __imageIndex);
            // Draw Shadow Models
            // Second pass : Geometry pass
            __graphicsSecondCheckpointCommandBuffers[_currentFrame]->BindPipeline(shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>()->GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_MODEL_MATRICES, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], *shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>(), VK_PIPELINE_BIND_POINT_GRAPHICS);
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_CAMERA, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], *shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>(), VK_PIPELINE_BIND_POINT_GRAPHICS);
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SAMPLER, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], *shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>(), VK_PIPELINE_BIND_POINT_GRAPHICS);
            vc::ECS::GetECS()->ForEach<vc::Model, vc::Transform3D>([&](vc::Entity entity, vc::Model & model, vc::Transform3D & transform)
            {
                int index;
            #ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
                index = transform.GetModelMatrixId();
            #endif
                __graphicsSecondCheckpointCommandBuffers[_currentFrame]->DrawModel(model.GetImpl()->As<VulkanModel>(), index, *shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            });

            // Execute Lighting shader
            __shadowRenderPass.NextSubpass(__graphicsSecondCheckpointCommandBuffers[_currentFrame]);
            __graphicsSecondCheckpointCommandBuffers[_currentFrame]->BindPipeline(shadowRenderingPipeline[1].GetImpl()->As<VulkanShaderPipeline>()->GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_CAMERA, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], *shadowRenderingPipeline[1].GetImpl()->As<VulkanShaderPipeline>(), VK_PIPELINE_BIND_POINT_GRAPHICS);
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], *shadowRenderingPipeline[1].GetImpl()->As<VulkanShaderPipeline>(), VK_PIPELINE_BIND_POINT_GRAPHICS);
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], *shadowRenderingPipeline[1].GetImpl()->As<VulkanShaderPipeline>(), VK_PIPELINE_BIND_POINT_GRAPHICS);
            __graphicsSecondCheckpointCommandBuffers[_currentFrame]->DrawVertices(__screenQuadVertexBuffer);

        __shadowRenderPass.EndRenderPass(__graphicsSecondCheckpointCommandBuffers[_currentFrame]);

        // Draw GUI
        __guiRenderPass.BeginRenderPass(&__swapChain, __graphicsSecondCheckpointCommandBuffers[_currentFrame], __imageIndex);
            _gui->Render();
        __guiRenderPass.EndRenderPass(__graphicsSecondCheckpointCommandBuffers[_currentFrame]);

    if (auto err = __graphicsSecondCheckpointCommandBuffers[_currentFrame]->EndCommandBuffer(); err != vc::Error::Success)
        return err;

    VkSemaphore signalSemaphores[] = {__renderFinishedSemaphores[_currentFrame].GetSemaphore()};
    {
        // Synchronization between the image being presented and the image being rendered
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = {__graphicsFirstCheckpointSemaphores[_currentFrame].GetSemaphore(), __computeShadersFinishedSemaphores[_currentFrame].GetSemaphore()};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = std::size(waitSemaphores);
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        VkCommandBuffer commandBuffers[] = {__graphicsSecondCheckpointCommandBuffers[_currentFrame]->GetVkCommandBuffer()};
        submitInfo.pCommandBuffers = commandBuffers;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VkResult result;
        vc::Timer theoreticalFpsCounter;
        if (result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, *__graphicsInFlightFences[_currentFrame].GetFence()); result != VK_SUCCESS) {
            vc::Log::Error("Failed to submit draw command buffer");
            return vc::Error::Failure;
        }
        _UpdateTheoreticalFPS(theoreticalFpsCounter.GetMicroSeconds());
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {__swapChain.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &__imageIndex;

    vkQueuePresentKHR(__presentQueue.GetVkQueue(), &presentInfo);
    return vc::Error::Success;
}

vc::Error VulkanApplication::__ComputeOperations()
{
    vc::Error err;
    if (err = __computeCommandBuffers[_currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        const auto & shadowRenderingPipeline = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::ShadowModel);
        // First pass : Light Culling
        __computeCommandBuffers[_currentFrame]->BindPipeline(shadowRenderingPipeline[2].GetImpl()->As<VulkanShaderPipeline>()->GetPipeline(), VK_PIPELINE_BIND_POINT_COMPUTE);
        DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_CAMERA, *__computeCommandBuffers[_currentFrame], *shadowRenderingPipeline[2].GetImpl()->As<VulkanShaderPipeline>(), VK_PIPELINE_BIND_POINT_COMPUTE);
        DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, *__computeCommandBuffers[_currentFrame], *shadowRenderingPipeline[2].GetImpl()->As<VulkanShaderPipeline>(), VK_PIPELINE_BIND_POINT_COMPUTE);
        DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, *__computeCommandBuffers[_currentFrame], *shadowRenderingPipeline[2].GetImpl()->As<VulkanShaderPipeline>(), VK_PIPELINE_BIND_POINT_COMPUTE);
        __computeCommandBuffers[_currentFrame]->Dispatch(1, 1, 1);

    if (err = __computeCommandBuffers[_currentFrame]->EndCommandBuffer(); err != vc::Error::Success)
        return err;

    // Synchronization between the image being presented and the image being rendered
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffers[] = {__computeCommandBuffers[_currentFrame]->GetVkCommandBuffer()};
    submitInfo.pCommandBuffers = commandBuffers;
    VkSemaphore signalSemaphores[] = {__computeShadersFinishedSemaphores[_currentFrame].GetSemaphore()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    VkResult result;
    if (result = vkQueueSubmit(QueueManager::GetComputeQueue().GetVkQueue(), 1, &submitInfo, *__computeInFlightFences[_currentFrame].GetFence()); result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || __framebufferChanged) {
        vc::Log::Error("Error: %d", result);
        return vc::Error::Failure;
    }

    return vc::Error::Success;
}

vc::Error VulkanApplication::__DrawFrame()
{
    vc::Error err;
    // Draw image
    vc::ECS::GetECS()->ForEach<vc::Model>([&](vc::Entity entity, vc::Model & model)
    {
        for (auto & mesh : model.GetImpl()->As<VulkanModel>()->GetMeshes())
            mesh.GetMaterial().GetImpl()->ConstAs<VulkanMaterial>()->GetMaterialDescriptorSet();
    });
    // Wait for the fence to be signaled
    vkWaitForFences(LogicalDevice::GetVkDevice(), 1, __graphicsInFlightFences[_currentFrame].GetFence(), VK_TRUE, UINT64_MAX);
    vkWaitForFences(LogicalDevice::GetVkDevice(), 1, __computeInFlightFences[_currentFrame].GetFence(), VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(LogicalDevice::GetVkDevice(), __swapChain.swapChain, UINT64_MAX, __imageAvailableSemaphores[_currentFrame].GetSemaphore(), VK_NULL_HANDLE, &__imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || __framebufferChanged || _gfxSettingsChangeQueued) {
        __framebufferChanged = false;
        _gfxSettingsChangeQueued = false;
        vc::Log::Print("Recreating swap chain");
        _currentFrame = 0;
        return _LoadGfxSettings();
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        vc::Log::Error("Failed to acquire swap chain image");
        return vc::Error::Failure;
    }

    // If we reset before, then it will wait endlessly as no work is done
    vkResetFences(LogicalDevice::GetVkDevice(), 1, __graphicsInFlightFences[_currentFrame].GetFence());
    vkResetFences(LogicalDevice::GetVkDevice(), 1, __computeInFlightFences[_currentFrame].GetFence());
    __graphicsFirstCheckpointCommandBuffers[_currentFrame]->Reset(0);
    __graphicsSecondCheckpointCommandBuffers[_currentFrame]->Reset(0);
    __computeCommandBuffers[_currentFrame]->Reset(0);

    if (err = __ComputeOperations(); err != vc::Error::Success)
        return err;
    if (err = __GraphicsOperations(); err != vc::Error::Success)
        return err;

    _currentFrame = (_currentFrame + 1) % VENOM_MAX_FRAMES_IN_FLIGHT;
    return vc::Error::Success;
}
}
