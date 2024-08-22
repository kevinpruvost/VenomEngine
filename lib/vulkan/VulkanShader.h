///
/// Project: Bazel_Vulkan_Metal
/// File: VulkanShader.h
/// Date: 8/22/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include "VulkanDebug.h"

#include "common/Context.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace venom
{
class VulkanShader
{
public:
    VulkanShader();
    ~VulkanShader();

    Error LoadShader(const Context* context, const std::string& shaderPath);
};
}