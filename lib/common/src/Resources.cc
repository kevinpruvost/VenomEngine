///
/// Project: VenomEngine
/// @file Resources.cc
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Resources.h>

#include "tools/cpp/runfiles/runfiles.h"

namespace venom::common
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
