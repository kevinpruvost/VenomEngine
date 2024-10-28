///
/// Project: VenomEngineWorkspace
/// @file GraphicsSettings.cc
/// @date Oct, 28 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GraphicsSettings.h>
#include <venom/common/Log.h>

namespace venom
{
namespace common
{
static GraphicsSettings * s_graphicsSettings = nullptr;
GraphicsSettings::GraphicsSettings()
    : _gfxSettingsChangeState(GfxSettingsChangeState::Ended)
    , _multisamplingDirty(false)
    , _samples(1)
{
    venom_assert(s_graphicsSettings == nullptr, "GraphicsSettings is a singleton.");
    s_graphicsSettings = this;
}

GraphicsSettings::~GraphicsSettings()
{
    s_graphicsSettings = nullptr;
}

vc::Error GraphicsSettings::SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples)
{
    vc::Error err = s_graphicsSettings->_SetMultiSampling(mode, samples);
    if (err != vc::Error::Success)
        return err;
    s_graphicsSettings->_multisamplingDirty = true;
    s_graphicsSettings->_samples = static_cast<int>(samples);
    if (s_graphicsSettings->_gfxSettingsChangeState == GfxSettingsChangeState::Ended)
        return LoadGfxSettings();
    return err;
}

vc::Error GraphicsSettings::LoadGfxSettings()
{
    vc::Error err = s_graphicsSettings->_LoadGfxSettings();
    if (err != vc::Error::Success)
        return err;
    s_graphicsSettings->_multisamplingDirty = false;
    return err;
}

void GraphicsSettings::StartGfxSettingsChange()
{
    s_graphicsSettings->_gfxSettingsChangeState = GfxSettingsChangeState::Started;
}

vc::Error GraphicsSettings::EndGfxSettingsChange()
{
    s_graphicsSettings->_gfxSettingsChangeState = GfxSettingsChangeState::Ended;
    return LoadGfxSettings();
}

int GraphicsSettings::GetSamplesMultisampling()
{
    return s_graphicsSettings->_samples;
}

const vc::Vector<GraphicsSettings::MultiSamplingCountOption> & GraphicsSettings::GetAvailableMultisamplingOptions()
{
    if (__availableMultisamplingOptions.empty()) {
        __availableMultisamplingOptions = _GetAvailableMultisamplingOptions();
    }
    return __availableMultisamplingOptions;
}
}
}
