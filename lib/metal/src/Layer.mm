///
/// Project: VenomEngineWorkspace
/// @file Layer.mm
/// @date Mar, 07 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/Layer.h>
#include <venom/metal/Device.h>

#include <venom/common/Config.h>
#include <venom/common/context/ContextApple.h>

namespace venom
{
namespace metal
{
CAMetalLayer* GetMetalLayer()
{
    return getGlobalMetalLayer();
}
vc::Error InitializeLayer()
{
    CAMetalLayer * MetalLayer = [CAMetalLayer layer];
    MetalLayer.device = GetMetalDevice();
    setGlobalMetalLayer(MetalLayer);
    return MetalLayer == nil ? vc::Error::Failure : vc::Error::Success;
}

void DestroyLayer()
{
    CAMetalLayer * MetalLayer = getGlobalMetalLayer();
    [MetalLayer release];
    MetalLayer = nil;
    setGlobalMetalLayer(nil);
}
}
}
