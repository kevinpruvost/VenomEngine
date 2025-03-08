///
/// Project: VenomEngineWorkspace
/// @file ContextPlugin.h
/// @date Mar, 09 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/context/ContextPlugin.h>

namespace venom
{
namespace context
{
namespace glfw
{
class VENOM_CONTEXT_API GLFWContextPlugin : public vc::ContextPlugin
{
public:
    GLFWContextPlugin();
    ~GLFWContextPlugin() override;

    vc::Context * CreateContext() override;
};
}
}
}

extern "C" EXPORT vc::ContextPlugin * createContextPlugin();