///
/// Project: VenomEngineWorkspace
/// @file GraphicsSettings.h
/// @date Oct, 28 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/Error.h>
#include <venom/common/Export.h>
#include <venom/common/Containers.h>

namespace venom
{
namespace common
{
/**
 * Interface of GraphicsApplication class to set the graphics settings.
 * Includable in any plugin, useful to get access to enum and other things...
 */
class VENOM_COMMON_API GraphicsSettings
{
public:
    GraphicsSettings();
    virtual ~GraphicsSettings();

    enum class MultiSamplingModeOption
    {
        None = 1,
        MSAA = 2
    };
    enum class MultiSamplingCountOption
    {
        None = 1,
        Samples2 = 2,
        Samples4 = 4,
        Samples8 = 8,
        Samples16 = 16,
        Samples32 = 32,
    };
    static vc::Error SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples);
    static vc::Error LoadGfxSettings();

    static void StartGfxSettingsChange();
    static vc::Error EndGfxSettingsChange();
    static int GetSamplesMultisampling();
    const vc::Vector<MultiSamplingCountOption> & GetAvailableMultisamplingOptions();

protected:
    virtual vc::Error _SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples) = 0;
    virtual vc::Error _LoadGfxSettings() = 0;
    virtual vc::Vector<MultiSamplingCountOption> _GetAvailableMultisamplingOptions() = 0;

protected:
    enum class GfxSettingsChangeState
    {
        Ended,
        Started
    };
    GfxSettingsChangeState _gfxSettingsChangeState;

    int _samples;
    bool _multisamplingDirty;

private:
    vc::Vector<MultiSamplingCountOption> __availableMultisamplingOptions;
};
}
}
