///
/// Project: VenomEngineWorkspace
/// @file CommandQueue.h
/// @date Mar, 07 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/metal/Device.h>

namespace venom
{
namespace metal
{
id <MTLCommandQueue> GetMetalCommandQueue();
vc::Error InitializeCommandQueue();
void DestroyCommandQueue();
}
}