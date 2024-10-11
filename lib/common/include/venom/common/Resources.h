///
/// Project: VenomEngine
/// @file Resources.h
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Export.h>
#include <string>

#if defined(__APPLE__) && defined(VENOM_PACKAGE)
std::string getResourcePath();
#endif

namespace venom
{
namespace common
{
class Resources
{
public:
    VENOM_COMMON_API static void InitializeFilesystem(int argc, char ** argv);
    VENOM_COMMON_API static void FreeFilesystem();
    VENOM_COMMON_API static std::string GetResourcePath(const std::string & resourcePath);
    VENOM_COMMON_API static std::string GetTexturesResourcePath(const std::string & resourcePath);
    VENOM_COMMON_API static std::string GetShadersResourcePath(const std::string & resourcePath);
    VENOM_COMMON_API static std::string GetModelsResourcePath(const std::string & resourcePath);
private:
    VENOM_COMMON_API static std::string __GetResourcePath(const std::string & resourcePath, const std::string & folder);
};
}
}