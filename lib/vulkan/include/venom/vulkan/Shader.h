///
/// Project: VenomEngine
/// @file Shader.h
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/PhysicalDevice.h>
#include <venom/vulkan/SwapChain.h>
#include <venom/vulkan/RenderPass.h>
#include <venom/vulkan/VertexBuffer.h>
#include <venom/vulkan/DescriptorPool.h>
#include <venom/vulkan/DescriptorSetLayout.h>

#include <venom/common/math/Matrix.h>

namespace venom
{
namespace vulkan
{
class ShaderPipeline
{
public:
    ShaderPipeline();
    ~ShaderPipeline();
    ShaderPipeline(const ShaderPipeline&) = delete;
    ShaderPipeline& operator=(const ShaderPipeline&) = delete;
    ShaderPipeline(ShaderPipeline&& other) noexcept;
    ShaderPipeline& operator=(ShaderPipeline&& other) noexcept;

    vc::Error AddVertexBufferToLayout(const uint32_t vertexSize, const uint32_t binding, const uint32_t location, const uint32_t offset, const VkFormat format);
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
    vc::Error LoadShaders(const SwapChain * swapChain, const RenderPass * renderPass, const std::vector<std::string>& shaderPaths);
    VkPipeline GetPipeline() const;
    VkPipelineLayout GetPipelineLayout() const;
    const VkDescriptorSetLayout & GetDescriptorSetLayout() const;

    /**
    * @brief Gets the descriptor sets from the specified set index
    * returns a vector of size VENOM_MAX_FRAMES_IN_FLIGHT because of the multiple
    * swapchain images to handle
    * @return std::vector<DescriptorSet> &
    */
    std::vector<DescriptorSet> & GetDescriptorSets(const uint32_t index);
    /**
    * @brief Gets the descriptor set layout from the specified set index
    * @return uint32_t
    */
    uint32_t GetDescriptorSetCount() const;
    /**
    * @brief Bind all descriptor sets to the pipeline for the current image
    * @param commandBuffer
    * @param bindPoint
    */
    void BindDescriptorSets(const CommandBuffer & commandBuffer, const VkPipelineBindPoint bindPoint);
    /**
    * @brief Bind all descriptor sets to the pipeline for the current image
    * @param commandBuffer
    * @param bindPoint
    * @param dynamicOffsets
    */
    void BindDescriptorSets(const CommandBuffer & commandBuffer, const VkPipelineBindPoint bindPoint, const std::vector<uint32_t> & dynamicOffsets);
private:
    vc::Error LoadShader(const std::string& shaderPath, VkPipelineShaderStageCreateInfo * pipelineCreateInfo);

private:
    VkPipeline __graphicsPipeline;
    VkPipelineLayout __pipelineLayout;

    std::vector<std::vector<DescriptorSet>> __descriptorSets;
    std::vector<DescriptorSetLayout> __descriptorSetLayouts;
    DescriptorPool __descriptorPool;

    std::vector<std::unique_ptr<VertexBuffer>> __vertexBuffers;

    std::vector<VkVertexInputBindingDescription> __bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> __attributeDescriptions;
};

}
}