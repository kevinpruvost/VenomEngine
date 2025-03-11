///
/// Project: VenomEngineWorkspace
/// @file ContextAppleViewController.mm
/// @date Mar, 11 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/context/apple/ContextAppleViewController.h>
#include <venom/common/context/ContextApple.h>

@implementation ContextAppleViewController
{
    MTKView *_view;
}

- (void)loadView
{
    // Explicitly create an MTKView
    _view = [[MTKView alloc] initWithFrame:NSMakeRect(0, 0, 800, 600)];

    // Set the view controller's view to be the MTKView
    self.view = _view;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    _view = (MTKView *)self.view;
    _view.layer = _layer;
    _view.device = _device;
    _view.colorPixelFormat = _layer.pixelFormat;
    _view.delegate = self;
    CGSize size = _view.drawableSize;
    venom::context::apple::ContextApple::GetAppleContext()->__UpdateWindowSize(size);
}


- (void)drawInMTKView:(nonnull MTKView *)view {
    if (venom::context::apple::ContextApple::RunLoop() != vc::Error::Success) {
        [[NSApplication sharedApplication] terminate:nil];
    }
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size { 
    venom::context::apple::ContextApple::GetAppleContext()->__UpdateWindowSize(size);
}

@end
