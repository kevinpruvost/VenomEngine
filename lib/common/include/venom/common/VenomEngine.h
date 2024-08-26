///
/// Project: Bazel_Vulkan_Metal
/// @file VenomEngine.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/PluginManager.h>

namespace venom
{
namespace common
{
class VENOM_COMMON_API VenomEngine
{
private:
    VenomEngine();
public:
    ~VenomEngine();
    static VenomEngine * GetInstance();

public:
    PluginManager pluginManager;
};
}
}
