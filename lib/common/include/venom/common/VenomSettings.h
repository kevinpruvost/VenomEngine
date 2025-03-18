///
/// Project: VenomEngineWorkspace
/// @file VenomSettings.h
/// @date Oct, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/DLL.h>

#define VENOM_MAX_FRAMES_IN_FLIGHT 3
#define VENOM_MAX_ENTITIES 65536

#define VENOM_MAX_BINDLESS_TEXTURES 4096
#define VENOM_MAX_SAMPLED_IMAGES 128 // Limit on macOS + iOS systems
#define VENOM_MAX_DYNAMIC_TEXTURES 32 // For materials
#define VENOM_MAX_SHADOW_MAPS (VENOM_MAX_SAMPLED_IMAGES - VENOM_MAX_DYNAMIC_TEXTURES)
#define VENOM_MAX_LIGHTS 128
#define VENOM_MAX_DIRECTIONAL_LIGHTS 4
#define VENOM_MAX_SPOT_LIGHTS (VENOM_MAX_LIGHTS >> 2) // 32
#define VENOM_MAX_POINT_LIGHTS (VENOM_MAX_LIGHTS - VENOM_MAX_DIRECTIONAL_LIGHTS - VENOM_MAX_SPOT_LIGHTS) // 92
// 16 in total because on a MacBook Pro, we can only use 8 storage images per stage
#define VENOM_MAX_SHADOWED_DIRECTIONAL_LIGHTS 1 // 1
#define VENOM_MAX_SHADOWED_SPOT_LIGHTS 3 // 6
#define VENOM_MAX_SHADOWED_POINT_LIGHTS 4 // 10 * 6 = 60
// Total = 67

// Only shadow-casting ones
#define VENOM_CSM_MAX_DIRECTIONAL_LIGHTS (VENOM_MAX_SHADOWED_DIRECTIONAL_LIGHTS)
// Only shadow-casting ones
#define VENOM_CSM_MAX_SPOT_LIGHTS (VENOM_MAX_SHADOWED_SPOT_LIGHTS)
// Only shadow-casting ones
#define VENOM_CSM_MAX_POINT_LIGHTS (VENOM_MAX_SHADOWED_POINT_LIGHTS)
#define VENOM_CSM_TOTAL_CASCADES 3
#define VENOM_CSM_DIMENSION (2048)
#define VENOM_CSM_DIRECTIONAL_DIMENSION (VENOM_CSM_DIMENSION << 2)
#define VENOM_CSM_POINT_DIMENSION (VENOM_CSM_DIMENSION >> 1)
#define VENOM_CSM_SPOT_DIMENSION (VENOM_CSM_DIMENSION)
#define VENOM_BITS_PER_INT (sizeof(int) * 8)
#define VENOM_NUM_FORWARD_PLUS_INTS ((int)((VENOM_MAX_LIGHTS + VENOM_BITS_PER_INT - 1) / VENOM_BITS_PER_INT))

// CSM Total Cascades must be greater or equal than 1
static_assert(VENOM_CSM_TOTAL_CASCADES >= 1, "CSM Total Cascades must be greater or equal than 1");
// CSM Dimension lowest size must be greater or equal than 64
static_assert((VENOM_CSM_POINT_DIMENSION >> (VENOM_CSM_TOTAL_CASCADES - 1)) >= 64, "CSM lowest size must be greater or equal than 64");
// CSM Dimension must also be a power of 2
static_assert((VENOM_CSM_DIMENSION & (VENOM_CSM_DIMENSION - 1)) == 0, "CSM Dimension must be a power of 2");

namespace venom
{
namespace common
{
class VENOM_COMMON_API VenomSettings
{
public:
    VenomSettings();
    ~VenomSettings();

    static VenomSettings * Get();
    static uint64_t GetTotalShadowMapGPUSize();

    bool IsDebug() const;
private:
    const bool __isDebug;
};
}
}
