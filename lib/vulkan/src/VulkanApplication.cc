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
#include <venom/vulkan/plugin/graphics/Light.h>
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

    // Update Uniform Buffers
    __UpdateUniformBuffers();

    //
    // SKYBOX
    //
    if (auto err = __graphicsFirstCheckpointCommandBuffers[_currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __graphicsFirstCheckpointCommandBuffers[_currentFrame]->SetViewport(__swapChain.viewport);
        __graphicsFirstCheckpointCommandBuffers[_currentFrame]->SetScissor(__swapChain.scissor);


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
        VkSemaphore waitSemaphores[] = {__imageAvailableSemaphores[_currentFrame].GetVkSemaphore()};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = std::size(waitSemaphores);
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        VkCommandBuffer commandBuffers[] = {__graphicsFirstCheckpointCommandBuffers[_currentFrame]->GetVkCommandBuffer()};
        submitInfo.pCommandBuffers = commandBuffers;
        VkSemaphore signalSemaphores[] = {__graphicsSkyboxDoneSemaphores[_currentFrame].GetVkSemaphore()};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VkResult result;
        vc::Timer theoreticalFpsCounter;
        if (result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, VK_NULL_HANDLE); result != VK_SUCCESS) {
            vc::Log::Error("Failed to submit draw command buffer");
            return vc::Error::Failure;
        }
    }

    //
    // SHADOW MAPS (Parallel to Skybox & Forward+)
    //
    if (auto err = __GraphicsShadowMapOperations(); err != vc::Error::Success)
        return err;

    //
    // SCENE AND GUI (Must wait for Skybox, Shadow Maps and Forward+)
    //
    if (auto err = __graphicsSceneCheckpointCommandBuffers[_currentFrame]->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

        __graphicsSceneCheckpointCommandBuffers[_currentFrame]->SetViewport(__swapChain.viewport);
        __graphicsSceneCheckpointCommandBuffers[_currentFrame]->SetScissor(__swapChain.scissor);

        // Draw Lit Models (Forward+)
        const auto & shadowRenderingPipeline = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::PBRModel);
        __graphicsRenderPass.BeginRenderPass(__graphicsSceneCheckpointCommandBuffers[_currentFrame], __imageIndex);
            // Draw Models
            // Lighting Pass
            __graphicsSceneCheckpointCommandBuffers[_currentFrame]->BindPipeline(shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_ModelMatrices, *__graphicsSceneCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Camera, *__graphicsSceneCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Sampler, *__graphicsSceneCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Scene, *__graphicsSceneCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light, *__graphicsSceneCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Panorama, *__graphicsSceneCheckpointCommandBuffers[_currentFrame], shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            vc::ECS::GetECS()->ForEach<vc::Model, vc::Transform3D>([&](vc::Entity entity, vc::Model & model, vc::Transform3D & transform)
            {
                int index;
            #ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
                index = transform.GetModelMatrixId();
            #endif
                __graphicsSceneCheckpointCommandBuffers[_currentFrame]->DrawModel(model.GetImpl()->As<VulkanModel>(), index, *shadowRenderingPipeline[0].GetImpl()->As<VulkanShaderPipeline>());
            });

        __graphicsRenderPass.EndRenderPass(__graphicsSceneCheckpointCommandBuffers[_currentFrame]);

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
                __graphicsSceneCheckpointCommandBuffers[_currentFrame]->TransitionImageLayout(*attachmentImage, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
            __graphicsSceneCheckpointCommandBuffers[_currentFrame]->TransitionImageLayout(renderTarget->GetTexture()->GetImpl()->ConstAs<VulkanTexture>()->GetImage(), renderTarget->GetTexture()->GetImpl()->As<VulkanTexture>()->GetImage().GetLayout(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            __graphicsSceneCheckpointCommandBuffers[_currentFrame]->CopyImage(*attachmentImage, renderTarget->GetTexture()->GetImpl()->As<VulkanTexture>()->GetImage());
            __graphicsSceneCheckpointCommandBuffers[_currentFrame]->TransitionImageLayout(renderTarget->GetTexture()->GetImpl()->ConstAs<VulkanTexture>()->GetImage(), renderTarget->GetTexture()->GetImpl()->As<VulkanTexture>()->GetImage().GetLayout(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            if (GraphicsSettings::GetActiveSamplesMultisampling() == 1)
                __graphicsSceneCheckpointCommandBuffers[_currentFrame]->TransitionImageLayout(*attachmentImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        }

        // Draw GUI
        __guiRenderPass.BeginRenderPass(__graphicsSceneCheckpointCommandBuffers[_currentFrame], __imageIndex);
            _gui->Render();
        __guiRenderPass.EndRenderPass(__graphicsSceneCheckpointCommandBuffers[_currentFrame]);

    if (auto err = __graphicsSceneCheckpointCommandBuffers[_currentFrame]->EndCommandBuffer(); err != vc::Error::Success)
        return err;

    VkSemaphore signalSemaphores[] = {__renderFinishedSemaphores[_currentFrame].GetVkSemaphore()};
    {
        // Synchronization between the image being presented and the image being rendered
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        vc::Vector<VkSemaphore> waitSemaphores = {__graphicsSkyboxDoneSemaphores[_currentFrame].GetVkSemaphore(), __computeShadersFinishedSemaphores[_currentFrame].GetVkSemaphore()};
        vc::Vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        // Add semaphores from shadow maps generation
        for (size_t i = 0; i < vc::Light::GetCountOfLightsOfType(vc::LightType::Directional); ++i) {
            for (size_t j = 0; j < std::size(__shadowMapsDirectionalFinishedSemaphores[_currentFrame][i]); ++j) {
                waitSemaphores.emplace_back(__shadowMapsDirectionalFinishedSemaphores[_currentFrame][i][j].GetVkSemaphore());
                waitStages.emplace_back(VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);
            }
        }
        // for (size_t i = 0; i < vc::Light::GetCountOfLightsOfType(vc::LightType::Point); ++i) {
        //     waitSemaphores.emplace_back(__shadowMapsPointFinishedSemaphores[_currentFrame][i].GetSemaphore());
        //     waitStages.emplace_back(VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);
        // }
        // for (size_t i = 0; i < vc::Light::GetCountOfLightsOfType(vc::LightType::Spot); ++i) {
        //     waitSemaphores.emplace_back(__shadowMapsSpotFinishedSemaphores[_currentFrame][i].GetSemaphore());
        //     waitStages.emplace_back(VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);
        // }

        submitInfo.waitSemaphoreCount = waitSemaphores.size();
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();
        submitInfo.commandBufferCount = 1;
        VkCommandBuffer commandBuffers[] = {__graphicsSceneCheckpointCommandBuffers[_currentFrame]->GetVkCommandBuffer()};
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
    const auto & shadowRenderingPipeline = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::CascadedShadowMapping);
    const auto & lights = vc::Light::GetLights();
    vc::Camera * camera = vc::Camera::GetMainCamera();

    for (int l = 0; l < lights.size(); ++l)
    {
        const int shadowMapIndex = lights[l]->GetImpl()->As<vc::LightImpl>()->GetLightIndexPerType();
        switch (lights[l]->GetLightType())
        {
            case vc::LightType::Directional: {
                for (int cascade = 0; cascade < VENOM_CSM_TOTAL_CASCADES; ++cascade)
                {
                    const auto & lightConstants = lights[l]->GetImpl()->As<vc::LightImpl>()->GetShadowMapConstantsStruct(cascade, 0, camera);
                    if (auto err = __GraphicsShadowMapOperationPerLight(lights[l], l, lightConstants,
                        &lights[l]->GetImpl()->As<VulkanLight>()->GetShadowMapFramebuffers(_currentFrame, cascade)[0],
                        &__shadowMapsDirectionalFinishedSemaphores[_currentFrame][l][cascade], __shadowMapDirectionalCommandBuffers[_currentFrame][l][cascade],
                        &shadowRenderingPipeline[0]); err != vc::Error::Success)
                        return err;
                }
                break;
            }
            case vc::LightType::Point:
                break;
            case vc::LightType::Spot:
                break;
            default:
                break;
        }
    }
    return vc::Error::Success;
}

vc::Error VulkanApplication::__GraphicsShadowMapOperationPerLight(const vc::Light* light, const int lightIndex, const vc::LightCascadedShadowMapConstantsStruct & lightPushConstants,
    const Framebuffer * const framebuffer, const Semaphore * const semaphore, CommandBuffer * const commandBuffer, const vc::ShaderPipeline * const shaderPipeline)
{
    if (vc::Error err = commandBuffer->BeginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); err != vc::Error::Success)
        return err;

    VkExtent2D extent = framebuffer->GetFramebufferExtent();
    VkViewport viewport{};
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    commandBuffer->SetViewport(viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;
    commandBuffer->SetScissor(scissor);

    // Draw Shadowed Models
    // Lighting Pass
    __shadowMapRenderPass.BeginRenderPassCustomFramebuffer(commandBuffer, framebuffer);

    commandBuffer->BindPipeline(shaderPipeline->GetImpl()->As<VulkanShaderPipeline>());
    commandBuffer->PushConstants(shaderPipeline, VK_SHADER_STAGE_VERTEX_BIT, &lightPushConstants);

    // Push constants for each light
    DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_ModelMatrices, *commandBuffer, shaderPipeline->GetImpl()->As<VulkanShaderPipeline>());
    DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light, *commandBuffer, shaderPipeline->GetImpl()->As<VulkanShaderPipeline>());
    DescriptorPool::GetPool()->BindDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Camera, *commandBuffer, shaderPipeline->GetImpl()->As<VulkanShaderPipeline>());
    vc::ECS::GetECS()->ForEach<vc::Model, vc::Transform3D>([&](vc::Entity entity, vc::Model & model, vc::Transform3D & transform)
    {
        int index;
    #ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
        index = transform.GetModelMatrixId();
    #endif
        commandBuffer->DrawModel(model.GetImpl()->As<VulkanModel>(), index, *shaderPipeline->GetImpl()->As<VulkanShaderPipeline>());
    });

    __shadowMapRenderPass.EndRenderPass(commandBuffer);

    if (auto err = commandBuffer->EndCommandBuffer(); err != vc::Error::Success)
        return err;

    {
        // Synchronization between the image being presented and the image being rendered
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = nullptr;
        submitInfo.pWaitDstStageMask = nullptr;
        submitInfo.commandBufferCount = 1;
        VkCommandBuffer commandBuffers[] = {commandBuffer->GetVkCommandBuffer()};
        submitInfo.pCommandBuffers = commandBuffers;
        VkSemaphore signalSemaphores[] = {semaphore->GetVkSemaphore()};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (VkResult result = vkQueueSubmit(__graphicsQueue.GetVkQueue(), 1, &submitInfo, VK_NULL_HANDLE); result != VK_SUCCESS) {
            vc::Log::Error("Failed to submit draw command buffer");
            return vc::Error::Failure;
        }
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
    VkSemaphore signalSemaphores[] = {__computeShadersFinishedSemaphores[_currentFrame].GetVkSemaphore()};
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

    VkResult result = vkAcquireNextImageKHR(LogicalDevice::GetVkDevice(), __swapChain.swapChain, UINT64_MAX, __imageAvailableSemaphores[_currentFrame].GetVkSemaphore(), VK_NULL_HANDLE, &__imageIndex);
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
    __graphicsSceneCheckpointCommandBuffers[_currentFrame]->Reset(0);
    __computeCommandBuffers[_currentFrame]->Reset(0);
    for (size_t i = 0; i < vc::Light::GetCountOfLightsOfType(vc::LightType::Directional); ++i) {
        for (size_t j = 0; j < std::size(__shadowMapDirectionalCommandBuffers[_currentFrame][i]); ++j) {
            __shadowMapDirectionalCommandBuffers[_currentFrame][i][j]->Reset(0);
        }
    }
    for (size_t i = 0; i < vc::Light::GetCountOfLightsOfType(vc::LightType::Point); ++i) {
        for (size_t j = 0; j < std::size(__shadowMapPointCommandBuffers[_currentFrame][i]); ++j) {
            __shadowMapPointCommandBuffers[_currentFrame][i][j]->Reset(0);
        }
    }
    for (size_t i = 0; i < vc::Light::GetCountOfLightsOfType(vc::LightType::Spot); ++i) {
        __shadowMapSpotCommandBuffers[_currentFrame][i]->Reset(0);
    }

    if (err = __ComputeOperations(); err != vc::Error::Success)
        return err;
    if (err = __GraphicsOperations(); err != vc::Error::Success)
        return err;

    _currentFrame = (_currentFrame + 1) % VENOM_MAX_FRAMES_IN_FLIGHT;
    return vc::Error::Success;
}
}
