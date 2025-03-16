///
/// Project: VenomEngineWorkspace
/// @file ContextAppleDelegate.h
/// @date Mar, 11 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/context/ContextApple.h>
#include <venom/common/context/apple/ContextAppleInfo.h>
#include <venom/common/context/apple/ContextAppleWindowController.h>

#if defined(VENOM_PLATFORM_IOS)
#import <UIKit/UIKit.h>
#define PlatformAppDelegate UIResponder <UIApplicationDelegate>
#else
#import <AppKit/AppKit.h>
#define PlatformAppDelegate NSObject <NSApplicationDelegate>
#endif

@interface ContextAppleDelegate : PlatformAppDelegate

@property (nonatomic, strong) AppleWindow * window;
@property (nonatomic, strong) AppleWindowController * windowController;
@property (nonatomic, strong) AppleView * view;

@end
