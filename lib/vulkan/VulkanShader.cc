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
{
}

VulkanShader::~VulkanShader()
{
}

Error VulkanShader::LoadShader(const Context* context, const std::string& shaderPath)
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
    return Error::Success;
}

}
