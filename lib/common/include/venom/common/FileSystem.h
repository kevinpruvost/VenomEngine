///
/// Project: VenomEngineWorkspace
/// @file File.h
/// @date Feb, 17 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/File.h>

namespace venom
{
namespace common
{
class Filesystem
{
public:
    static inline bool Exists(const char* path) {
        return std::filesystem::exists(path);
    }
};
}
}
