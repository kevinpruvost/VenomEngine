///
/// Project: VenomEngineWorkspace
/// @file Platform.h
/// @date Mar, 09 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#if defined(_WIN32)
    #define VENOM_PLATFORM_WINDOWS
    #define VENOM_PLATFORM_DESKTOP
#endif

#if defined(__linux__)
    #define VENOM_PLATFORM_LINUX
    #define VENOM_PLATFORM_DESKTOP
#endif

#if TARGET_OS_OSX
    #define VENOM_PLATFORM_MACOS
    #define VENOM_PLATFORM_DESKTOP
#endif

#if TARGET_OS_IOS
    #define VENOM_PLATFORM_IOS
    #define VENOM_PLATFORM_MOBILE

    // Disable GLFW if iOS because GLFW does not work there
    #define VENOM_DISABLE_GLFW
#endif

#if TARGET_OS_OSX || TARGET_OS_IOS
    #define VENOM_PLATFORM_APPLE
// IOS is always a package
#if TARGET_OS_OSX || TARGET_OS_IOS
    #define VENOM_APPLE_PACKAGE
#endif
#endif

namespace venom
{
namespace common
{

}
}
