///
/// Project: VenomEngineWorkspace
/// @file Skybox.cc
/// @date Oct, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/CommandPoolManager.h>
#include <venom/vulkan/plugin/graphics/Skybox.h>

#include "venom/common/plugin/graphics/RenderingPipeline.h"

namespace venom
{
namespace vulkan
{
VulkanSkybox::VulkanSkybox()
    : __descriptorSet(DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Panorama).AllocateSet())
{
    // Init Vertex Buffer
    const vcm::Vec3 vertices[] = {
        vcm::Vec3(-1.0f,  1.0f, 0.0f),
        vcm::Vec3(-1.0f, -1.0f, 0.0f),
        vcm::Vec3( 1.0f, -1.0f, 0.0f),

        vcm::Vec3( 1.0f,  1.0f, 0.0f),
        vcm::Vec3(-1.0f,  1.0f, 0.0f),
        vcm::Vec3( 1.0f, -1.0f, 0.0f),
    };
    if (__vertexBuffer.Init(6, sizeof(vcm::Vec3), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertices) != vc::Error::Success)
    {
        vc::Log::Error("Failed to create vertex buffer for skybox");
        exit(1);
    }
    __uniformBuffer.Init(sizeof(vc::SkyboxShaderData));
}

VulkanSkybox::~VulkanSkybox()
{
    DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SetsIndex_Panorama).FreeSet(__descriptorSet);
}

vc::Error VulkanSkybox::_LoadSkybox(const vc::Texture& texture)
{
    // Init Descriptor Set
    __descriptorSet->GroupUpdateTexture(texture.GetConstImpl()->ConstAs<VulkanTexture>(), 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, 0);
    __uniformBuffer.WriteToBuffer(&_shaderData, sizeof(vc::SkyboxShaderData));
    __descriptorSet->GroupUpdateBuffer(__uniformBuffer, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);
    return vc::Error::Success;
}

vc::Error VulkanSkybox::_LoadIrradianceMap(const vc::Texture& texture, vc::Texture & irradianceMap)
{
    const auto & computeShader = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::IrradianceMap);
    {
        SingleTimeCommandBuffer cmdBuffer;
        if (CommandPoolManager::GetComputeCommandPool()->CreateSingleTimeCommandBuffer(cmdBuffer) != vc::Error::Success)
        {
            vc::Log::Error("Failed to create single time command buffer for generating irradiance map");
            return vc::Error::Failure;
        }
        cmdBuffer.BindPipeline(computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
        DescriptorPool::GetPool()->BindDescriptorSets(DSETS_INDEX_PANORAMA, cmdBuffer, computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
        DescriptorPool::GetPool()->BindDescriptorSets(DSETS_INDEX_MATERIAL, cmdBuffer, computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
        DescriptorPool::GetPool()->BindDescriptorSets(DSETS_INDEX_CAMERA, cmdBuffer, computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
        cmdBuffer.Dispatch(SKYBOX_IRRADIANCE_WIDTH, SKYBOX_IRRADIANCE_HEIGHT, 1);
    }
    return vc::Error::Success;
}

vc::Error VulkanSkybox::_LoadRadianceMap(const vc::Texture& texture, vc::Texture & radianceMap)
{
    const auto & computeShader = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::RadianceMap);
    {
        // Create Radiance Map Texture and then bind to descriptor sets
        uint32_t originalWidth = texture.GetWidth();
        uint32_t originalHeight = texture.GetHeight();
        radianceMap.CreateReadWriteTexture(originalWidth, originalHeight, vc::ShaderVertexFormat::Vec4, SKYBOX_RADIANCE_MIP_LEVELS);
        Image & radianceImg = radianceMap.GetImpl()->As<VulkanTexture>()->GetImage();

        VkRect2D scissor = {{0, 0}, {originalWidth, originalHeight}};
        for (int i = 0; i < SKYBOX_RADIANCE_MIP_LEVELS; ++i)
        {
            // ImageView must be created first as it should be destroyed last
            ImageView imageView;
            // Image View for each mip level
            imageView.Create(radianceImg, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, i, 1, 0, 1);

            uint32_t width = std::max(1u, originalWidth >> i);
            uint32_t height = std::max(1u, originalHeight >> i);
            
            SingleTimeCommandBuffer cmdBuffer;
            if (CommandPoolManager::GetComputeCommandPool()->CreateSingleTimeCommandBuffer(cmdBuffer) != vc::Error::Success)
            {
                vc::Log::Error("Failed to create single time command buffer for generating irradiance map");
                return vc::Error::Failure;
            }
            cmdBuffer.BindPipeline(computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(DSETS_INDEX_CAMERA, cmdBuffer, computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(DSETS_INDEX_MATERIAL, cmdBuffer, computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
            DescriptorPool::GetPool()->BindDescriptorSets(DSETS_INDEX_PANORAMA, cmdBuffer, computeShader[0].GetImpl()->As<VulkanShaderPipeline>());

            VkViewport viewport = {0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f};
            cmdBuffer.SetViewport(viewport);
            cmdBuffer.SetScissor(scissor);

            DescriptorPool::GetPool()->GetDescriptorSets(DSETS_INDEX_MATERIAL).GroupUpdateImageView(imageView, 2, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, 0);
            float roughness = (1.0f / static_cast<float>(SKYBOX_RADIANCE_MIP_LEVELS) * i);
            vc::ShaderResourceTable::UpdateDescriptor(DSETS_INDEX_MATERIAL, 3, &roughness, sizeof(float));
            cmdBuffer.Dispatch(width, height, 1);
        }
    }
    return vc::Error::Success;
}

vc::Error VulkanSkybox::_LoadBlurMap(const common::Texture& texture, common::Texture& blurMap)
{
    const auto & computeShader = vc::RenderingPipeline::GetRenderingPipelineCache(vc::RenderingPipelineType::BlurMap);
    {
        SingleTimeCommandBuffer cmdBuffer;
        if (CommandPoolManager::GetComputeCommandPool()->CreateSingleTimeCommandBuffer(cmdBuffer) != vc::Error::Success)
        {
            vc::Log::Error("Failed to create single time command buffer for generating irradiance map");
            return vc::Error::Failure;
        }
        cmdBuffer.BindPipeline(computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
        DescriptorPool::GetPool()->BindDescriptorSets(DSETS_INDEX_PANORAMA, cmdBuffer, computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
        DescriptorPool::GetPool()->BindDescriptorSets(DSETS_INDEX_MATERIAL, cmdBuffer, computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
        DescriptorPool::GetPool()->BindDescriptorSets(DSETS_INDEX_CAMERA, cmdBuffer, computeShader[0].GetImpl()->As<VulkanShaderPipeline>());
        cmdBuffer.Dispatch(texture.GetWidth(), texture.GetHeight(), 1);
    }
    return vc::Error::Success;
}

vc::Error VulkanSkybox::_ChangeBlurFactor(const float factor)
{
    __uniformBuffer.WriteToBuffer(&_shaderData, sizeof(vc::SkyboxShaderData));
    return vc::Error::Success;
}
}
}
