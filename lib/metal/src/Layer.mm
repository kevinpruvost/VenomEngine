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
    return vc::Error::Success;
}

void DestroyLayer()
{
}
}
}
