///
/// Project: VenomEngineWorkspace
/// @file ContextPlugin.h
/// @date Mar, 09 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/Plugin.h>

namespace venom
{
namespace common
{

class Context;

class VENOM_COMMON_API ContextPlugin : public Plugin
{
public:
    ContextPlugin();
    ~ContextPlugin() override;

    static ContextPlugin * Get();

    // Graphics objects
    virtual Context * CreateContext() = 0;

private:
};
}
}