///
/// Project: VenomEngine
/// @file GraphicsPlugin.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GraphicsPlugin.h>

#include <venom/common/Log.h>
#include <venom/common/VenomEngine.h>

namespace venom
{
namespace common
{
GraphicsPlugin::GraphicsPlugin()
    : Plugin(PluginType::Graphics)
{
    __graphicsResourceCache.reset(new vc::UMap<vc::String, vc::SPtr<GraphicsCachedResource>>());
}

GraphicsPlugin::~GraphicsPlugin()
{
}

GraphicsPlugin* GraphicsPlugin::Get()
{
    return VenomEngine::GetInstance()->pluginManager->GetGraphicsPlugin();
}
}
}
