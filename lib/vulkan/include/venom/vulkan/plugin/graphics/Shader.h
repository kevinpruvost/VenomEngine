///
/// Project: VenomEngine
/// @file Shader.h
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Shader.h>

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
class VulkanShader : public vc::ShaderImpl
{
public:
    VulkanShader();
    ~VulkanShader() override;
    VulkanShader(const VulkanShader&) = delete;
    VulkanShader& operator=(const VulkanShader&) = delete;
    VulkanShader(VulkanShader&& other) noexcept;
    VulkanShader& operator=(VulkanShader&& other) noexcept;

    vc::Error _LoadShader(const std::string & path) override;
    void SetLineWidth(const float width) override;
    void SetMultiSamplingCount(const int samples) override;

    void _AddVertexBufferToLayout(const uint32_t vertexSize, const uint32_t binding, const uint32_t location, const uint32_t offset, const vc::ShaderVertexFormat format) override;
    vc::Error LoadShaders();
    VkPipeline GetPipeline() const;
    VkPipelineLayout GetPipelineLayout() const;
    const VkDescriptorSetLayout & GetDescriptorSetLayout() const;
private:
    vc::Error LoadShader(const std::string& shaderPath, VkPipelineShaderStageCreateInfo * pipelineCreateInfo);

private:
    VkPipeline __graphicsPipeline;
    VkPipelineLayout __pipelineLayout;

    std::vector<VkVertexInputBindingDescription> __bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> __attributeDescriptions;

    VkPipelineMultisampleStateCreateInfo __multisamplingCreateInfo;
    VkPipelineRasterizationStateCreateInfo __rasterizerCreateInfo;
};

}
}