///
/// Project: VenomEngineWorkspace
/// @file CommandQueue.mm
/// @date Mar, 07 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#include <venom/metal/CommandQueue.h>

namespace venom
{
namespace metal
{
id<MTLCommandQueue> MetalCommandQueue = nil;

id<MTLCommandQueue> GetMetalCommandQueue()
{
    return MetalCommandQueue;
}

vc::Error InitializeCommandQueue()
{
    MetalCommandQueue = [GetMetalDevice() newCommandQueue];
    return MetalCommandQueue == nil ? vc::Error::Failure : vc::Error::Success;
}

void DestroyCommandQueue()
{
    [MetalCommandQueue release];
    MetalCommandQueue = nil;
}
}
}
