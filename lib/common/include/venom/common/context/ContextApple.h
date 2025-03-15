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

#include <venom/common/context/apple/ContextAppleInfo.h>

struct ContextAppleData
{
    __strong id __window;
    __strong id __delegate;
    id __layer;
};

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

    vc::Error Run(int argc, const char * argv[]) override;
    vc::Error PreRun();
    inline vc::Error Loop() { return _runLoopFunction(); }
    static inline vc::Error RunLoop() { return GetAppleContext()->Loop(); }
    static void * GetMetalLayer();
    
    inline void __SetKeyboardState(vc::KeyboardInput key, vc::InputState state) { _SetKeyboardState(key, state); }
    inline void __SetMouseState(vc::MouseButton button, vc::InputState state) { _SetMouseState(button, state); }
    inline void __AddKeyReleasedEvent(vc::KeyboardInput key) { _AddKeyReleasedEvent(key); }
    inline void __AddMouseReleasedEvent(vc::MouseButton button) { _AddMouseReleasedEvent(button); }
    inline void __SetKeyboardModifierState(bool shift, bool control, bool alt, bool super) { _SetKeyboardModifierState(shift, control, alt, super); }
    inline void __SetCursorPosition(double x, double y) { __mouseX = x; __mouseY = y; };
#if defined(VENOM_PLATFORM_MOBILE)
    inline void __SetVirtualJoystickPress(bool enable) { _SetVirtualJoystickPress(enable); }
    inline void __SetVirtualCameraPress(bool enable) { _SetVirtualCameraPress(enable); }
    inline void __SetVirtualJoystickMovement(double x, double y) { _SetVirtualJoystickMovement(x, y); }
    inline void __SetVirtualCameraRotation(double x, double y) { _SetVirtualCameraRotation(x, y); }
    inline void __TriggerPreviousAction() { _TriggerPreviousAction(); }
    inline void __TriggerNextAction() { _TriggerNextAction(); }
#endif

#ifdef __OBJC__
    void __GiveMetalLayer(CAMetalLayer * layer);
    void __UpdateWindowSize(CGSize size);

    AppleWindow * GetAppleWindow();
    AppleView * GetAppleView();
#endif

protected:
    bool _ShouldClose() override;
    void _GetCursorPos(double * pos) override;
    void _PollEvents() override;

private:
    int __ConvertKeyboardInputToAppleEnum(vc::KeyboardInput key) const;
    vc::KeyboardInput __ConvertAppleEnumToKeyboardInput(int key) const;

private:
#ifdef __OBJC__
    ContextAppleData __contextAppleData;
    ContextAppleData * GetData();
#endif
    double __mouseX;
    double __mouseY;
};

}
}
}

venom::context::apple::ContextApple * VENOM_COMMON_API CreateContextApple();

#endif
