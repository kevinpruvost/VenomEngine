///
/// Project: VenomEngine
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
    static Error RunEngine(char ** argv);

public:
    std::unique_ptr<PluginManager> pluginManager;

private:
    std::unique_ptr<DLL_Cache> __dllCache;
};
}
}
