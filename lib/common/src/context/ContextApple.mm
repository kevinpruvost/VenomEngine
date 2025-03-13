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
#include <venom/common/context/apple/ContextAppleInfo.h>

namespace venom
{
namespace context
{
namespace apple
{
ContextApple::ContextApple()
{
#ifdef VENOM_PLATFORM_MACOS
    [NSApplication sharedApplication];
#else
    [UIApplication sharedApplication];
#endif
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

void* ContextApple::GetMetalLayer()
{
    return (void *)getGlobalMetalLayer();
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

vc::Error ContextApple::PostRun()
{
    return _postRunLoopFunction();
}

vc::Error ContextApple::Run(int argc, const char * argv[])
{
    venom_assert(_postRunLoopFunction, "Context::Run() : No post run loop function set");
    venom_assert(_runLoopFunction, "Context::Run() : No run loop function set");
    int ret;
#ifdef VENOM_PLATFORM_MACOS
    ret = NSApplicationMain(argc, argv);
#else
    // Cast argv as UIApplicationMain prototype is different
    DEBUG_PRINT("About to start UIApplicationMain...");
    ret = UIApplicationMain(argc, (char **)argv, nil, NSStringFromClass([ContextAppleDelegate class]));
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
    id <MTLDevice> device = MTLCreateSystemDefaultDevice();
    setGlobaMetallDevice(device);
    if (device == nil) {
        vc::Log::Error("Could not initialize Metal Device.\n");
        return vc::Error::Failure;
    }
    CAMetalLayer * MetalLayer = [CAMetalLayer layer];
    MetalLayer.device = getGlobalMetalDevice();
    setGlobalMetalLayer(MetalLayer);
    if (MetalLayer == nil) {
        vc::Log::Error("Could not initialize Metal Layer.\n");
        return vc::Error::Failure;
    }
    // Might move initialization in Metal
    ContextAppleDelegate *delegate = [[ContextAppleDelegate alloc] init];
    __contextAppleData.__delegate = delegate;
#if defined(VENOM_PLATFORM_MACOS)
    [NSApp setDelegate: delegate];
#elif defined(VENOM_PLATFORM_IOS)
    [[UIApplication sharedApplication] setDelegate: delegate];
#endif
    DEBUG_PRINT("_InitContext done...");
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

void ContextApple::__GiveMetalLayer(CAMetalLayer * layer)
{
    __contextAppleData.__layer = layer;
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
