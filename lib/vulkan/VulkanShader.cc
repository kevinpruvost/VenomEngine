///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanShader.cc
/// Date: 8/22/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "VulkanShader.h"

#include <fstream>

#include <common/Application.h>
#include <common/Resources.h>

namespace venom
{
VulkanShader::VulkanShader()
    : __shaderModule(VK_NULL_HANDLE)
    , __logicalDevice(VK_NULL_HANDLE)
{
}

VulkanShader::~VulkanShader()
{
    if (__shaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(__logicalDevice, __shaderModule, nullptr);
    }
}

VulkanShader::VulkanShader(VulkanShader && other)
    : __shaderModule(other.__shaderModule)
    , __logicalDevice(other.__logicalDevice)
{
    other.__shaderModule = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
}

VulkanShader & VulkanShader::operator=(VulkanShader && other)
{
    if (this == &other) return *this;

    __shaderModule = other.__shaderModule;
    __logicalDevice = other.__logicalDevice;
    other.__shaderModule = VK_NULL_HANDLE;
    other.__logicalDevice = VK_NULL_HANDLE;
    return *this;
}

Error VulkanShader::LoadShader(VkDevice logicalDevice, const std::string& shaderPath)
{
    const auto folder_shaderPath = std::string("shaders/compiled/") + shaderPath;
    const std::string path = Resources::GetResourcePath(folder_shaderPath);
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        Log::Error("Failed to open file: %s", path.c_str());
        return Error::Failure;
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

    if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &__shaderModule) != VK_SUCCESS)
    {
        Log::Error("Failed to create shader module");
        return Error::Failure;
    }
    __logicalDevice = logicalDevice;
    return Error::Success;
}

VulkanShaderPipeline::VulkanShaderPipeline()
{
}

VulkanShaderPipeline::~VulkanShaderPipeline()
{
}

Error VulkanShaderPipeline::LoadShaders(VkDevice logicalDevice, const std::vector<std::string>& shaderPaths)
{
    for (const auto& shaderPath : shaderPaths)
    {
        VulkanShader shader;
        if (shader.LoadShader(logicalDevice, shaderPath) != Error::Success)
        {
            Log::Error("Failed to load shader: %s", shaderPath.c_str());
            return Error::Failure;
        }
        // Avoid shader from destruction
        __shaders.emplace_back(std::move(shader));
    }
    return Error::Success;
}
}
