///
/// Project: VenomEngineWorkspace
/// @file SceneSettings.cc
/// @date Nov, 30 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/SceneSettings.h>
#include <venom/common/Log.h>

namespace venom
{
namespace common
{
static SceneSettings * s_sceneSettings = nullptr;

SceneSettings::SceneSettings()
    : __data{
        .targetLuminance = 100.0f
    }
    , __dirty(true)
{
    venom_assert(s_sceneSettings == nullptr, "SceneSettings already initialized");
    s_sceneSettings = this;
}

SceneSettings::~SceneSettings()
{
    venom_assert(s_sceneSettings != nullptr, "SceneSettings not initialized");
    s_sceneSettings = nullptr;
}

SceneSettings* SceneSettings::__Get()
{
    return s_sceneSettings;
}
}
}
