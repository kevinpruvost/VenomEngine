///
/// Project: VenomEngineWorkspace
/// @file Layer.h
/// @date Mar, 07 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Error.h>

#ifdef __OBJC__
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

namespace venom
{
namespace metal
{
CAMetalLayer * GetMetalLayer();
vc::Error InitializeLayer();
void DestroyLayer();
}
}
#endif
