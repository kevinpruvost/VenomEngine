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
#include <venom/common/Callback.h>

namespace venom
{
namespace common
{
struct GraphicsSettingsData
{
    int screenWidth;
    int screenHeight;
    int extentWidth;
    int extentHeight;
    int multisamplingMode;
    int multisamplingSamples;
    int hdrEnabled;
    int debugVisualizationMode;
};

typedef Callback<void> GraphicsCallback;

/**
 * Interface of GraphicsApplication class to set the graphics settings.
 * Includable in any plugin, useful to get access to enum and other things...
 */
class VENOM_COMMON_API GraphicsSettings
{
public:
    GraphicsSettings();
    virtual ~GraphicsSettings();

    /**
     * Multisampling
     */
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
    static MultiSamplingModeOption GetActiveMultisamplingMode();
    static MultiSamplingCountOption GetActiveMultisamplingCount();
    static int GetActiveMultisamplingCountIndex();
    static const vc::Vector<MultiSamplingCountOption> & GetAvailableMultisamplingCountOptions();
    static const vc::Vector<vc::String> & GetAvailableMultisamplingCountOptionsStrings();
    static int GetActiveSamplesMultisampling();

    /**
    * HDR
    */
    static vc::Error SetHDR(bool enable);
    static bool IsHDREnabled();
    static bool IsHDRSupported();

    /**
     * General GFX Settings
     */
    static void StartGfxSettingsChange();
    static vc::Error EndGfxSettingsChange();
    static const GraphicsSettingsData * GetGfxSettingsDataPtr();

    /**
     * Debug
     */
    enum class DebugVisualizationMode
    {
        None = 0,
        Depth = 1,
        Normals = 2,
        ForwardPlus = 3,
        ShadowMapping = 4,
        Count
    };
    static const vc::Vector<vc::String> & GetDebugVisualizerStrings();
    static void SetDebugVisualizationMode(DebugVisualizationMode mode);
    static DebugVisualizationMode GetDebugVisualizationMode();

    static void SetWindowResolution(int width, int height);
    static void SetWindowExtent(int width, int height);

    static void ReloadGFXSettings();

    static void CallbackAfterDraws(const GraphicsCallback callback);
    inline bool HasCallbacksAfterDraws() const { return !__callbacksAfterDraws.empty(); }
    void LaunchCallbacksAfterDraws();

protected:
    static bool _IsGfxSettingsDataDirty();
    static bool _IsGfxConstantsDataDirty();
    virtual vc::Error _OnGfxSettingsChange() = 0;
    virtual vc::Error _OnGfxConstantsChange() = 0;

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
    bool _hdrDirty;
    bool _windowSizeDirty;
    bool _isHdrSupported;

    bool _gfxSettingsChangeQueued;

private:
    static vc::Error __LoadGfxSettings();
    void __AddLoadGFXSettingsToQueue();

    bool __isHdrEnabled;

    GraphicsSettingsData __gfxSettingsData;
    bool __gfxSettingsDataDirty;
    bool __gfxConstantsDataDirty;

    vc::Vector<vc::String> __availableMultisamplingCountsStrings;
    const vc::Vector<vc::String> __debugVisualizerStrings;
    vc::Vector<GraphicsCallback> __callbacksAfterDraws;

private:
    vc::Vector<MultiSamplingCountOption> __availableMultisamplingOptions;
};
}
}
