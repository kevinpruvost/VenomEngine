///
/// Project: VenomEngineWorkspace
/// @file Device.mm
/// @date Mar, 07 2025
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/Device.h>

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
    NSLog(@"Retain count after creation: %lu", [MetalDevice retainCount]);
    return MetalDevice == nil ? vc::Error::Failure : vc::Error::Success;
}
void DestroyDevice()
{
    NSLog(@"Retain count before retain: %lu", [MetalDevice retainCount]);
    [MetalDevice release];
    MetalDevice = nil;
}
}
}
