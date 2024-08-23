///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanShader.h
/// Date: 8/22/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace venom
{

class VulkanShaderPipeline
{
public:
    VulkanShaderPipeline();
    ~VulkanShaderPipeline();

    Error LoadShaders(VkDevice logicalDevice, const VulkanSwapChain * swapChain, const VulkanRenderPass * renderPass, const std::vector<std::string>& shaderPaths);
    VkPipeline GetPipeline() const;

private:
    Error LoadShader(VkDevice logicalDevice, const std::string& shaderPath, VkPipelineShaderStageCreateInfo * pipelineCreateInfo);

private:
    VkPipeline __graphicsPipeline;
    VkPipelineLayout __pipelineLayout;
    VkDevice __logicalDevice;
};

}