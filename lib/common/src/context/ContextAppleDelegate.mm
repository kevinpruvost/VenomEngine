///
/// Project: VenomEngineWorkspace
/// @file ContextAppleDelegate.mm
/// @date Mar, 11 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/context/apple/ContextAppleDelegate.h>
#include <venom/common/context/apple/ContextAppleViewController.h>

#include <venom/common/Log.h>

@implementation ContextAppleDelegate

#if defined(VENOM_PLATFORM_IOS)

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Initialize your iOS-specific setup
    ContextAppleViewController *viewController = [[ContextAppleViewController alloc] init];
    
    // Set the root view controller for your app
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.rootViewController = viewController;
    [self.window makeKeyWindow];
    self.window.hidden = NO;
    // Set view to delegate
    self.view = self.window.rootViewController.view;
    return YES;
}

#elif defined(VENOM_PLATFORM_MACOS)

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    NSRect frame = NSMakeRect(100, 100, 800, 600);
    
    // Create the window
    self.window = [[NSWindow alloc] initWithContentRect:frame
                                              styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable)
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];
    [self.window setTitle:@"VenomEngine"];
    
    ContextAppleViewController * viewController = [[ContextAppleViewController alloc] init];
    //self.windowController = [[ContextAppleWindowController alloc] initWithWindow:self.window];
    self.windowController = nil;
    
    [self.window setContentViewController: viewController];
    [self.window makeKeyAndOrderFront:nil];
    // Set view to delegate
    self.view = self.window.contentView;
    // Set the preferredFramesPerSecond to the maximum supported by the display
    ((MTKView *)self.view).preferredFramesPerSecond = self.window.screen.maximumFramesPerSecond;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

#endif

@end
