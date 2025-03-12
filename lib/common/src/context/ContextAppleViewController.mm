///
/// Project: VenomEngineWorkspace
/// @file ContextAppleViewController.mm
/// @date Mar, 11 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/context/apple/ContextAppleViewController.h>
#include <venom/common/context/ContextApple.h>

#include <venom/common/Log.h>

@implementation ContextAppleViewController
{
    MTKView *_view;
}

- (void)loadView
{
    // Explicitly create an MTKView
#ifdef VENOM_PLATFORM_MACOS
    _view = [[MTKView alloc] initWithFrame:NSMakeRect(0, 0, 800, 600)];
    self.view = _view;
#else
    _view = [[MTKView alloc] initWithFrame:UIScreen.mainScreen.bounds];
#endif

    // Set the view controller's view to be the MTKView
    self.view = _view;
    DEBUG_PRINT("loadView done...");
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    DEBUG_PRINT("viewDidLoad done...");
    _view = (MTKView *)self.view;
    CAMetalLayer * layer = getGlobalMetalLayer();
    if (layer != nil) {
        CAMetalLayer * newLayer = (CAMetalLayer*)_view.layer;
        newLayer.pixelFormat = layer.pixelFormat; // Match pixel format
        newLayer.contentsScale = newLayer.contentsScale; // Contents scale
        newLayer.drawableSize = layer.drawableSize;  // Match buffer size
        
        // Check if the property exists before setting it (iOS 16+)
        if ([newLayer respondsToSelector:@selector(setWantsExtendedDynamicRangeContent:)]) {
            newLayer.wantsExtendedDynamicRangeContent = layer.wantsExtendedDynamicRangeContent; // If using HDR
        }
        newLayer.maximumDrawableCount = layer.maximumDrawableCount; // multisampling

        [layer release];

        setGlobalMetalLayer(newLayer);
        layer = getGlobalMetalLayer();
    }
    _view.device = getGlobalMetalDevice();
    _view.colorPixelFormat = layer.pixelFormat;
    _view.delegate = self;
    CGSize size = _view.drawableSize;
    venom::context::apple::ContextApple::GetAppleContext()->__UpdateWindowSize(size);
}


- (void)drawInMTKView:(nonnull MTKView *)view {
#ifdef VENOM_PLATFORM_MACOS
    if (venom::context::apple::ContextApple::RunLoop() != vc::Error::Success) {
        [[NSApplication sharedApplication] terminate:nil];
    }
#else
    venom::context::apple::ContextApple::RunLoop();
#endif
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size { 
    venom::context::apple::ContextApple::GetAppleContext()->__UpdateWindowSize(size);
}

@end
