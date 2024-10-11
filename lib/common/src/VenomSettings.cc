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

#ifdef VENOM_DEBUG
#define IS_VENOM_DEBUG true
#else
#define IS_VENOM_DEBUG false
#endif

namespace venom
{
namespace common
{
static VenomSettings * s_settings = nullptr;

static constexpr int s_framesInFlight = 3;

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

bool VenomSettings::IsDebug() const
{
    return __isDebug;
}
}
}
