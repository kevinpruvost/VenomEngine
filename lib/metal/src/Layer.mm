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
static CAMetalLayer* MetalLayer = nil;
CAMetalLayer* GetMetalLayer()
{
    return MetalLayer;
}
vc::Error InitializeLayer()
{
    MetalLayer = [CAMetalLayer layer];
    MetalLayer.device = GetMetalDevice();
    if (vc::Config::GetContextType() == vc::Context::ContextType::Apple)
        venom::context::apple::ContextApple::GetAppleContext()->SetMetalLayer(MetalLayer);
    return MetalLayer == nil ? vc::Error::Failure : vc::Error::Success;
}

void DestroyLayer()
{
    [MetalLayer release];
    MetalLayer = nil;
}
}
}
