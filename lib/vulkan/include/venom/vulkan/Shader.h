///
/// Project: Bazel_Vulkan_Metal
/// File: Shader.h
/// Date: 8/22/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/PhysicalDevice.h>
#include <venom/vulkan/SwapChain.h>
#include <venom/vulkan/RenderPass.h>
#include "tools/cpp/runfiles/runfiles.h"

namespace venom
{
namespace vulkan
{
class ShaderPipeline
{
public:
    ShaderPipeline();
    ~ShaderPipeline();

    Error LoadShaders(const VkDevice logicalDevice, const SwapChain * swapChain, const RenderPass * renderPass, const std::vector<std::string>& shaderPaths);
    VkPipeline GetPipeline() const;

private:
    Error LoadShader(const VkDevice logicalDevice, const std::string& shaderPath, VkPipelineShaderStageCreateInfo * pipelineCreateInfo);

private:
    VkPipeline __graphicsPipeline;
    VkPipelineLayout __pipelineLayout;
    VkDevice __logicalDevice;
};

}
}