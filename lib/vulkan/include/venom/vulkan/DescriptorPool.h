///
/// Project: VenomEngine
/// @file DescriptorPool.h
/// @date Sep, 16 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/DescriptorSet.h>
#include <venom/vulkan/DescriptorSetLayout.h>
#include <venom/vulkan/plugin/graphics/ShaderPipeline.h>
#include <venom/common/VenomSettings.h>
#include <venom/common/plugin/graphics/GraphicsApplication.h>

namespace venom
{
namespace vulkan
{
/**
* @brief Regroups descriptor sets by frame, so if updates have to be done by group, they will be done
* on the group directly
*/
class DescriptorSetGroup
{
public:
    DescriptorSetGroup() = default;
    ~DescriptorSetGroup();

    void GroupUpdate(const VkWriteDescriptorSet &write);
    void GroupUpdateBuffer(UniformBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void GroupUpdateBuffer(StorageBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    inline void GroupUpdateTexture(const vc::Texture & texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { GroupUpdateTexture(texture.GetImpl()->As<VulkanTexture>(), binding, descriptorType, descriptorCount, arrayElement); }
    void GroupUpdateTexture(const VulkanTexture * texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void GroupUpdateSampler(const Sampler &sampler, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void GroupUpdateImageView(const ImageView &imageView, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void GroupUpdatePerFrame(int frameIndex, const VkWriteDescriptorSet &write);
    void GroupUpdateBufferPerFrame(int frameIndex, UniformBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void GroupUpdateBufferPerFrame(int frameIndex, StorageBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    inline void GroupUpdateTexturePerFrame(int frameIndex, const vc::Texture & texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { GroupUpdateTexturePerFrame(frameIndex, texture.GetImpl()->As<VulkanTexture>(), binding, descriptorType, descriptorCount, arrayElement); }
    void GroupUpdateTexturePerFrame(int frameIndex, const VulkanTexture * texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void GroupUpdateSamplerPerFrame(int frameIndex, const Sampler &sampler, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);
    void GroupUpdateImageViewPerFrame(int frameIndex, const ImageView &imageView, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0);

    inline DescriptorSet & operator[](const uint32_t index) { return __descriptorSets[index]; }
    inline DescriptorSet & GetCurrentSet() { return __descriptorSets[vc::GraphicsApplication::GetCurrentFrameInFlight()]; }

private:
    vc::Array<DescriptorSet, VENOM_MAX_FRAMES_IN_FLIGHT> __descriptorSets;
};

class DescriptorSetGroupAllocator
{
public:
    DescriptorSetGroupAllocator(vc::Vector<DescriptorSetGroup> && sets);

    DescriptorSetGroup * AllocateSet();
    void FreeSet(DescriptorSetGroup * set);

#define DSGA_GROUP_UPDATE(func) for (int i = 0; i < __descriptorSetGroups.size(); ++i) { __descriptorSetGroups[i].func; }

    inline void GroupUpdate(const VkWriteDescriptorSet &write) { DSGA_GROUP_UPDATE(GroupUpdate(write)); }
    inline void GroupUpdateBuffer(UniformBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateBuffer(buffer, bufferOffset, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateBuffer(StorageBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateBuffer(buffer, bufferOffset, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateStorageImage(const VulkanTexture * texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateTexture(texture, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateTexture(const vc::Texture & texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateTexture(texture.GetImpl()->As<VulkanTexture>(), binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateTexture(const VulkanTexture * texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateTexture(texture, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateSampler(const Sampler &sampler, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateSampler(sampler, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateImageView(const ImageView &imageView, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateImageView(imageView, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdatePerFrame(int frameIndex, const VkWriteDescriptorSet &write) { DSGA_GROUP_UPDATE(GroupUpdatePerFrame(frameIndex, write)); }
    inline void GroupUpdateBufferPerFrame(int frameIndex, UniformBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateBufferPerFrame(frameIndex, buffer, bufferOffset, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateBufferPerFrame(int frameIndex, StorageBuffer & buffer, uint32_t bufferOffset, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateBufferPerFrame(frameIndex, buffer, bufferOffset, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateTexturePerFrame(int frameIndex, const vc::Texture & texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateTexturePerFrame(frameIndex, texture, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateTexturePerFrame(int frameIndex, const VulkanTexture * texture, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateTexturePerFrame(frameIndex, texture, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateSamplerPerFrame(int frameIndex, const Sampler &sampler, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateSamplerPerFrame(frameIndex, sampler, binding, descriptorType, descriptorCount, arrayElement)); }
    inline void GroupUpdateImageViewPerFrame(int frameIndex, const ImageView &imageView, uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t arrayElement = 0) { DSGA_GROUP_UPDATE(GroupUpdateImageViewPerFrame(frameIndex, imageView, binding, descriptorType, descriptorCount, arrayElement)); }

    inline DescriptorSetGroup & operator[](const uint32_t index) { return __descriptorSetGroups[index]; }
    inline size_t size() const { return __descriptorSetGroups.size(); }

#undef DSGA_GROUP_UPDATE
private:
    vc::Vector<DescriptorSetGroup> __descriptorSetGroups;
    std::stack<DescriptorSetGroup *> __freeSets;
};

class DescriptorPool
{
public:
    DescriptorPool();
    ~DescriptorPool();
    DescriptorPool &operator=(const DescriptorPool &other) = delete;
    DescriptorPool(const DescriptorPool &other) = delete;
    DescriptorPool &operator=(DescriptorPool &&other) noexcept = delete;
    DescriptorPool(DescriptorPool &&other) noexcept = delete;

    void AddPoolSize(VkDescriptorType type, uint32_t count);
    /**
    * @brief Add a descriptor set layout binding to the pipeline
    * For HLSL shader writing, refer to https://github.com/microsoft/DirectXShaderCompiler/blob/080aeb7199e66e4b0a2b6383fd26a9f8d2cccbf5/docs/SPIR-V.rst#:~:text=Without%20attribute%20and%20command%2Dline%20option%2C,so%20this%20may%20cause%20overlap.
    * @param descriptorSetIndex
    * @param binding
    * @param type
    * @param descriptorCount
    * @param stageFlags
    */
    void AddDescriptorSetLayoutBinding(const uint32_t descriptorSetIndex, const uint32_t binding, const VkDescriptorType type, const uint32_t descriptorCount, const VkShaderStageFlags stageFlags);
    /**
    * @brief Set the descriptor set layout create flags
    * @param descriptorSetIndex
    * @param flags
    */
    void SetDescriptorSetLayoutCreateFlags(const uint32_t descriptorSetIndex, const VkDescriptorSetLayoutCreateFlags flags);
    /**
    * @brief Set the descriptor set layout binding specifications
    * @param descriptorSetIndex
    * @param flags Binding Flags
    */
    void SetDescriptorSetLayoutBindingSpecifications(const uint32_t descriptorSetIndex, const VkDescriptorBindingFlags flags);
    /**
    * @brief Set the descriptor set layout max sets
    * @param descriptorSetIndex
    * @param maxSets
    */
    void SetDescriptorSetLayoutMaxSets(const uint32_t descriptorSetIndex, const uint32_t flags);
    DescriptorSetLayout & GetOrCreateDescriptorSetLayout(const uint32_t descriptorSetIndex);
    inline void SetDescriptorSetLayoutBindless(const uint32_t descriptorSetIndex) { SetDescriptorSetLayoutBindingSpecifications(descriptorSetIndex, VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT); }
    vc::Error Create(VkDescriptorPoolCreateFlags flags, uint32_t maxSets = VENOM_MAX_FRAMES_IN_FLIGHT);
    vc::Vector<DescriptorSetGroup> AllocateSets(const VkDescriptorSetLayout &layout, uint32_t count, const bool bindless) const;
    vc::Vector<DescriptorSetGroup> AllocateSets(const vc::Vector<VkDescriptorSetLayout> &layouts, const bool bindless) const;

    static inline DescriptorPool * GetPool() { return s_pool; }

    inline vc::Vector<DescriptorSetGroupAllocator> & GetDescriptorSets() { return __descriptorSets; }
    inline vc::Vector<DescriptorSetLayout> & GetDescriptorSetLayouts() { return __descriptorSetLayouts; }
    inline vc::Vector<VkDescriptorSetLayout> & GetVkDescriptorSetLayouts() { return __vkDescriptorSetLayouts; }

    /**
    * @brief Gets the descriptor sets from the specified set index
    * returns a vector of size VENOM_MAX_FRAMES_IN_FLIGHT because of the multiple
    * swapchain images to handle
    * @return vc::Vector<DescriptorSet> &
    */
    inline DescriptorSetGroupAllocator & GetDescriptorSets(const uint32_t index)
    {
        venom_assert(index < __descriptorSets.size(), "Index out of range");
        return __descriptorSets[index];
    }

    /**
    * @brief Bind all descriptor sets to the pipeline for the current image
    * @param descriptorSetIndex
    * @param commandBuffer
    * @param shaderPipeline Shader Pipeline we are binding the descriptor sets to
    * @param bindPoint
    */
    void BindDescriptorSets(const int descriptorSetIndex, const CommandBuffer & commandBuffer, const VulkanShaderPipeline * pipeline);
    /**
    * @brief Bind all descriptor sets to the pipeline for the current image
    * @param descriptorSetIndex
    * @param commandBuffer
    * @param shaderPipeline Shader Pipeline we are binding the descriptor sets to
    * @param bindPoint
    * @param dynamicOffsets
    */
    void BindDescriptorSets(const int descriptorSetIndex, const CommandBuffer & commandBuffer, const VulkanShaderPipeline * pipeline, const vc::Vector<uint32_t> & dynamicOffsets);

    inline VkDescriptorPool GetVkDescriptorPool() const { return __pool; }

private:
    static DescriptorPool * s_pool;

private:
    vc::Vector<VkDescriptorPoolSize> __poolSizes;
    vc::Vector<DescriptorSetLayout> __descriptorSetLayouts;
    vc::Vector<VkDescriptorSetLayout> __vkDescriptorSetLayouts;
    vc::Vector<DescriptorSetGroupAllocator> __descriptorSets;
    VkDescriptorPoolCreateInfo __poolInfo;
    VkDescriptorPool __pool;
};
}
}
