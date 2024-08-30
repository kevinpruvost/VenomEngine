///
/// Project: VenomEngine
/// @file PluginObject.cc
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/PluginObject.h>

#include <venom/common/VenomEngine.h>
#include <venom/common/Debug.h>

#include "venom/common/Log.h"

namespace venom
{
namespace common
{
PluginObject::PluginObject(const PluginType type)
    : __type(type)
{
    VenomEngine::GetInstance()->pluginManager->AddPluginObject(__type, this);
}

PluginObject::~PluginObject()
{
}

void PluginObject::Destroy()
{
    VenomEngine::GetInstance()->pluginManager->RemovePluginObject(__type, this);
}

}
}
