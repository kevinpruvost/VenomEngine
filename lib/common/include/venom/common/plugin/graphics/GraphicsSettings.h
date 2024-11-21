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

    static vc::Error ManageGfxSettingsLoadingQueue();

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

    static vc::Error SetHDR(bool enable);
    static bool IsHDREnabled();
    static bool IsHDRSupported();

    static void StartGfxSettingsChange();
    static vc::Error EndGfxSettingsChange();
    static int GetSamplesMultisampling();
    static const vc::Vector<MultiSamplingCountOption> & GetAvailableMultisamplingOptions();

protected:
    virtual vc::Error _LoadGfxSettings() = 0;

    virtual vc::Error _SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples) = 0;
    virtual vc::Vector<MultiSamplingCountOption> _GetAvailableMultisamplingOptions() = 0;

    virtual vc::Error _SetHDR(bool enable) = 0;

protected:
    enum class GfxSettingsChangeState
    {
        Ended,
        Started
    };
    GfxSettingsChangeState _gfxSettingsChangeState;

    MultiSamplingModeOption _samplingMode;
    int _samples;
    bool _multisamplingDirty;
    bool _isHdrSupported;

private:
    static vc::Error __LoadGfxSettings();
    void __AddLoadGFXSettingsToQueue();

    bool __isHdrEnabled;
    bool __loadingQueued;

private:
    vc::Vector<MultiSamplingCountOption> __availableMultisamplingOptions;
};
}
}
