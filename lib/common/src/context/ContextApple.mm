///
/// Project: VenomEngineWorkspace
/// @file Context_Apple.mm
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/context/ContextApple.h>

#include <venom/common/context/apple/ContextAppleDelegate.h>

#include <venom/common/Log.h>
#include <venom/common/Config.h>

#include <venom/common/plugin/graphics/GraphicsSettings.h>

namespace venom
{
namespace context
{
namespace apple
{
ContextApple::ContextApple()
{
    [NSApplication sharedApplication];
}

ContextApple* ContextApple::GetAppleContext()
{
    venom_assert(vc::Config::GetContextType() == vc::Context::ContextType::Apple, "Trying to get Apple Context while context is in a different mode.");
    return dynamic_cast<ContextApple *>(Get());
}

ContextAppleData * ContextApple::GetData()
{
    return &__contextAppleData;
}

ContextApple::~ContextApple()
{
#ifdef VENOM_PLATFORM_MACOS
    if (GetAppleWindow())
        [GetAppleWindow() close];
#else
    if (GetAppleWindow())
        [GetAppleWindow() setHidden:YES];
#endif
}

vc::Error ContextApple::Run(int argc, const char * argv[])
{
    venom_assert(_runLoopFunction, "Context::Run() : No run loop function set");
    int ret;
#ifdef VENOM_PLATFORM_MACOS
    ret = NSApplicationMain(argc, argv);
#else
    ret = UIApplicationMain(argc, argv, nil, NSStringFromClass([ContextAppleDelegate class]));
#endif
    return ret == 0 ? vc::Error::Success : vc::Error::Failure;
}

void ContextApple::_SetWindowTitle(const char* title)
{
#ifdef VENOM_PLATFORM_MACOS
    [__contextAppleData.__window setTitle:[NSString stringWithUTF8String:title]];
#else
    // No equivalent for iOS
    vc::Log::Error("Cannot change window title at runtime on iOS");
#endif
}

void ContextApple::SetMetalDevice(id<MTLDevice> device)
{
    [__contextAppleData.__delegate setDevice:device];
}

void ContextApple::SetMetalLayer(CAMetalLayer * layer)
{
    [__contextAppleData.__delegate setLayer: layer];
}

void* ContextApple::_GetWindow()
{
    return __contextAppleData.__window;
}

AppleWindow* ContextApple::GetAppleWindow()
{
    return ((ContextAppleDelegate *)__contextAppleData.__delegate).window;
}

AppleView* ContextApple::GetAppleView()
{
    return __contextAppleData.__view;
}

vc::Error ContextApple::_InitContext()
{
    // Might move initialization in Metal
#ifdef VENOM_PLATFORM_MACOS
    ContextAppleDelegate *delegate = [[ContextAppleDelegate alloc] init];
    __contextAppleData.__delegate = delegate;
    [NSApp setDelegate: __contextAppleData.__delegate];
#else
    
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
#ifdef VENOM_PLATFORM_MACOS
    [GetAppleWindow() toggleFullScreen:nil];
#else
    // No equivalent for iOS
#endif
    return vc::Error::Success;
}

bool ContextApple::_ShouldClose()
{
#ifdef VENOM_PLATFORM_MACOS
    return ![GetAppleWindow() isVisible];
#else
    return GetAppleWindow().isHidden;
#endif
}

void ContextApple::_GetCursorPos(double* pos)
{
}

void ContextApple::__UpdateWindowSize(CGSize size)
{
    _width = size.width;
    _height = size.height;
    vc::GraphicsSettings::SetWindowResolution(_width, _height);
}

void ContextApple::_PollEvents()
{
}
}
}
}

venom::context::apple::ContextApple * CreateContextApple()
{
    return new venom::context::apple::ContextApple();
}
