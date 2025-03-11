///
/// Project: VenomEngineWorkspace
/// @file ContextAppleViewController.h
/// @date Mar, 11 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Platform.h>
#include <venom/common/context/apple/ContextAppleInfo.h>

#if defined(VENOM_PLATFORM_IOS)
#import <UIKit/UIKit.h>
#define PlatformViewController UIViewController
#else
#import <AppKit/AppKit.h>
#define PlatformViewController NSViewController
#endif

#import <MetalKit/MetalKit.h>

// The view controller
@interface ContextAppleViewController : PlatformViewController <MTKViewDelegate>

@property id<MTLDevice> device;
@property CAMetalLayer * layer;

@end
