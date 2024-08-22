///
/// Project: Bazel_Vulkan_Metal
/// File: Resources.h
/// Date: 8/22/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <common/Export.h>
#include <string>

namespace venom
{
class Resources
{
public:
    VENOM_COMMON_API static void InitializeFilesystem(char ** argv);
    VENOM_COMMON_API static std::string GetResourcePath(const std::string & resourcePath);
};
}