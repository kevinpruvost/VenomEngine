///
/// Project: VenomEngineWorkspace
/// @file Device.h
/// @date Mar, 07 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Error.h>

#ifdef __OBJC__

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#import <Foundation/Foundation.h>

namespace venom
{
namespace metal
{
id <MTLDevice> GetMetalDevice();
vc::Error InitializeDevice();
void DestroyDevice();
}
}

#endif
