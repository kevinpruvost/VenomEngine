///
/// Project: VenomEngineWorkspace
/// @file ContextAppleWindow.h
/// @date Mar, 16 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Platform.h>
#include <venom/common/context/apple/ContextAppleInfo.h>

#if defined(VENOM_PLATFORM_IOS)
#define AppleWindowController UIViewController
#else
#define AppleWindowController NSWindowController
#endif

@interface ContextAppleWindowController : AppleWindowController

@end
