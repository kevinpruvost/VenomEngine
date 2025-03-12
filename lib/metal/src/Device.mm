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
static id<MTLDevice> MetalDevice = nil;

id<MTLDevice> GetMetalDevice()
{
    return MetalDevice;
}

vc::Error InitializeDevice()
{
    MetalDevice = MTLCreateSystemDefaultDevice();
    if (vc::Config::GetContextType() == vc::Context::ContextType::Apple)
        setGlobaMetallDevice(MetalDevice);
    return MetalDevice == nil ? vc::Error::Failure : vc::Error::Success;
}
void DestroyDevice()
{
    [MetalDevice release];
    MetalDevice = nil;
    if (vc::Config::GetContextType() == vc::Context::ContextType::Apple)
        setGlobaMetallDevice(nil);
}
}
}
