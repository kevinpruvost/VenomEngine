///
/// Project: VenomEngineWorkspace
/// @file ContextAppleInfo.h
/// @date Mar, 11 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Platform.h>

#ifdef __OBJC__

#if defined(VENOM_PLATFORM_MACOS)
#import <Cocoa/Cocoa.h>
#define AppleWindow NSWindow
#define AppleView NSView
#define AppleScreen NSScreen
#else
#import <UIKit/UIKit.h>
#import <TargetConditionals.h>
#import <Availability.h>
#define AppleWindow UIWindow
#define AppleView UIView
#define AppleScreen UIScreen
#endif
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <Foundation/Foundation.h>

// Declare global variables for the Metal device and layer
extern id<MTLDevice> sharedMetalDevice;
extern CAMetalLayer *sharedMetalLayer;

void setGlobaMetallDevice(id<MTLDevice> device);
void setGlobalMetalLayer(CAMetalLayer *layer);
id<MTLDevice> getGlobalMetalDevice(void);
CAMetalLayer *getGlobalMetalLayer(void);

#endif
