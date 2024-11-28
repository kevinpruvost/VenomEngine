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
    , _isHdrSupported(false)
    , __isHdrEnabled(false)
    , _gfxSettingsChangeQueued(false)
    , _samplingMode(MultiSamplingModeOption::None)
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
    s_graphicsSettings->_multisamplingDirty = true;
    s_graphicsSettings->_samples = static_cast<int>(samples);
    s_graphicsSettings->_samplingMode = mode;
    if (s_graphicsSettings->_gfxSettingsChangeState == GfxSettingsChangeState::Ended)
        s_graphicsSettings->__AddLoadGFXSettingsToQueue();
    if (s_graphicsSettings->_SetMultiSampling(mode, samples) != vc::Error::Success)
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error GraphicsSettings::SetHDR(bool enable)
{
    if (!IsHDRSupported()) return vc::Error::FeatureNotSupported;
    if (IsHDREnabled() == enable) return vc::Error::Success;

    vc::Error err = s_graphicsSettings->_SetHDR(enable);
    if (err != vc::Error::Success)
        return err;
    s_graphicsSettings->__isHdrEnabled = enable;
    return err;
}

bool GraphicsSettings::IsHDREnabled()
{
    return s_graphicsSettings->__isHdrEnabled;
}

bool GraphicsSettings::IsHDRSupported()
{
    return s_graphicsSettings->_isHdrSupported;
}

vc::Error GraphicsSettings::__LoadGfxSettings()
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
    return __LoadGfxSettings();
}

int GraphicsSettings::GetActiveSamplesMultisampling()
{
    return s_graphicsSettings->_samples;
}

GraphicsSettings::MultiSamplingModeOption GraphicsSettings::GetActiveMultisamplingMode()
{
    return s_graphicsSettings->_samplingMode;
}

GraphicsSettings::MultiSamplingCountOption GraphicsSettings::GetActiveMultisamplingCount()
{
    return static_cast<GraphicsSettings::MultiSamplingCountOption>(s_graphicsSettings->_samples);
}

int GraphicsSettings::GetActiveMultisamplingCountIndex()
{
    const auto & availableMultisamplingOptions = s_graphicsSettings->GetAvailableMultisamplingCountOptions();
    return static_cast<int>(std::distance(availableMultisamplingOptions.begin(),
                                          std::find(availableMultisamplingOptions.begin(), availableMultisamplingOptions.end(),
                                                    GetActiveMultisamplingCount())));
}

const vc::Vector<GraphicsSettings::MultiSamplingCountOption> & GraphicsSettings::GetAvailableMultisamplingCountOptions()
{
    if (s_graphicsSettings->__availableMultisamplingOptions.empty()) {
        s_graphicsSettings->__availableMultisamplingOptions = s_graphicsSettings->_GetAvailableMultisamplingOptions();
        s_graphicsSettings->__availableMultisamplingCountsStrings.clear();
        s_graphicsSettings->__availableMultisamplingCountsStrings.reserve(s_graphicsSettings->__availableMultisamplingOptions.size());
        for (const auto & option : s_graphicsSettings->__availableMultisamplingOptions) {
            s_graphicsSettings->__availableMultisamplingCountsStrings.emplace_back(format("x%d", static_cast<int>(option)));
        }
    }
    return s_graphicsSettings->__availableMultisamplingOptions;
}

const vc::Vector<vc::String>& GraphicsSettings::GetAvailableMultisamplingCountOptionsStrings()
{
    return s_graphicsSettings->__availableMultisamplingCountsStrings;
}

void GraphicsSettings::ReloadGFXSettings()
{
    s_graphicsSettings->__AddLoadGFXSettingsToQueue();
}

void GraphicsSettings::__AddLoadGFXSettingsToQueue()
{
    _gfxSettingsChangeQueued = true;
}
}
}
