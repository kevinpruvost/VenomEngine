///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanShader.h
/// Date: 8/22/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanPhysicalDevice.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace venom
{
class VulkanShader
{
public:
    // Making sure handles are not destroyed when the object is moved

    VulkanShader();
    ~VulkanShader();
    VulkanShader(const VulkanShader&) = delete;
    VulkanShader& operator=(const VulkanShader&) = delete;
    VulkanShader(VulkanShader&&);
    VulkanShader& operator=(VulkanShader&&);

    Error LoadShader(VkDevice logicalDevice, const std::string& shaderPath);

private:
    VkShaderModule __shaderModule;
    VkDevice __logicalDevice;
};

class VulkanShaderPipeline
{
public:
    VulkanShaderPipeline();
    ~VulkanShaderPipeline();

    Error LoadShaders(VkDevice logicalDevice, const std::vector<std::string>& shaderPaths);

private:
    std::vector<VulkanShader> __shaders;
};

}