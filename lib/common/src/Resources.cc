///
/// Project: VenomEngine
/// @file Resources.cc
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Resources.h>

namespace venom::common
{
// If using bazel
#ifdef __BAZEL__
#include "tools/cpp/runfiles/runfiles.h"

typedef bazel::tools::cpp::runfiles::Runfiles Runfiles;

static std::unique_ptr<Runfiles> s_runfiles;
void Resources::InitializeFilesystem(char** argv)
{
    s_runfiles.reset(bazel::tools::cpp::runfiles::Runfiles::Create(argv[0]));
}

void Resources::FreeFilesystem()
{
    s_runfiles.reset();
}

std::string Resources::GetResourcePath(const std::string& resourcePath)
{
    const std::string path = "VenomEngineWorkspace/resources/" + resourcePath;
    return s_runfiles->Rlocation(path);
}

#else

void Resources::InitializeFilesystem(char** argv)
{
    (void)argv;
}

void Resources::FreeFilesystem()
{
}

std::string Resources::GetResourcePath(const std::string& resourcePath)
{
    return "resources/" + resourcePath;
}

#endif

std::string Resources::GetTexturesResourcePath(const std::string& resourcePath)
{
    return GetResourcePath("textures/" + resourcePath);
}

std::string Resources::GetShadersResourcePath(const std::string& resourcePath)
{
    return GetResourcePath("shaders/" + resourcePath);
}

std::string Resources::GetModelsResourcePath(const std::string& resourcePath)
{
    return GetResourcePath("models/" + resourcePath);
}

}
