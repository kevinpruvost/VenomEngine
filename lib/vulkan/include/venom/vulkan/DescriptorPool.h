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
#include <venom/vulkan/ShaderPipeline.h>
#include <venom/common/VenomSettings.h>

namespace venom
{
namespace vulkan
{
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
    * @param bindingCount
    * @param stageFlags
    */
    void AddDescriptorSetLayoutBinding(const uint32_t descriptorSetIndex, const uint32_t binding, const VkDescriptorType type, const uint32_t count, const VkShaderStageFlags stageFlags);
    vc::Error Create(VkDescriptorPoolCreateFlags flags, uint32_t maxSets = VENOM_MAX_FRAMES_IN_FLIGHT);
    std::vector<DescriptorSet> AllocateSets(const VkDescriptorSetLayout &layout, uint32_t count) const;
    std::vector<DescriptorSet> AllocateSets(const std::vector<VkDescriptorSetLayout> &layouts) const;

    static inline DescriptorPool * GetPool() { return s_pool; }

    inline std::vector<std::vector<DescriptorSet>> & GetDescriptorSets() { return __descriptorSets; }
    inline std::vector<DescriptorSetLayout> & GetDescriptorSetLayouts() { return __descriptorSetLayouts; }
    inline std::vector<VkDescriptorSetLayout> & GetVkDescriptorSetLayouts() { return __vkDescriptorSetLayouts; }

    /**
    * @brief Gets the descriptor sets from the specified set index
    * returns a vector of size VENOM_MAX_FRAMES_IN_FLIGHT because of the multiple
    * swapchain images to handle
    * @return std::vector<DescriptorSet> &
    */
    inline std::vector<DescriptorSet> & GetDescriptorSets(const uint32_t index)
    {
        venom_assert(index < __descriptorSets.size(), "Index out of range");
        return __descriptorSets[index];
    }

    /**
    * @brief Bind all descriptor sets to the pipeline for the current image
    * @param commandBuffer
    * @param shaderPipeline Shader Pipeline we are binding the descriptor sets to
    * @param bindPoint
    */
    void BindDescriptorSets(const CommandBuffer & commandBuffer, const ShaderPipeline & pipeline, const VkPipelineBindPoint bindPoint);
    /**
    * @brief Bind all descriptor sets to the pipeline for the current image
    * @param commandBuffer
    * @param shaderPipeline Shader Pipeline we are binding the descriptor sets to
    * @param bindPoint
    * @param dynamicOffsets
    */
    void BindDescriptorSets(const CommandBuffer & commandBuffer, const ShaderPipeline & pipeline, const VkPipelineBindPoint bindPoint, const std::vector<uint32_t> & dynamicOffsets);

private:
    static DescriptorPool * s_pool;

private:
    std::vector<VkDescriptorPoolSize> __poolSizes;
    std::vector<DescriptorSetLayout> __descriptorSetLayouts;
    std::vector<VkDescriptorSetLayout> __vkDescriptorSetLayouts;
    std::vector<std::vector<DescriptorSet>> __descriptorSets;
    VkDescriptorPoolCreateInfo __poolInfo;
    VkDescriptorPool __pool;
};
}
}
