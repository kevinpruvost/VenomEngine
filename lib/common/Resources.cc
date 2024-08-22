///
/// Project: Bazel_Vulkan_Metal
/// File: Resources.cc
/// Date: 8/22/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <common/Resources.h>

#include "tools/cpp/runfiles/runfiles.h"

namespace venom
{
typedef bazel::tools::cpp::runfiles::Runfiles Runfiles;

static std::unique_ptr<Runfiles> s_runfiles;
void Resources::InitializeFilesystem(char** argv)
{
    s_runfiles.reset(bazel::tools::cpp::runfiles::Runfiles::Create(argv[0]));
}

std::string Resources::GetResourcePath(const std::string& resourcePath)
{
    const std::string path = "BazelVulkanMetalWorkspace/resources/" + resourcePath;
    return s_runfiles->Rlocation(path);
}
}
