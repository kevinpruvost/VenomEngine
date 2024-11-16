///
/// Project: VenomEngine
/// @file ShaderPipeline.h
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/ShaderPipeline.h>

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
enum class PipelineType : uint32_t
{
    Graphics = 0,
    Compute = 1,
    RayTracing = 2
};

class VulkanShaderResource : public vc::ShaderResource
{
public:
    VulkanShaderResource(vc::GraphicsCachedResourceHolder * h);
    ~VulkanShaderResource() override;

    void DestroyShaderModules();
public:
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;

    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo;
    VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    bool shaderDirty;
    PipelineType pipelineType;
};

class VulkanShaderPipeline : public vc::ShaderPipelineImpl
{
public:
    VulkanShaderPipeline();
    ~VulkanShaderPipeline() override;
    VulkanShaderPipeline(const VulkanShaderPipeline&) = delete;
    VulkanShaderPipeline& operator=(const VulkanShaderPipeline&) = delete;
    VulkanShaderPipeline(VulkanShaderPipeline&& other) noexcept;
    VulkanShaderPipeline& operator=(VulkanShaderPipeline&& other) noexcept;

    vc::Error _LoadShader(const std::string & path) override;
    void SetMultiSamplingCount(const int samples) override;
    void _SetLineWidth(const float width) override;
    void _SetDepthTest(const bool enable) override;
    void _SetDepthWrite(const bool enable) override;
    vc::Error _ReloadShader() override;

    void _AddVertexBufferToLayout(const uint32_t vertexSize, const uint32_t binding, const uint32_t location, const uint32_t offset, const vc::ShaderVertexFormat format) override;
    vc::Error LoadShaders();
    VkPipeline GetPipeline() const;
    VkPipelineLayout GetPipelineLayout() const;
    const VkDescriptorSetLayout & GetDescriptorSetLayout() const;
private:
    vc::Error LoadShader(const std::string& shaderPath, VkPipelineShaderStageCreateInfo * pipelineCreateInfo);
};

}
}