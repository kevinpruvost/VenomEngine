///
/// Project: VenomEngineWorkspace
/// @file RenderTarget.cc
/// @date Dec, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/RenderTarget.h>

#include "venom/common/plugin/graphics/GraphicsSettings.h"

namespace venom
{
namespace metal
{
MetalRenderTarget::MetalRenderTarget()
{
}

vc::Error MetalRenderTarget::__PrepareRenderTarget()
{
    return vc::Error::Success;
}
}
}
