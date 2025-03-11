///
/// Project: VenomEngineWorkspace
/// @file ContextAppleInfo.h
/// @date Mar, 11 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#ifdef __OBJC__

#if defined(VENOM_PLATFORM_MACOS)
#import <Cocoa/Cocoa.h>
#define AppleWindow NSWindow
#define AppleView NSView
#else
#import <UIKit/UIKit.h>
#define AppleWindow UIWindow
#define AppleView UIView
#endif
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <Foundation/Foundation.h>

#endif