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
struct GraphicsSettingsData
{
    int screenWidth;
    int screenHeight;
    int multisamplingMode;
    int multisamplingSamples;
    int hdrEnabled;
};

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

    static vc::Error SetHDR(bool enable);
    static bool IsHDREnabled();
    static bool IsHDRSupported();

    static void StartGfxSettingsChange();
    static vc::Error EndGfxSettingsChange();
    static int GetActiveSamplesMultisampling();
    static MultiSamplingModeOption GetActiveMultisamplingMode();
    static MultiSamplingCountOption GetActiveMultisamplingCount();
    static int GetActiveMultisamplingCountIndex();
    static const vc::Vector<MultiSamplingCountOption> & GetAvailableMultisamplingCountOptions();
    static const vc::Vector<vc::String> & GetAvailableMultisamplingCountOptionsStrings();

    static const GraphicsSettingsData * GetGfxSettingsDataPtr();
    static void SetWindowResolution(int width, int height);

    static void ReloadGFXSettings();

protected:
    static bool _IsGfxSettingsDataDirty();

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
    bool _multisamplingDirty;
    bool _isHdrSupported;

    bool _gfxSettingsChangeQueued;

private:
    static vc::Error __LoadGfxSettings();
    void __AddLoadGFXSettingsToQueue();

    bool __isHdrEnabled;

    GraphicsSettingsData __gfxSettingsData;
    bool __gfxSettingsDataDirty;

    vc::Vector<vc::String> __availableMultisamplingCountsStrings;

private:
    vc::Vector<MultiSamplingCountOption> __availableMultisamplingOptions;
};
}
}
