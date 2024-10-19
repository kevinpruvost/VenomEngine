///
/// Project: VenomEngine
/// @file DescriptorSet.cc
/// @date Sep, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/DescriptorSet.h>

#include <venom/vulkan/LogicalDevice.h>

namespace venom
{
namespace vulkan
{
DescriptorSet::DescriptorSet()
    : __set(VK_NULL_HANDLE)
{
}

DescriptorSet::~DescriptorSet()
{
    // Automatically freed by the descriptor pool
}

void DescriptorSet::Update(const VkWriteDescriptorSet& write)
{
    vkUpdateDescriptorSets(LogicalDevice::GetVkDevice(), 1, &write, 0, nullptr);
}

void DescriptorSet::UpdateBuffer(UniformBuffer& buffer, uint32_t bufferOffset, uint32_t binding,
    VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    VkDescriptorBufferInfo bufferInfo = {
        .buffer = buffer.GetVkBuffer(),
        .offset = bufferOffset,
        .range = buffer.GetSize()
    };

    VkWriteDescriptorSet write = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = __set,
        .dstBinding = binding,
        .dstArrayElement = arrayElement,
        .descriptorCount = descriptorCount,
        .descriptorType = descriptorType,
        .pBufferInfo = &bufferInfo
    };

    Update(write);
}

void DescriptorSet::UpdateBuffer(StorageBuffer& buffer, uint32_t bufferOffset, uint32_t binding,
    VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement)
{
    VkDescriptorBufferInfo bufferInfo = {
        .buffer = buffer.GetVkBuffer(),
        .offset = bufferOffset,
        .range = buffer.GetSize()
    };

    VkWriteDescriptorSet write = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = __set,
        .dstBinding = binding,
        .dstArrayElement = arrayElement,
        .descriptorCount = descriptorCount,
        .descriptorType = descriptorType,
        .pBufferInfo = &bufferInfo
    };

    Update(write);
}

void DescriptorSet::UpdateTexture(const VulkanTexture* texture, uint32_t binding, VkDescriptorType descriptorType,
                                  uint32_t descriptorCount, uint32_t arrayElement)
{
    VkDescriptorImageInfo imageInfo = {
        .imageView = texture->GetImageView().GetVkImageView(),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    VkWriteDescriptorSet write = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = __set,
        .dstBinding = binding,
        .dstArrayElement = arrayElement,
        .descriptorCount = descriptorCount,
        .descriptorType = descriptorType,
        .pImageInfo = &imageInfo
    };

    Update(write);
}

void DescriptorSet::UpdateSampler(const Sampler& sampler, uint32_t binding, VkDescriptorType descriptorType,
    uint32_t descriptorCount, uint32_t arrayElement)
{
    VkDescriptorImageInfo imageInfo = {
        .sampler = sampler.GetVkSampler()
    };

    VkWriteDescriptorSet write = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = __set,
        .dstBinding = binding,
        .dstArrayElement = arrayElement,
        .descriptorCount = descriptorCount,
        .descriptorType = descriptorType,
        .pImageInfo = &imageInfo
    };

    Update(write);
}

const VkDescriptorSet & DescriptorSet::GetVkDescriptorSet() const
{
    return __set;
}
}
}