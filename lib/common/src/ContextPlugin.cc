///
/// Project: VenomEngineWorkspace
/// @file ContextPlugin.cc
/// @date Mar, 09 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/context/ContextPlugin.h>

#include <venom/common/Log.h>
#include <venom/common/VenomEngine.h>

namespace venom
{
namespace common
{

ContextPlugin::ContextPlugin()
    : Plugin(PluginType::Context)
{
}

ContextPlugin::~ContextPlugin()
{
}

ContextPlugin* ContextPlugin::Get()
{
    return VenomEngine::GetInstance()->pluginManager->GetContextPlugin();
}
}
}