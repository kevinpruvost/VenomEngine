///
/// Project: VenomEngineWorkspace
/// @file Skybox.cc
/// @date Oct, 29 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/Skybox.h>

namespace venom
{
namespace vulkan
{
VulkanSkybox::VulkanSkybox()
    : __descriptorSet(DescriptorPool::GetPool()->GetDescriptorSets(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_PANORAMA).AllocateSet())
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
    __uniformBuffer.Init(sizeof(float));
}

VulkanSkybox::~VulkanSkybox()
{
}

vc::Error VulkanSkybox::_LoadSkybox(const vc::Texture& texture)
{
    // Init Descriptor Set
    __descriptorSet->GroupUpdateTexture(texture.GetConstImpl()->ConstAs<VulkanTexture>(), 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, 0);
    float lum = texture.GetConstImpl()->ConstAs<VulkanTexture>()->GetTexturePeakLuminance();
    __uniformBuffer.WriteToBuffer(&texture.GetConstImpl()->ConstAs<VulkanTexture>()->GetTexturePeakLuminance(), sizeof(float));
    __descriptorSet->GroupUpdateBuffer(__uniformBuffer, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0);
    return vc::Error::Success;
}
}
}