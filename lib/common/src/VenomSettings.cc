///
/// Project: VenomEngineWorkspace
/// @file VenomSettings.cc
/// @date Oct, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/VenomSettings.h>
#include <venom/common/Log.h>
#include <venom/common/Debug.h>

#if defined(VENOM_DEBUG)
#define IS_VENOM_DEBUG true
#else
#define IS_VENOM_DEBUG false
#endif

namespace venom
{
namespace common
{
static VenomSettings * s_settings = nullptr;

VenomSettings::VenomSettings()
    : __isDebug(IS_VENOM_DEBUG)
{
    venom_assert(s_settings == nullptr, "VenomSettings already exists");
    s_settings = this;
}

VenomSettings::~VenomSettings()
{
    s_settings = nullptr;
}

VenomSettings* VenomSettings::Get()
{
    return s_settings;
}

uint64_t VenomSettings::GetTotalShadowMapGPUSize()
{
    // For float16
    constexpr const uint64_t pixelSize = VENOM_CSM_TOTAL_CASCADES * VENOM_MAX_FRAMES_IN_FLIGHT * 2;
    return pixelSize * (VENOM_CSM_MAX_DIRECTIONAL_LIGHTS * VENOM_CSM_DIRECTIONAL_DIMENSION * VENOM_CSM_DIRECTIONAL_DIMENSION +
                        VENOM_CSM_MAX_SPOT_LIGHTS * VENOM_CSM_SPOT_DIMENSION * VENOM_CSM_SPOT_DIMENSION +
                        6 * VENOM_CSM_MAX_POINT_LIGHTS * VENOM_CSM_POINT_DIMENSION * VENOM_CSM_POINT_DIMENSION);
}

bool VenomSettings::IsDebug() const
{
    return __isDebug;
}
}
}
