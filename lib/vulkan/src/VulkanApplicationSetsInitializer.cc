///
/// Project: VenomEngineWorkspace
/// @file VulkanApplicationSetsInitializer.cc
/// @date Oct, 27 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/VulkanApplication.h>

#include "venom/common/plugin/graphics/Light.h"
#include "venom/common/SceneSettings.h"

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
        if (err = __graphicsSkyboxDoneSemaphores[i].InitSemaphore(); err != vc::Error::Success)
            return err;
        if (err = __graphicsInFlightFences[i].InitFence(VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT); err != vc::Error::Success)
            return err;
        if (err = __computeInFlightFences[i].InitFence(VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT); err != vc::Error::Success)
            return err;
        if (err = __shadowMapsInFlightFences[i].InitFence(VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT); err != vc::Error::Success)
            return err;
        // Shadow Maps Semaphores
        for (int j = 0; j < VENOM_MAX_LIGHTS; ++j) {
            if (err = __shadowMapsFinishedSemaphores[i][j].InitSemaphore(); err != vc::Error::Success)
                return err;
        }
    }

    // Create Uniform Buffers
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __objectStorageBuffers[i].Init(VENOM_MAX_ENTITIES * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
        if (err = __cameraUniformBuffers[i].Init(2 * sizeof(vcm::Mat4) + 2 * sizeof(vcm::Vec3)); err != vc::Error::Success)
            return err;
    }

    // Descriptor Set Layout
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_ModelMatrices)
        .AddBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_Camera)
        .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL);
    DescriptorSetLayout & texturesLayout = DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_Textures)
        .SetBindingFlags(VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT);
    if (vc::ShaderResourceTable::UsingLargeBindlessTextures()) {
        texturesLayout
            .AddBinding(0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, vc::ShaderResourceTable::GetMaxTextures(), VK_SHADER_STAGE_FRAGMENT_BIT)
            // Using uniform buffer dynamic for texture IDs (4.1)
            .AddBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    } else {
        texturesLayout
            .AddBinding(0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_MAX_DYNAMIC_TEXTURES, VK_SHADER_STAGE_FRAGMENT_BIT)
            .SetMaxSets(VENOM_MAX_ENTITIES);
    }
    // Enabling update after bind pool for textures, dynamic or bindless
    texturesLayout
        .SetBindless()
        .SetFlags(VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT);
    // Sampler
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_Sampler)
        .AddBinding(0, VK_DESCRIPTOR_TYPE_SAMPLER, 1, VK_SHADER_STAGE_ALL);
    // Material properties (4.0)
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_Material)
        .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
        // BRDF LUT
        .AddBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_ALL)
        // Irradiance Map / Radiance Map for calculations
        .AddBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_ALL)
        // Roughness for calculations
        .AddBinding(3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL)
        .SetMaxSets(VENOM_MAX_ENTITIES);

    // Scene settings & Graphics Settings
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_Scene)
        .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL)
        .AddBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL);

    // Panorama
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_Panorama)
        .AddBinding(0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_ALL)
        .AddBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
        // Irradiance
        .AddBinding(2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
        // Radiance
        .AddBinding(3, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
    ;

    // Lights
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light)
        // Light Structures
        .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL)
        // Light count
        .AddBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL)
        // Forward Plus
        .AddBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL)
        // Shadow maps
        // Indices
        .AddBinding(3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL)
        // Directional Shadow Maps Arrays
        .AddBinding(4, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_CSM_TOTAL_CASCADES, VK_SHADER_STAGE_ALL)
        // Point Shadow Maps Arrays
        .AddBinding(5, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_CSM_TOTAL_CASCADES, VK_SHADER_STAGE_ALL)
        // Spot Shadow Maps Arrays
        .AddBinding(6, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_CSM_TOTAL_CASCADES, VK_SHADER_STAGE_ALL);


    // GUI needs VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT & VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER for fonts
    DescriptorPool::GetPool()->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VENOM_MAX_FRAMES_IN_FLIGHT * 200);

    // Makes the pool able to allocate descriptor sets that can be updated after binding
    if (DescriptorPool::GetPool()->Create(VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT | VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT) != vc::Error::Success)
        return vc::Error::Failure;

    // Target Luminance
    if (err = __sceneSettingsBuffer.Init(sizeof(vc::SceneSettingsData)); err != vc::Error::Success)
        return err;

    // BRDF LUT
    __brdfLutTexture.CreateReadWriteTexture(1024, 1024, vc::ShaderVertexFormat::Vec2, 1);
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Material).GroupUpdateTexture(__brdfLutTexture, 1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, 0);

    // Scene Settings
    __sceneSettingsBuffer.WriteToBuffer(vc::SceneSettings::GetCurrentSettingsData(), sizeof(vc::SceneSettingsData));
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Scene).GroupUpdateBuffer(__sceneSettingsBuffer, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);

    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        // Model & PBR info
        DescriptorPool::GetPool()->GetDescriptorSets(0).GroupUpdateBufferPerFrame(i, __objectStorageBuffers[i], 0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
        // View & Projection
        DescriptorPool::GetPool()->GetDescriptorSets(1).GroupUpdateBufferPerFrame(i, __cameraUniformBuffers[i], 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);
    }

    // Screen Props
    if (err = __graphicsSettingsBuffer.Init(sizeof(vc::GraphicsSettingsData)); err != vc::Error::Success)
        return err;
    __graphicsSettingsBuffer.WriteToBuffer(vc::GraphicsSettings::GetGfxSettingsDataPtr(), sizeof(vc::GraphicsSettingsData));
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Scene).GroupUpdateBuffer(__graphicsSettingsBuffer, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);

    // Lights
    if (err = __lightsBuffer.Init(VENOM_MAX_LIGHTS * sizeof(vc::LightShaderStruct)); err != vc::Error::Success)
        return err;
    if (err = __lightCountBuffer.Init(sizeof(uint32_t)); err != vc::Error::Success)
        return err;
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateBuffer(__lightsBuffer, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateBuffer(__lightCountBuffer, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);

    // Forward Plus
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __forwardPlusPropsBuffer[i].Init(32 * 32 * VENOM_NUM_FORWARD_PLUS_INTS * sizeof(int)); err != vc::Error::Success)
            return err;
        DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateBuffer(__forwardPlusPropsBuffer[i], 0, 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
    }

    // Shadow map indices
    if (err = __shadowMapIndices.Init(VENOM_MAX_LIGHTS * sizeof(int)); err != vc::Error::Success)
        return err;

    auto size = vc::VenomSettings::GetTotalShadowMapGPUSize();
    vc::Log::Print("Total shadow map GPU size: %d MB", size / 1024 / 1024);

    // Shadow Maps
    for (int x = 0; x < VENOM_MAX_FRAMES_IN_FLIGHT; ++x) {
        for (int i = 0; i < VENOM_CSM_TOTAL_CASCADES; ++i) {
            if (err = __shadowMapsDirectional[x][i].CreateShadowMaps(VENOM_CSM_DIRECTIONAL_DIMENSION >> i, VENOM_CSM_MAX_DIRECTIONAL_LIGHTS); err != vc::Error::Success)
                return err;
            if (err = __shadowMapsPoint[x][i].CreateShadowMaps(VENOM_CSM_POINT_DIMENSION >> i, VENOM_CSM_MAX_POINT_LIGHTS * 6); err != vc::Error::Success)
                return err;
            if (err = __shadowMapsSpot[x][i].CreateShadowMaps(VENOM_CSM_SPOT_DIMENSION >> i, VENOM_CSM_MAX_SPOT_LIGHTS); err != vc::Error::Success)
                return err;
            for (int j = 0; j < VENOM_CSM_MAX_DIRECTIONAL_LIGHTS; ++j)
                __shadowMapDirectionalImageViews[x][i][j] = &__shadowMapsDirectional[x][i].GetImpl()->As<VulkanTexture>()->GetImageView(j);
            for (int j = 0; j < VENOM_CSM_MAX_POINT_LIGHTS; ++j) {
                for (int k = 0; k < 6; ++k)
                    __shadowMapPointImageViews[x][i][j * 6 + k] = &__shadowMapsPoint[x][i].GetImpl()->As<VulkanTexture>()->GetImageView(j * 6 + k);
            }
            for (int j = 0; j < VENOM_CSM_MAX_SPOT_LIGHTS; ++j)
                __shadowMapSpotImageViews[x][i][j] = &__shadowMapsSpot[x][i].GetImpl()->As<VulkanTexture>()->GetImageView(j);
            DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateTexture(__shadowMapsDirectional[x][i], 4, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, i);
            DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateTexture(__shadowMapsPoint[x][i], 5, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, i);
            DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateTexture(__shadowMapsSpot[x][i], 6, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, i);
        }
    }

    // Radiance roughness
    if (err = __radianceRoughness.Init(sizeof(float)); err != vc::Error::Success)
        return err;
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Material).GroupUpdateBuffer(__radianceRoughness, 0, 3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);

    // Screen Quad for Forward+
    static vc::Array<vcm::Vec4, 6> screenQuadVertices = {
        vcm::Vec4{-1.0f, -1.0f, 0.0f, 1.0f},
        vcm::Vec4{1.0f, -1.0f, 0.0f, 1.0f},
        vcm::Vec4{1.0f, 1.0f, 0.0f, 1.0f},
        vcm::Vec4{-1.0f, 1.0f, 0.0f, 1.0f},
        vcm::Vec4{-1.0f, -1.0f, 0.0f, 1.0f},
        vcm::Vec4{1.0f, 1.0f, 0.0f, 1.0f},
    };
    if (err = __screenQuadVertexBuffer.Init(screenQuadVertices.size(), sizeof(vcm::Vec4), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, screenQuadVertices.data()); err != vc::Error::Success)
        return err;

    return err;
}
}
}
