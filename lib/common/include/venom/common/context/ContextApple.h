///
/// Project: VenomEngineWorkspace
/// @file ContextApple.h
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Context.h>

#if defined(VENOM_PLATFORM_APPLE)

#ifdef __OBJC__

#if TARGET_OS_OSX
#import <Cocoa/Cocoa.h>
#define AppleWindow NSWindow
#define AppleView NSView
#else
#import <UIKit/UIKit.h>
#define AppleWindow UIWindow
#define AppleView UIView
#endif

@interface ContextAppleData : NSObject
@end

#endif

namespace venom
{
namespace context
{
namespace apple
{
class VENOM_COMMON_API ContextApple : public vc::Context
{
public:
    ContextApple();
    ~ContextApple();

    static ContextApple * GetAppleContext();
    void _SetWindowTitle(const char* title) override;
    void * _GetWindow() override;
    vc::Error _InitContext() override;
    vc::Error _UpdateVideoMode() override;
    vc::Error _UpdateRefreshRate() override;
    vc::Error _UpdateScreen() override;
    vc::Error _SetFullscreen() override;

protected:
    bool _ShouldClose() override;
    void _GetCursorPos(double * pos) override;
    void _PollEvents() override;

private:
    int __ConvertKeyboardInputToAppleEnum(vc::KeyboardInput key) const;
    vc::KeyboardInput __ConvertAppleEnumToKeyboardInput(int key) const;

private:
    void * __contextAppleData;
#ifdef __OBJC__
    ContextAppleData * GetData();
    AppleWindow * __GetWindow();
#endif
};
}
}
}

#endif
