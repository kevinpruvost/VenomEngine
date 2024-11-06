///
/// Project: VenomEngineWorkspace
/// @file VulkanApplicationSetsInitializer.cc
/// @date Oct, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>

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
        if (err = __inFlightFences[i].InitFence(VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT); err != vc::Error::Success)
            return err;
    }

    // Create Uniform Buffers
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __objectStorageBuffers[i].Init(VENOM_MAX_ENTITIES * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
        if (err = __cameraUniformBuffers[i].Init(2 * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
    }

    // Descriptor Set Layout
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_MODEL_MATRICES, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_CAMERA, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
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
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);

    // Panorama
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_PANORAMA, 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    DescriptorPool::GetPool()->AddDescriptorSetLayoutBinding(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_PANORAMA, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);

    // Makes the pool able to allocate descriptor sets that can be updated after binding
    if (DescriptorPool::GetPool()->Create(VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT) != vc::Error::Success)
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

    return err;
}
}
}