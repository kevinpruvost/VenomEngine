///
/// Project: VenomEngineWorkspace
/// @file SceneSettings.h
/// @date Nov, 30 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

namespace venom
{
namespace common
{
struct SceneSettingsData
{
    float targetLuminance;
};

class VenomEngine;
class SceneSettings
{
public:
    SceneSettings();
    ~SceneSettings();

    inline static void SetTargetLuminance(const float targetLuminance) { __Get()->__SetTargetLuminance(targetLuminance); }
    inline static float GetTargetLuminance() { return __Get()->__GetTargetLuminance(); }

    inline static const SceneSettingsData * GetCurrentSettingsData() { return __Get()->__GetDataPtr(); }

    /**
     * @brief Check if the data has been modified
     * @warning This will reset the dirty flag
     * @return dirty or not
     */
    inline static bool IsDataDirty() { return __Get()->__IsDataDirty(); }

private:
    static SceneSettings * __Get();

    inline const SceneSettingsData * __GetDataPtr() const { return &__data; }

    inline void __SetTargetLuminance(const float targetLuminance) { __data.targetLuminance = targetLuminance; __dirty = true; }
    inline float __GetTargetLuminance() const { return __data.targetLuminance; }

    inline bool __IsDataDirty() { const bool ret = __dirty; __dirty = false; return ret; }

private:
    SceneSettingsData __data;
    bool __dirty;

    friend class VenomEngine;
};
}
}