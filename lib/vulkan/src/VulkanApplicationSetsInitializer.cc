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
        for (int j = 0; j < std::size(__shadowMapsDirectionalFinishedSemaphores[i]); ++j) {
            for (int k = 0; k < std::size(__shadowMapsDirectionalFinishedSemaphores[i][j]); ++k) {
                if (err = __shadowMapsDirectionalFinishedSemaphores[i][j][k].InitSemaphore(); err != vc::Error::Success)
                    return err;
            }
        }
        for (int j = 0; j < std::size(__shadowMapsPointFinishedSemaphores[i]); ++j) {
            for (int k = 0; k < std::size(__shadowMapsPointFinishedSemaphores[i][j]); ++k) {
                if (err = __shadowMapsPointFinishedSemaphores[i][j][k].InitSemaphore(); err != vc::Error::Success)
                    return err;
            }
        }
        for (int j = 0; j < std::size(__shadowMapsSpotFinishedSemaphores[i]); ++j) {
            if (err = __shadowMapsSpotFinishedSemaphores[i][j].InitSemaphore(); err != vc::Error::Success)
                return err;
        }
    }

    // Create Uniform Buffers
    for (int i = 0; i < VENOM_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (err = __modelMatricesStorageBuffers[i].Init(VENOM_MAX_ENTITIES * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
        if (err = __cameraUniformBuffers[i].Init(2 * sizeof(vcm::Mat4) + 2 * sizeof(vcm::Vec3)); err != vc::Error::Success)
            return err;
    }

    // Descriptor Set Layout
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_ModelMatrices)
        .AddBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_Camera)
        .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL)
        // Sampler
        .AddBinding(1, VK_DESCRIPTOR_TYPE_SAMPLER, 1, VK_SHADER_STAGE_ALL);
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
        .AddBinding(3, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL)
        // // Directional Shadow Maps Arrays
        // .AddBinding(4, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_CSM_TOTAL_CASCADES, VK_SHADER_STAGE_ALL)
        // // Point Shadow Maps Arrays (6 faces)
        // .AddBinding(5, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 6, VK_SHADER_STAGE_ALL)
        // // Spot Shadow Maps Arrays
        // .AddBinding(6, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_ALL)
        // Directional Shadow Maps Light Space Matrices
        .AddBinding(7, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VENOM_CSM_TOTAL_CASCADES, VK_SHADER_STAGE_ALL)
        // Point Shadow Maps Light Space Matrices
        .AddBinding(8, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 6, VK_SHADER_STAGE_ALL)
        // Spot Shadow Maps Light Space Matrices
        .AddBinding(9, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL)
        .AddBinding(10, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT);

    // Individual Lights
    DescriptorPool::GetPool()->GetOrCreateDescriptorSetLayout(vc::ShaderResourceTable::SetsIndex::SetsIndex_LightIndividual)
        // Max images for each light is 6 (Point), Directional has equal to the number of cascades (3) and Spot has 1
        .AddBinding(0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 6, VK_SHADER_STAGE_ALL)
        .SetMaxSets(VENOM_MAX_LIGHTS);

    // GUI needs VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT & VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER for fonts
    DescriptorPool::GetPool()->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VENOM_MAX_FRAMES_IN_FLIGHT * 200);

    // Allocate for lights
    // /// Directional lights
    // DescriptorPool::GetPool()->AddPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_MAX_FRAMES_IN_FLIGHT * VENOM_CSM_TOTAL_CASCADES * VENOM_CSM_MAX_DIRECTIONAL_LIGHTS);
    // /// Point lights
    // DescriptorPool::GetPool()->AddPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_MAX_FRAMES_IN_FLIGHT * VENOM_CSM_MAX_POINT_LIGHTS * 6);
    // /// Spot lights
    // DescriptorPool::GetPool()->AddPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VENOM_MAX_FRAMES_IN_FLIGHT * VENOM_CSM_MAX_SPOT_LIGHTS);

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
        DescriptorPool::GetPool()->GetDescriptorSets(0).GroupUpdateBufferPerFrame(i, __modelMatricesStorageBuffers[i], 0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
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

    auto size = vc::VenomSettings::GetTotalShadowMapGPUSize();
    vc::Log::Print("Total shadow map GPU size: %d MB", size / 1024 / 1024);

    // Shadow Maps
    for (int x = 0; x < VENOM_MAX_FRAMES_IN_FLIGHT; ++x) {
        // Indices
        if (err = __shadowMapsIndicesBuffers[x].Init(VENOM_MAX_LIGHTS * sizeof(int)); err != vc::Error::Success)
            return err;
        DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateBuffer(__shadowMapsIndicesBuffers[x], 0, 3, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
        // Shadow Maps of each light type
        for (int i = 0; i < VENOM_CSM_TOTAL_CASCADES; ++i) {
            for (int k = 0; k < VENOM_CSM_MAX_DIRECTIONAL_LIGHTS; ++k) {
                if (err = __shadowMapsDirectional[x][i][k].CreateShadowMaps(VENOM_CSM_DIRECTIONAL_DIMENSION >> i); err != vc::Error::Success)
                    return err;
                __shadowMapDirectionalImageViews[x][i][k] = &__shadowMapsDirectional[x][i][k].GetImpl()->As<VulkanTexture>()->GetImageView();
                // DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateTexturePerFrame(x, __shadowMapsDirectional[x][i][k], 4, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, k + i * VENOM_CSM_MAX_DIRECTIONAL_LIGHTS);
            }
            for (int k = 0; k < VENOM_CSM_MAX_POINT_LIGHTS; ++k) {
                for (int j = 0; j < 6; ++j) {
                    if (err = __shadowMapsPoint[x][i][k * 6 + j].CreateShadowMaps(VENOM_CSM_POINT_DIMENSION >> i); err != vc::Error::Success)
                        return err;
                    __shadowMapPointImageViews[x][i][k * 6 + j] = &__shadowMapsPoint[x][i][k * 6 + j].GetImpl()->As<VulkanTexture>()->GetImageView();
                    // DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateTexturePerFrame(x, __shadowMapsPoint[x][i][k * 6 + j], 5, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, j + k * 6 + i * VENOM_CSM_MAX_POINT_LIGHTS * 6);
                }
            }
            for (int k = 0; k < VENOM_CSM_MAX_SPOT_LIGHTS; ++k) {
                if (err = __shadowMapsSpot[x][i][k].CreateShadowMaps(VENOM_CSM_SPOT_DIMENSION >> i); err != vc::Error::Success)
                    return err;
                __shadowMapSpotImageViews[x][i][k] = &__shadowMapsSpot[x][i][k].GetImpl()->As<VulkanTexture>()->GetImageView();
                // DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateTexturePerFrame(x, __shadowMapsSpot[x][i][k], 6, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, k + i * VENOM_CSM_MAX_SPOT_LIGHTS);
            }
        }

        // Shadow Map Uniform buffers
        if (err = __shadowMapDirectionalLightSpaceMatricesBuffers[x].Init(VENOM_MAX_DIRECTIONAL_LIGHTS * VENOM_CSM_TOTAL_CASCADES * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
        DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateBufferPerFrame(x, __shadowMapDirectionalLightSpaceMatricesBuffers[x], 0, 7, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
        if (err = __shadowMapPointLightSpaceMatricesBuffers[x].Init(VENOM_MAX_POINT_LIGHTS * 6 * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
        DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateBufferPerFrame(x, __shadowMapPointLightSpaceMatricesBuffers[x], 0, 8, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
        if (err = __shadowMapSpotLightSpaceMatricesBuffers[x].Init(VENOM_MAX_SPOT_LIGHTS * sizeof(vcm::Mat4)); err != vc::Error::Success)
            return err;
        DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Light).GroupUpdateBufferPerFrame(x, __shadowMapSpotLightSpaceMatricesBuffers[x], 0, 9, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 0);
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
