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
#define VENOM_MAX_DYNAMIC_TEXTURES 32
#define VENOM_MAX_LIGHTS 256
#define VENOM_BITS_PER_INT (sizeof(int) * 8)
#define VENOM_NUM_FORWARD_PLUS_INTS ((int)((VENOM_MAX_LIGHTS + VENOM_BITS_PER_INT - 1) / VENOM_BITS_PER_INT))


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

    bool IsDebug() const;

private:
    const bool __isDebug;
};
}
}