///
/// Project: VenomEngineWorkspace
/// @file ContextAppleDelegate.mm
/// @date Mar, 11 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/context/apple/ContextAppleDelegate.h>
#include <venom/common/context/apple/ContextAppleViewController.h>

@implementation ContextAppleDelegate

- (void)setLayer:(CAMetalLayer *)layer
{
    _layer = layer;
}

- (void)setDevice:(id<MTLDevice>)device__
{
    _device = device__;
}

#if defined(VENOM_PLATFORM_IOS)

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    return YES;
}

#else

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    NSRect frame = NSMakeRect(100, 100, 800, 600);
    
    // Create the window
    self.window = [[NSWindow alloc] initWithContentRect:frame
                                              styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable)
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];
    [self.window setTitle:@"Metal Window"];
    
    ContextAppleViewController * viewController = [[ContextAppleViewController alloc] init];
    // Give Metal Device and Layer to the View Controller
    [viewController setDevice:_device];
    [viewController setLayer: _layer];
    
    [self.window setContentViewController: viewController];
    [self.window makeKeyAndOrderFront:nil];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

#endif

@end
