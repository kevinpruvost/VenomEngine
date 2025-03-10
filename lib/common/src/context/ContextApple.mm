///
/// Project: VenomEngineWorkspace
/// @file Context_Apple.mm
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/context/ContextApple.h>

@implementation ContextAppleData
{
@public
    AppleWindow * __window;
    AppleView * __view;
}
@end

namespace venom
{
namespace context
{
namespace apple
{
ContextApple::ContextApple()
    : __contextAppleData([[ContextAppleData alloc] init])
{
}

ContextApple* ContextApple::GetAppleContext()
{
    return dynamic_cast<ContextApple *>(Get());
}

ContextAppleData * ContextApple::GetData()
{
    return (ContextAppleData *)__contextAppleData;
}

ContextApple::~ContextApple()
{
#ifdef TARGET_OS_OSX
    if (GetData()->__window)
        [GetData()->__window close];
#else
    if (GetData()->__window)
        [GetData()->__window setHidden:YES];
#endif
}

void ContextApple::_SetWindowTitle(const char* title)
{
#ifdef TARGET_OS_OSX
    [GetData()->__window setTitle:[NSString stringWithUTF8String:title]];
#else
    // No equivalent for iOS
    vc::Log::Error("Cannot change window title at runtime on iOS");
#endif
}

void* ContextApple::_GetWindow()
{
    return GetData()->__window;
}

AppleWindow* ContextApple::__GetWindow()
{
    return GetData()->__window;
}

vc::Error ContextApple::_InitContext()
{
    // Might move initialization in Metal
#ifdef TARGET_OS_OSX
    NSRect frame = NSMakeRect(100, 100, 800, 600);
    GetData()->__window = [[AppleWindow alloc] initWithContentRect:frame
                                            styleMask:(NSWindowStyleMaskTitled |
                                                       NSWindowStyleMaskClosable |
                                                       NSWindowStyleMaskResizable)
                                              backing:NSBackingStoreBuffered
                                                defer:NO];
    GetData()->__view = [[NSView alloc] initWithFrame: frame];
    [GetData()->__view wantsLayer: YES];
    [__GetWindow() setContentView: GetData()->__view];
    [__GetWindow() makeKeyAndOrderFront:nil];
#else
    GetData()->__window = [[AppleWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    [__GetWindow() makeKeyAndVisible];
#endif
    return vc::Error::Success;
}

vc::Error ContextApple::_UpdateVideoMode()
{
    return _UpdateScreen();
}

vc::Error ContextApple::_UpdateRefreshRate()
{
    return _UpdateScreen();
}

vc::Error ContextApple::_UpdateScreen()
{
    return vc::Error::Success;
}

vc::Error ContextApple::_SetFullscreen()
{
#ifdef TARGET_OS_OSX
    [__GetWindow() toggleFullScreen:nil];
#else
    // No equivalent for iOS
#endif
    return vc::Error::Success;
}

bool ContextApple::_ShouldClose()
{
#ifdef TARGET_OS_OSX
    return ![__GetWindow() isVisible];
#else
    return __GetWindow().isHidden;
#endif
}

void ContextApple::_GetCursorPos(double* pos)
{
#ifdef TARGET_OS_OSX
    NSPoint mouseLocation = [NSEvent mouseLocation];
    pos[0] = mouseLocation.x;
    pos[1] = mouseLocation.y;
#else
    // No equivalent for iOS
#endif
}

void ContextApple::_PollEvents()
{
}
}
}
}
