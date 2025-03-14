///
/// Project: VenomEngine
/// @file Resources.h
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Error.h>
#include <venom/common/Export.h>
#include <venom/common/String.h>
#include <venom/common/Containers.h>

#if defined(VENOM_PLATFORM_APPLE)
vc::String getAppleResourcePath();
vc::String getAppleLogPath();
#endif

namespace venom
{
namespace common
{
class Resources
{
public:
    VENOM_COMMON_API static void InitializeFilesystem(int argc, const char* argv[]);
    VENOM_COMMON_API static void FreeFilesystem();
    VENOM_COMMON_API static String GetLogsPath(const String & logPath);
    VENOM_COMMON_API static String GetResourcePath(const String & resourcePath);
    VENOM_COMMON_API static String GetTexturesResourcePath(const String & resourcePath);
    VENOM_COMMON_API static String GetVenomAssetResourcePath(const String & resourcePath);
    VENOM_COMMON_API static String GetFontsResourcePath(const String & resourcePath);
    VENOM_COMMON_API static String GetShadersResourcePath(const String & resourcePath);
    VENOM_COMMON_API static String GetShadersFolderPath();
    VENOM_COMMON_API static String GetModelsResourcePath(const String & resourcePath);
private:
    VENOM_COMMON_API static String __GetResourcePath(const String & resourcePath, const String & folder);
};
}
}
