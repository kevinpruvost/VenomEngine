///
/// Project: VenomEngineWorkspace
/// @file Device.mm
/// @date Mar, 07 2025
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/Device.h>

#include <venom/common/Config.h>
#include <venom/common/context/ContextApple.h>

namespace venom
{
namespace metal
{
id<MTLDevice> GetMetalDevice()
{
    return getGlobalMetalDevice();
}

vc::Error InitializeDevice()
{
    return vc::Error::Success;
}
void DestroyDevice()
{
}
}
}
