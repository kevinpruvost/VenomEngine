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
#if defined(VENOM_PLATFORM_IOS)
    CGPoint leftTouchStart;  // Left touch (movement)
    CGPoint rightTouchStart; // Right touch (rotation)
    UITouch *leftTouch;
    UITouch *rightTouch;
#endif
}

- (void)loadView
{
    // Explicitly create an MTKView
#if defined(VENOM_PLATFORM_MACOS)
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
        newLayer.maximumDrawableCount = layer.maximumDrawableCount; // frames in flight

        [layer release];

        setGlobalMetalLayer(newLayer);
        layer = getGlobalMetalLayer();
    }
    _view.autoResizeDrawable = YES;
    _view.device = getGlobalMetalDevice();
    _view.colorPixelFormat = layer.pixelFormat;
    _view.delegate = self;

#if defined(VENOM_PLATFORM_IOS)
    _view.multipleTouchEnabled = YES;
    leftTouch = nil;
    rightTouch = nil;
    
    // Swipe from LEFT edge (Previous)
    UIScreenEdgePanGestureRecognizer *leftSwipe = [[UIScreenEdgePanGestureRecognizer alloc] initWithTarget:self action:@selector(handleEdgeSwipe:)];
    leftSwipe.edges = UIRectEdgeLeft;
    [_view addGestureRecognizer:leftSwipe];

    // Swipe from RIGHT edge (Next)
    UIScreenEdgePanGestureRecognizer *rightSwipe = [[UIScreenEdgePanGestureRecognizer alloc] initWithTarget:self action:@selector(handleEdgeSwipe:)];
    rightSwipe.edges = UIRectEdgeRight;
    [_view addGestureRecognizer:rightSwipe];
#endif

    CGSize size = _view.bounds.size;
    venom::context::apple::ContextApple::GetAppleContext()->__UpdateWindowSize(size);
    venom::context::apple::ContextApple::GetAppleContext()->PreRun();
    
    // Managing focus/unfocus
#if defined(VENOM_PLATFORM_MACOS)
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(windowDidBecomeKeyNotification:) name:NSWindowDidBecomeKeyNotification object:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(windowDidResignKeyNotification:) name:NSWindowDidResignKeyNotification object:nil];
#endif
}

- (void)drawInMTKView:(nonnull MTKView *)view {
#if defined(VENOM_PLATFORM_MACOS)
    if (venom::context::apple::ContextApple::RunLoop() != vc::Error::Success) {
        [[NSApplication sharedApplication] stop:nil];
    }
#else
    venom::context::apple::ContextApple::RunLoop();
#endif
    venom::context::apple::ContextApple::GetAppleContext()->PollEvents();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    // Adapt the preferred frame rate to the screen's maximum frame rate
    static AppleScreen *screen = nil;
    if (view.window.screen != screen) {
        screen = view.window.screen;
        _view.preferredFramesPerSecond = screen.maximumFramesPerSecond;
    }
    
    venom::context::apple::ContextApple::GetAppleContext()->__UpdateWindowSize(view.bounds.size);
}

#if defined(VENOM_PLATFORM_MACOS)

- (void)viewDidAppear {
    [super viewDidAppear];
    
    [self.view.window makeFirstResponder:self.view];
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

// Mouse moved
- (void)mouseMoved:(NSEvent *)event
{
    NSPoint location = event.locationInWindow;
    CGFloat windowHeight = self.view.frame.size.height; // Get the window height
    //NSLog(@"Mouse moved: %f, %f", location.x, location.y);

    // Y axis reversed
    venom::context::apple::ContextApple::GetAppleContext()->__SetCursorPosition(location.x, windowHeight - location.y);
}

- (void)mouseDown:(NSEvent *)event
{
    vc::MouseButton button = static_cast<vc::MouseButton>(event.buttonNumber);
    const vc::InputState state = vc::InputState::Pressed;
    //NSLog(@"Mouse down: %d", button);

    venom::context::apple::ContextApple::GetAppleContext()->__SetMouseState(button, state);
}

- (void)mouseUp:(NSEvent *)event
{
    vc::MouseButton button = static_cast<vc::MouseButton>(event.buttonNumber);
    const vc::InputState state = vc::InputState::Released;
    //NSLog(@"Mouse up: %d", button);

    venom::context::apple::ContextApple::GetAppleContext()->__SetMouseState(button, state);
    venom::context::apple::ContextApple::GetAppleContext()->__AddMouseReleasedEvent(button);
}

- (void)keyDown:(NSEvent *)event
{
    //NSLog(@"Key pressed: %d", event.keyCode);
    if ((event.modifierFlags & NSEventModifierFlagCommand) && event.keyCode == 12) { // 12 = Q key
        [NSApp terminate:self]; // Quit app
        return;
    }
    
    vc::KeyboardInput key = convertAppleKeyToVCKey(event.keyCode); // Convert macOS keycode
    vc::InputState state = vc::InputState::Pressed;

    venom::context::apple::ContextApple::GetAppleContext()->__SetKeyboardState(key, state);
    [self updateModifiers:event];
}

- (void)keyUp:(NSEvent *)event
{
    //NSLog(@"Key released: %d", event.keyCode);
    vc::KeyboardInput key = convertAppleKeyToVCKey(event.keyCode);
    vc::InputState state = vc::InputState::Released;

    venom::context::apple::ContextApple::GetAppleContext()->__SetKeyboardState(key, state);
    venom::context::apple::ContextApple::GetAppleContext()->__AddKeyReleasedEvent(key);
    [self updateModifiers:event];
}

- (void)updateModifiers:(NSEvent *)event
{
    bool shift = (event.modifierFlags & NSEventModifierFlagShift) != 0;
    bool control = (event.modifierFlags & NSEventModifierFlagControl) != 0;
    bool alt = (event.modifierFlags & NSEventModifierFlagOption) != 0;
    bool sup = (event.modifierFlags & NSEventModifierFlagCommand) != 0;

    venom::context::apple::ContextApple::GetAppleContext()->__SetKeyboardModifierState(shift, control, alt, sup);
}

- (void)windowDidBecomeKeyNotification: (NSNotification *) notification {
    self.view.window.acceptsMouseMovedEvents = YES;
}

- (void)windowDidResignKeyNotification: (NSNotification *) notification {
    self.view.window.acceptsMouseMovedEvents = NO;
}

#else

// Handle touch start (Press)
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        CGPoint location = [touch locationInView:self.view];
        CGFloat halfWidth = self.view.frame.size.width / 2;
        
        if (location.x < halfWidth && !self->leftTouch) {  // Left side → Movement
            self->leftTouch = touch;
            self->leftTouchStart = location;
            // Trigger press event for movement
            venom::context::apple::ContextApple::GetAppleContext()->__SetVirtualJoystickPress(true);
        }
        else if (location.x >= halfWidth && !self->rightTouch) {  // Right side → Rotation
            self->rightTouch = touch;
            self->rightTouchStart = location;
            // Trigger press event for rotation
            venom::context::apple::ContextApple::GetAppleContext()->__SetVirtualCameraPress(true);
        }
    }
}

// Handle touch move (analog stick & look movement)
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        CGPoint location = [touch locationInView:self.view];
        
        if (touch == self->leftTouch) {  // Left joystick (Movement)
            CGPoint delta = CGPointMake(location.x - self->leftTouchStart.x, location.y - self->leftTouchStart.y);
            venom::context::apple::ContextApple::GetAppleContext()->__SetVirtualJoystickMovement(delta.x, -delta.y);
        }
        else if (touch == self->rightTouch) {  // Right joystick (Camera rotation)
            CGPoint delta = CGPointMake(location.x - self->rightTouchStart.x, location.y - self->rightTouchStart.y);
            venom::context::apple::ContextApple::GetAppleContext()->__SetVirtualCameraRotation(delta.x, delta.y);
        }
    }
}

// Handle touch end (Release)
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        if (touch == self->leftTouch) {
            self->leftTouch = nil;
            venom::context::apple::ContextApple::GetAppleContext()->__SetVirtualJoystickMovement(0, 0); // Stop movement
            // Trigger release event for movement
            venom::context::apple::ContextApple::GetAppleContext()->__SetVirtualJoystickPress(false);
        }
        if (touch == self->rightTouch) {
            self->rightTouch = nil;
            venom::context::apple::ContextApple::GetAppleContext()->__SetVirtualCameraRotation(0, 0); // Stop rotation
            // Trigger release event for rotation
            venom::context::apple::ContextApple::GetAppleContext()->__SetVirtualCameraPress(false);
        }
    }
}

// Handle the swipe gesture
- (void)handleEdgeSwipe:(UIScreenEdgePanGestureRecognizer *)gesture {
    if (gesture.state == UIGestureRecognizerStateRecognized) {
        if (gesture.edges == UIRectEdgeLeft) {
            NSLog(@"Swiped from left edge");
            venom::context::apple::ContextApple::GetAppleContext()->__TriggerPreviousAction();
        }
        else if (gesture.edges == UIRectEdgeRight) {
            NSLog(@"Swiped from right edge");
            venom::context::apple::ContextApple::GetAppleContext()->__TriggerNextAction();
        }
    }
}

// Handle canceled touches (same as ended)
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [self touchesEnded:touches withEvent:event];
}


#endif

@end
