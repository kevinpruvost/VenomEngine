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

    VkPipelineVertexInputStateCreateInfo SetupVertexInput();
    vc::Error LoadShaders(const VkDevice logicalDevice, const SwapChain * swapChain, const RenderPass * renderPass, const std::vector<std::string>& shaderPaths);
    VkPipeline GetPipeline() const;

private:
    vc::Error LoadShader(const VkDevice logicalDevice, const std::string& shaderPath, VkPipelineShaderStageCreateInfo * pipelineCreateInfo);

private:
    VkPipeline __graphicsPipeline;
    VkPipelineLayout __pipelineLayout;
    VkDevice __logicalDevice;
};

}
}