///
/// Project: VenomEngineWorkspace
/// @file VulkanApplicationSetsInitializer.cc
/// @date Oct, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>

#include "venom/common/Light.h"

namespace venom
{
namespace vulkan
{
vc::Error VulkanApplication::__InitializeSets()
{
    vc::Error err;
    // Create Semaphores & Fences
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __imageAvailableSemaphores[i].InitSemaphore(); err != vc::Error::Success)
            return err;
        if (err = __renderFinishedSemaphores[i].InitSemaphore(); err != vc::Error::Success)
            return err;
        if (err = __computeShadersFinishedSemaphores[i].InitSemaphore(); err != vc::Error::Success)
            return err;
        if (err = __graphicsFirstCheckpointSemaphores[i].InitSemaphore(); err != vc::Error::Success)
            return err;
        if (err = __graphicsInFlightFences[i].InitFence(VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT); err != vc::Error::Success)
            return err;
        if (err = __computeInFlightFences[i].InitFence(VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT); err != vc::Error::Success)
            return err;
    }

    // Create Uniform Buffers
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __objectStorageBuffers[i].Init(VENOM_MAX_ENTITIES * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
        if (err = __cameraUniformBuffers[i].Init(2 * sizeof(vcm::Mat4) + sizeof(vcm::Vec3)); err != vc::Error::Success)
            return err;
    }

    // Descriptor Set Layout
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_MODEL_MATRICES, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_CAMERA, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL);
    if (vc::ShaderResourceTable::UsingLargeBindlessTextures()) {
        DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_TEXTURES, 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, vc::ShaderResourceTable::GetMaxTextures(), VK_SHADER_STAGE_FRAGMENT_BIT);
        // Using uniform buffer dynamic for texture IDs (4.1)
        DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(5, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    } else {
        DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_TEXTURES, 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_MAX_DYNAMIC_TEXTURES, VK_SHADER_STAGE_FRAGMENT_BIT);
        DescriptorPool::GetPool()->SetDescriptorSetLayoutMaxSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_TEXTURES, VENOM_MAX_ENTITIES);
    }
    // Enabling update after bind pool for textures, dynamic or bindless
    DescriptorPool::GetPool()->SetDescriptorSetLayoutBindless(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_TEXTURES);
    DescriptorPool::GetPool()->SetDescriptorSetLayoutCreateFlags(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_TEXTURES, VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT);
    // Sampler
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SAMPLER, 0, VK_DESCRIPTOR_TYPE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    // Material properties (4.0)
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_MATERIAL, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    DescriptorPool::GetPool()->SetDescriptorSetLayoutMaxSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_MATERIAL, VENOM_MAX_ENTITIES);

    // Scene
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL);

    // Panorama
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_PANORAMA, 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_PANORAMA, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);

    // Lights
    // Light structures
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL);
    // Light count
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL);

    // Screen Props
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL);

    // Forward Plus
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL);

    // Input Attachments
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, 3, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, 4, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, 5, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, 6, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT, 7, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT);


    // GUI needs VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT & VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER for fonts
    DescriptorPool::GetPool()->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VENOM_MAX_FRAMES_IN_FLIGHT * 100);

    // Makes the pool able to allocate descriptor sets that can be updated after binding
    if (DescriptorPool::GetPool()->Create(VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT | VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT) != vc::Error::Success)
        return vc::Error::Failure;

    // Target Luminance
    if (err = __targetLuminanceBuffer.Init(sizeof(float)); err != vc::Error::Success)
        return err;
    float targetLuminance = 100.0f;
    __targetLuminanceBuffer.WriteToBuffer(&targetLuminance, sizeof(float));
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE).GroupUpdateBuffer(__targetLuminanceBuffer, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);

    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        // Model & PBR info
        DescriptorPool::GetPool()->GetDescriptorSets(0).GroupUpdateBufferPerFrame(i, __objectStorageBuffers[i], 0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
        // View & Projection
        DescriptorPool::GetPool()->GetDescriptorSets(1).GroupUpdateBufferPerFrame(i, __cameraUniformBuffers[i], 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);
    }

    // Screen Props
    if (err = __screenPropsBuffer.Init(sizeof(vcm::Vec2)); err != vc::Error::Success)
        return err;
    vcm::Vec2 screenProps = {__swapChain.extent.width, __swapChain.extent.height};
    __screenPropsBuffer.WriteToBuffer(&screenProps, sizeof(vcm::Vec2));
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE).GroupUpdateBuffer(__screenPropsBuffer, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);

    // Lights
    if (err = __lightsBuffer.Init(VENOM_MAX_LIGHTS * sizeof(vc::LightShaderStruct)); err != vc::Error::Success)
        return err;
    if (err = __lightCountBuffer.Init(sizeof(uint32_t)); err != vc::Error::Success)
        return err;
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT).GroupUpdateBuffer(__lightsBuffer, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT).GroupUpdateBuffer(__lightCountBuffer, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);

    // Forward Plus
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __forwardPlusPropsBuffer[i].Init(32 * 32 * sizeof(int)); err != vc::Error::Success)
            return err;
        DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_LIGHT).GroupUpdateBuffer(__forwardPlusPropsBuffer[i], 0, 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
    }

    // Screen Quad for Forward+
    static vc::Array<vcm::Vec4, 6> screenQuadVertices = {
        vcm::Vec4{-1.0f, -1.0f, 0.0f, 1.0f},
        vcm::Vec4{1.0f, -1.0f, 0.0f, 1.0f},
        vcm::Vec4{1.0f, 1.0f, 0.0f, 1.0f},
        vcm::Vec4{-1.0f, 1.0f, 0.0f, 1.0f},
        vcm::Vec4{-1.0f, -1.0f, 0.0f, 1.0f},
        vcm::Vec4{1.0f, 1.0f, 0.0f, 1.0f},
    };
    if (err = __screenQuadVertexBuffer.Init(6, sizeof(vcm::Vec4), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, screenQuadVertices.data()); err != vc::Error::Success)
        return err;

    return err;
}
}
}
