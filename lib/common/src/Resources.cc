///
/// Project: VenomEngine
/// @file Resources.cc
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Resources.h>

#include <venom/common/Log.h>
#include <venom/common/Config.h>

#include <filesystem>

namespace venom::common
{
// If using bazel
#ifdef __BAZEL__
#include "tools/cpp/runfiles/runfiles.h"

typedef bazel::tools::cpp::runfiles::Runfiles Runfiles;

static vc::UPtr<Runfiles> s_runfiles;
void Resources::InitializeFilesystem(int argc, char** argv)
{
    s_runfiles.reset(bazel::tools::cpp::runfiles::Runfiles::Create(argv[0]));
}

void Resources::FreeFilesystem()
{
    s_runfiles.reset();
}

vc::String Resources::GetResourcePath(const vc::String& resourcePath)
{
    const vc::String path = "VenomEngineWorkspace/resources/" + resourcePath;
    return s_runfiles->Rlocation(path);
}

#else

#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#endif

static vc::String s_basePath;
static vc::String s_logPath;
void Resources::InitializeFilesystem(int argc, const char* argv[])
{
    (void)argc;
    (void)argv;

#if defined(VENOM_PLATFORM_APPLE)
    vc::String bundleResourcePath = getAppleResourcePath();
    if (!bundleResourcePath.empty()) {
        s_basePath = bundleResourcePath;
        if (s_basePath.back() != '/') s_basePath += "/";
    }
    vc::String logPath = getAppleLogPath();
    if (!logPath.empty()) {
        s_logPath = logPath;
        if (s_logPath.back() != '/') s_logPath += "/";
        s_logPath += "VenomEngine_Logs/";
        return;
    }
#endif

    s_basePath = "./resources/";
    // Verify if the path exists
    // If not, test at ../
    if (!std::filesystem::exists(s_basePath)) {
        s_basePath = "./../resources/";
        if (!std::filesystem::exists(s_basePath)) {
            Log::Error("Failed to find resources folder");
            exit(1);
        }
    }
    s_logPath = "./logs/";
}

void Resources::FreeFilesystem()
{
}

String Resources::GetLogsPath(const String& logPath)
{
    return s_logPath + logPath;
}

static bool validPath(const vc::String& path, vc::String & res)
{
    std::error_code ec;
    // Replace all backslashes with forward slashes
    vc::String realPath = path;
    std::replace(realPath.begin(), realPath.end(), '\\', '/');
    realPath = std::filesystem::canonical(realPath, ec);
    if (realPath.empty() || ec) {
        Log::LogToFile("Failed to find canonical path when looking cache for: %s | error: %s ", path.c_str(), ec.message().c_str());
        return false;
    }
    realPath = std::filesystem::relative(realPath, ec);
    if (realPath.empty() || ec) {
        Log::LogToFile("Failed to find relative path when looking cache for: %s | error: %s", path.c_str(), ec.message().c_str());
        return false;
    }
    res = realPath;
    return true;
}

vc::String Resources::GetResourcePath(const vc::String& resourcePath)
{
    return s_basePath + resourcePath;
}

#endif

vc::String Resources::GetTexturesResourcePath(const vc::String& resourcePath)
{
    // If resource path already points to a valid pathm then returns it
    return __GetResourcePath(resourcePath, GetResourcePath("textures/"));
}

vc::String Resources::GetFontsResourcePath(const vc::String& resourcePath)
{
    return __GetResourcePath(resourcePath, GetResourcePath("fonts/"));
}

vc::String Resources::GetShadersResourcePath(const vc::String& resourcePath)
{
    return __GetResourcePath(resourcePath, GetResourcePath("shaders/"));
}

vc::String Resources::GetShadersFolderPath()
{
    return GetResourcePath(
       Config::GetGraphicsPluginType() == GraphicsPlugin::GraphicsPluginType::Vulkan ?
#ifdef VENOM_DEBUG
        "shaders/Debug/"
#else
        "shaders/Release/"
#endif
       : "shaders/"
    );
}

vc::String Resources::GetModelsResourcePath(const vc::String& resourcePath)
{
    return __GetResourcePath(resourcePath, GetResourcePath("models/"));
}

vc::String Resources::__GetResourcePath(const vc::String& resourcePath, const vc::String& folder)
{
    vc::String res;
    if (!validPath(resourcePath, res)
        && !validPath(folder + resourcePath, res))
        return "";
    return res;
}
}
