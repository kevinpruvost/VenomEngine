///
/// Project: VenomEngineWorkspace
/// @file Layer.mm
/// @date Mar, 07 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/Layer.h>
#include <venom/metal/Device.h>

namespace venom
{
namespace metal
{
static CAMetalLayer* MetalLayer = nil;
CAMetalLayer* GetMetalLayer()
{
    return MetalLayer;
}
vc::Error InitializeLayer()
{
    MetalLayer = [CAMetalLayer layer];
    MetalLayer.device = GetMetalDevice();
    return MetalLayer == nil ? vc::Error::Failure : vc::Error::Success;
}

void DestroyLayer()
{
    [MetalLayer release];
    MetalLayer = nil;
}
}
}
