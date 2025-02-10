///
/// Project: VenomEngine
/// @file DescriptorSet.h
/// @date Sep, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>

#include <venom/vulkan/UniformBuffer.h>
#include <venom/vulkan/plugin/graphics/Texture.h>
#include <venom/vulkan/StorageBuffer.h>

namespace venom
{
namespace vulkan
{
class DescriptorPool;
class DescriptorSet
{
public:
    DescriptorSet();
    ~DescriptorSet();
    DescriptorSet(const DescriptorSet &other) = default;
    DescriptorSet &operator=(const DescriptorSet &other) = default;
    DescriptorSet(DescriptorSet &&other) noexcept = default;
    DescriptorSet &operator=(DescriptorSet &&other) noexcept = default;

    void Update(const VkWriteDescriptorSet &write);
    void UpdateBuffer(UniformBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void UpdateBuffer(StorageBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void UpdateTexture(const VulkanTexture * texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void UpdateSampler(const Sampler &sampler, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void UpdateImageView(const ImageView &imageView, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void UpdateImageView(const ImageView &imageView, const Sampler & sampler, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);

    const VkDescriptorSet & GetVkDescriptorSet() const;
    const VkDescriptorSet * GetVkDescriptorSetPtr() const;
private:
    friend class DescriptorPool;
    VkDescriptorSet __set;
};
}
}
