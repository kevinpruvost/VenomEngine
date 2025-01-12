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

#include "venom/common/plugin/graphics/Light.h"
#include "venom/common/SceneSettings.h"
#include "venom/common/plugin/graphics/GUI.h"
#include "venom/common/plugin/graphics/RenderingPipeline.h"
#include "venom/common/plugin/graphics/RenderTarget.h"

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
        vcm::Vec3 direction;
    };
    CameraData camProps;
    vc::ECS::ForEach<vc::Camera, vc::Transform3D>([&](vc::Entity entity, vc::Camera & camera, vc::Transform3D & transform)
    {
        camProps.viewAndProj[0] = camera.GetViewMatrix();
        camProps.viewAndProj[1] = camera.GetProjectionMatrix();
        camProps.cameraPos = transform.GetPosition();
        camProps.direction = transform.GetForwardVector();
    });

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
    int lightI = 0;
    vc::ECS::ForEach<vc::Light>([&](vc::Entity entity, vc::Light & light)
    {
        lightShaderStructs[lightI++] = light.GetShaderStruct();
    });
    __lightsBuffer.WriteToBuffer(lightShaderStructs.data(), sizeof(vc::LightShaderStruct) * lightShaderStructs.size());
    uint32_t lightCount = lightI;
    __lightCountBuffer.WriteToBuffer(&lightCount, sizeof(uint32_t));
    if (vc::SceneSettings::IsDataDirty()) {
        __sceneSettingsBuffer.WriteToBuffer(vc::SceneSettings::GetCurrentSettingsData(), sizeof(vc::SceneSettingsData));
    }
    if (vc::GraphicsSettings::_IsGfxSettingsDataDirty()) {
        __graphicsSettingsBuffer.WriteToBuffer(vc::GraphicsSettings::GetGfxSettingsDataPtr(), sizeof(vc::GraphicsSettingsData));
    }
}

vc::Error VulkanApplication::__GraphicsOperations()
{
    const auto & renderTargets = vc::RenderTargetImpl::GetAllRenderTargets();

    if (auto err = __graphicsFirstCheckpointCommandBuffers[_currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __graphicsFirstCheckpointCommandBuffers[_currentFrame]->SetViewport(__swapChain.viewport);
        __graphicsFirstCheckpointCommandBuffers[_currentFrame]->SetScissor(__swapChain.scissor);

        // Update Uniform Buffers
        __UpdateUniformBuffers();

        // Draw Skybox
        __skyboxRenderPass.BeginRenderPass(__graphicsFirstCheckpointCommandBuffers[_currentFrame], __imageIndex);
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
        VkSemaphore signalSemaphores[] = {__graphicsSkyboxDoneSemaphores[_currentFrame].GetSemaphore()};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VkResult result;
        vc::Timer theoreticalFpsCounter;
        if (result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, VK_NULL_HANDLE); result != VK_SUCCESS) {
            vc::Log::Error("Failed to submit draw command buffer");
            return vc::Error::Failure;
        }
    }

    // Generate shadow maps
    // if (auto err = __GraphicsShadowMapOperations(); err != vc::Error::Success)
    //     return err;

    if (auto err = __graphicsSecondCheckpointCommandBuffers[_currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __graphicsSecondCheckpointCommandBuffers[_currentFrame]->SetViewport(__swapChain.viewport);
        __graphicsSecondCheckpointCommandBuffers[_currentFrame]->SetScissor(__swapChain.scissor);

        // Draw Shadowed Models (Forward+)
        const auto & shadowRenderingPipeline = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::PBRModel);
        __graphicsRenderPass.BeginRenderPass(__graphicsSecondCheckpointCommandBuffers[_currentFrame], __imageIndex);
            // Draw Shadow Models
            // Lighting Pass
            __graphicsSecondCheckpointCommandBuffers[_currentFrame]->BindPipeline(shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_ModelMatrices, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Camera, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Sampler, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Scene, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Panorama, *__graphicsSecondCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            vc::ECS::GetECS()->ForEach<vc::Model, vc::Transform3D>([&](vc::Entity entity, vc::Model & model, vc::Transform3D & transform)
            {
                int index;
            #ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
                index = transform.GetModelMatrixId();
            #endif
                __graphicsSecondCheckpointCommandBuffers[_currentFrame]->DrawModel(model.GetImpl()->As<VulkanModel>(), index, *shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            });

        __graphicsRenderPass.EndRenderPass(__graphicsSecondCheckpointCommandBuffers[_currentFrame]);

        // Copy to render target if any (for GUI)
        // TODO: It is a test, so should replace
        Image * attachmentImage;
        if (GraphicsSettings::GetActiveSamplesMultisampling() != 1)
            attachmentImage = const_cast<Image *>(*__graphicsRenderPass.GetCurrentFramebuffer()->GetAttachmentImages().rbegin());
        else
            attachmentImage = const_cast<Image *>(*__graphicsRenderPass.GetCurrentFramebuffer()->GetAttachmentImages().begin());
        for (auto & renderTarget : renderTargets) {
            if (renderTarget->GetRenderingPipelineType() != vc::RenderingPipelineType::PBRModel)
                continue;
            if (GraphicsSettings::GetActiveSamplesMultisampling() == 1)
                __graphicsSecondCheckpointCommandBuffers[_currentFrame]->TransitionImageLayout(*attachmentImage, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
            __graphicsSecondCheckpointCommandBuffers[_currentFrame]->TransitionImageLayout(renderTarget->GetTexture()->GetImpl()->ConstAs<VulkanTexture>()->GetImage(), renderTarget->GetTexture()->GetImpl()->As<VulkanTexture>()->GetImage().GetLayout(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            __graphicsSecondCheckpointCommandBuffers[_currentFrame]->CopyImage(*attachmentImage, renderTarget->GetTexture()->GetImpl()->As<VulkanTexture>()->GetImage());
            __graphicsSecondCheckpointCommandBuffers[_currentFrame]->TransitionImageLayout(renderTarget->GetTexture()->GetImpl()->ConstAs<VulkanTexture>()->GetImage(), renderTarget->GetTexture()->GetImpl()->As<VulkanTexture>()->GetImage().GetLayout(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            if (GraphicsSettings::GetActiveSamplesMultisampling() == 1)
                __graphicsSecondCheckpointCommandBuffers[_currentFrame]->TransitionImageLayout(*attachmentImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        }

        // Draw GUI
        __guiRenderPass.BeginRenderPass(__graphicsSecondCheckpointCommandBuffers[_currentFrame], __imageIndex);
            _gui->Render();
        __guiRenderPass.EndRenderPass(__graphicsSecondCheckpointCommandBuffers[_currentFrame]);

    if (auto err = __graphicsSecondCheckpointCommandBuffers[_currentFrame]->EndCommandBuffer(); err != vc::Error::Success)
        return err;

    VkSemaphore signalSemaphores[] = {__renderFinishedSemaphores[_currentFrame].GetSemaphore()};
    {
        // Synchronization between the image being presented and the image being rendered
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        vc::Vector<VkSemaphore> waitSemaphores = {__graphicsSkyboxDoneSemaphores[_currentFrame].GetSemaphore(), __computeShadersFinishedSemaphores[_currentFrame].GetSemaphore()};
        vc::Vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        // Add semaphores from shadow maps generation
        // for (size_t i = 0; i < vc::Light::GetCountOfLights(); ++i) {
        //     waitSemaphores.emplace_back(__shadowMapsFinishedSemaphores[_currentFrame][i].GetSemaphore());
        //     waitStages.emplace_back(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        // }


        submitInfo.waitSemaphoreCount = waitSemaphores.size();
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();
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

vc::Error VulkanApplication::__GraphicsShadowMapOperations()
{
    for (int l = 0; l < vc::Light::GetCountOfLights(); ++l)
    {
        if (auto err = __shadowMapCommandBuffers[_currentFrame][l]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
            return err;

        __shadowMapCommandBuffers[_currentFrame][l]->SetViewport(__swapChain.viewport);
        __shadowMapCommandBuffers[_currentFrame][l]->SetScissor(__swapChain.scissor);

        // Draw Shadowed Models
        __skyboxRenderPass.BeginRenderPass(__shadowMapCommandBuffers[_currentFrame][l], __imageIndex);
        vc::ECS::GetECS()->ForEach<vc::Skybox, vc::RenderingPipeline>([&](vc::Entity entity, vc::Skybox & skybox, vc::RenderingPipeline & pipeline)
        {
            const auto & shaders = pipeline.GetRenderingPipelineCache();
            shaders[0].GetConstImpl()->ConstAs<VulkanShaderPipeline>()->SetDepthWrite(false);
            __shadowMapCommandBuffers[_currentFrame][l]->DrawSkybox(skybox.GetImpl()->As<VulkanSkybox>(), shaders[0].GetConstImpl()->ConstAs<VulkanShaderPipeline>());
        });
        __skyboxRenderPass.EndRenderPass(__shadowMapCommandBuffers[_currentFrame][l]);

        if (auto err = __shadowMapCommandBuffers[_currentFrame][l]->EndCommandBuffer(); err != vc::Error::Success)
            return err;

        // {
        //     // Synchronization between the image being presented and the image being rendered
        //     VkSubmitInfo submitInfo{};
        //     submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        //     VkSemaphore waitSemaphores[] = {__imageAvailableSemaphores[_currentFrame].GetSemaphore()};
        //     VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        //     submitInfo.waitSemaphoreCount = std::size(waitSemaphores);
        //     submitInfo.pWaitSemaphores = waitSemaphores;
        //     submitInfo.pWaitDstStageMask = waitStages;
        //     submitInfo.commandBufferCount = 1;
        //     VkCommandBuffer commandBuffers[] = {__graphicsFirstCheckpointCommandBuffers[_currentFrame]->GetVkCommandBuffer()};
        //     submitInfo.pCommandBuffers = commandBuffers;
        //     VkSemaphore signalSemaphores[] = {__graphicsSkyboxDoneSemaphores[_currentFrame].GetSemaphore()};
        //     submitInfo.signalSemaphoreCount = 1;
        //     submitInfo.pSignalSemaphores = signalSemaphores;
        //
        //     if (VkResult result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, VK_NULL_HANDLE); result != VK_SUCCESS) {
        //         vc::Log::Error("Failed to submit draw command buffer");
        //         return vc::Error::Failure;
        //     }
        // }
    }
    return vc::Error::Success;
}

vc::Error VulkanApplication::__ComputeOperations()
{
    vc::Error err;
    if (err = __computeCommandBuffers[_currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        // Forward+ Light Culling compute
        const auto & forwardPlusRenderingPipeline = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::ForwardPlusLightCulling);
        __computeCommandBuffers[_currentFrame]->BindPipeline(forwardPlusRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
        DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Camera, *__computeCommandBuffers[_currentFrame], forwardPlusRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
        DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light, *__computeCommandBuffers[_currentFrame], forwardPlusRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
        DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Scene, *__computeCommandBuffers[_currentFrame], forwardPlusRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
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
    //vkWaitForFences(LogicalDevice::GetVkDevice(), 1, __shadowMapsInFlightFences[_currentFrame].GetFence(), VK_TRUE, UINT64_MAX);
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
    //vkResetFences(LogicalDevice::GetVkDevice(), 1, __shadowMapsInFlightFences[_currentFrame].GetFence());
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
