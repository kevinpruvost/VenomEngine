///
/// Project: Bazel_Vulkan_Metal
/// @file VenomEngine.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/PluginManager.h>

#include <venom/common/Singleton.h>

namespace venom
{
namespace common
{
class VENOM_COMMON_API VenomEngine : public Singleton<VenomEngine>
{
    friend class Singleton<VenomEngine>;
private:
    VenomEngine();
public:
    ~VenomEngine();

public:
    PluginManager pluginManager;
};
}
}
