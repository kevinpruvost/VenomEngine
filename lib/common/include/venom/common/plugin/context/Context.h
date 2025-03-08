///
/// Project: VenomEngine
/// @file Context.h
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Error.h>
#include <venom/common/Containers.h>
#include <venom/common/math/Matrix.h>
#include <venom/common/plugin/PluginObject.h>

#include <vector>

namespace venom
{
namespace common
{

enum InputState : int8_t
{
    Released = 0x1,
    Pressed  = 0x2,
    Repeat   = 0x4,
    Unknown  = 0x8
};

enum KeyboardInput {
    KeyboardUnknown = 0, KeyboardA = 1, KeyboardB = 2, KeyboardC = 3,
    KeyboardD = 4, KeyboardE = 5, KeyboardF = 6, KeyboardG = 7,
    KeyboardH = 8, KeyboardI = 9, KeyboardJ = 10, KeyboardK = 11,
    KeyboardL = 12, KeyboardM = 13, KeyboardN = 14, KeyboardO = 15,
    KeyboardP = 16, KeyboardQ = 17, KeyboardR = 18, KeyboardS = 19,
    KeyboardT = 20, KeyboardU = 21, KeyboardV = 22, KeyboardW = 23,
    KeyboardX = 24, KeyboardY = 25, KeyboardZ = 26,

    Keyboard0 = 27, Keyboard1 = 28, Keyboard2 = 29, Keyboard3 = 30,
    Keyboard4 = 31, Keyboard5 = 32, Keyboard6 = 33, Keyboard7 = 34,
    Keyboard8 = 35, Keyboard9 = 36,

    KeyboardEscape = 37, KeyboardEnter = 38, KeyboardTab = 39, KeyboardBackspace = 40,
    KeyboardInsert = 41, KeyboardDelete = 42, KeyboardRight = 43, KeyboardLeft = 44,
    KeyboardDown = 45, KeyboardUp = 46, KeyboardPageUp = 47, KeyboardPageDown = 48,
    KeyboardHome = 49, KeyboardEnd = 50, KeyboardCapsLock = 51, KeyboardScrollLock = 52,
    KeyboardNumLock = 53, KeyboardPrintScreen = 54, KeyboardPause = 55, KeyboardF1 = 56,

    KeyboardF2 = 57, KeyboardF3 = 58, KeyboardF4 = 59, KeyboardF5 = 60,
    KeyboardF6 = 61, KeyboardF7 = 62, KeyboardF8 = 63, KeyboardF9 = 64,
    KeyboardF10 = 65, KeyboardF11 = 66, KeyboardF12 = 67, KeyboardF13 = 68,
    KeyboardF14 = 69, KeyboardF15 = 70, KeyboardF16 = 71, KeyboardF17 = 72,
    KeyboardF18 = 73, KeyboardF19 = 74, KeyboardF20 = 75, KeyboardF21 = 76,
    KeyboardF22 = 77, KeyboardF23 = 78, KeyboardF24 = 79, KeyboardF25 = 80,

    KeyboardKP0 = 81, KeyboardKP1 = 82, KeyboardKP2 = 83, KeyboardKP3 = 84,
    KeyboardKP4 = 85, KeyboardKP5 = 86, KeyboardKP6 = 87, KeyboardKP7 = 88,
    KeyboardKP8 = 89, KeyboardKP9 = 90, KeyboardKPDecimal = 91, KeyboardKPDivide = 92,
    KeyboardKPMultiply = 93, KeyboardKPSubtract = 94, KeyboardKPAdd = 95, KeyboardKPEnter = 96,
    KeyboardKPEqual = 97, KeyboardLeftShift = 98, KeyboardLeftControl = 99, KeyboardLeftAlt = 100,
    KeyboardLeftSuper = 101, KeyboardRightShift = 102, KeyboardRightControl = 103, KeyboardRightAlt = 104,
    KeyboardRightSuper = 105, KeyboardMenu = 106, KeyboardSpace = 107, KeyboardApostrophe = 108,

    KeyboardComma = 109, KeyboardMinus = 110, KeyboardPeriod = 111, KeyboardSlash = 112,
    KeyboardSemicolon = 113, KeyboardEqual = 114, KeyboardLeftBracket = 115, KeyboardBackslash = 116,
    KeyboardRightBracket = 117, KeyboardGraveAccent = 118, KeyboardWorld1 = 119, KeyboardWorld2 = 120,

    KeyboardTotal
};

typedef int8_t KeyboardModifierState;
enum KeyboardModifier {
    KeyboardModShift = 0x0001,
    KeyboardModControl = 0x0002,
    KeyboardModAlt = 0x0004,
    KeyboardModSuper = 0x0008
};

enum MouseButton {
    MouseButton1 = 0, MouseButton2 = 1, MouseButton3 = 2,
    MouseButton4 = 3, MouseButton5 = 4, MouseButton6 = 5,
    MouseButton7 = 6, MouseButton8 = 7, MouseButtonLast = MouseButton8,
    MouseButtonLeft = MouseButton1, MouseButtonRight = MouseButton2,
    MouseButtonMiddle = MouseButton3,

    MouseTotal
};

class Screen;
class VENOM_COMMON_API ScreenVideoMode
{
public:
    ScreenVideoMode(int width, int height);
    ~ScreenVideoMode();

    inline int GetWidth() const { return __width; }
    inline int GetHeight() const { return __height; }
    const vc::Vector<int> & GetRefreshRates() const { return __refreshRates; }
    inline int GetRefreshRate(int index) const { return __refreshRates[index]; }
    inline int GetRefreshRateCount() const { return __refreshRates.size(); }
    void SortRefreshRates();
    bool operator<(const ScreenVideoMode & mode) const;
    bool operator>(const ScreenVideoMode & mode) const;
    bool operator==(const ScreenVideoMode&) const;
private:
    int __width;
    int __height;
    void __AddRefreshRate(int rate);
    vc::Vector<int> __refreshRates;

    friend class Screen;
};


class VENOM_COMMON_API Screen
{
public:
    Screen();
    ~Screen();

    void AddVideoMode(int width, int height, int refreshRate);
    inline int GetVideoModeCount() const { return __modes.size(); }
    const ScreenVideoMode & GetVideoMode(int index) const { return __modes[index]; }
    const vc::Vector<ScreenVideoMode> & GetVideoModes() const { return __modes; }
    void SortVideoModes();
private:
    vc::Vector<ScreenVideoMode> __modes;
};

class VENOM_COMMON_API Context : public PluginObjectImpl, public PluginObject
{
public:
    Context();
    ~Context();
public:
    enum class ContextType
    {
        GLFW = 0,
#ifdef __APPLE__
        Apple, // UIKit / Cocoa
#endif
        Count
    };

    static inline Context * Get() { return s_context; }
    vc::Error Init();

    inline bool ShouldClose() const { return __shouldClose; }
    void PollEvents();
    inline InputState GetKeyState(KeyboardInput key) const { return __keyboardState[key]; }
    inline bool IsKeyPressed(KeyboardInput key) const { return (__keyboardState[key] & (InputState::Pressed | InputState::Repeat)) > 0; }
    inline bool IsKeyReleased(KeyboardInput key) const { return __keyboardState[key] == InputState::Released; }
    inline bool IsKeyRepeat(KeyboardInput key) const { return __keyboardState[key] == InputState::Repeat; }
    static inline int GetWindowWidth() { return s_context->_width; }
    static inline int GetWindowHeight() { return s_context->_height; }

    /**
     * @brief Checks if a specified keyboard modifier key is currently pressed.
     * @param modifier A bitmask of one or more @ref KeyboardModifier values.
     * @return true if the specified modifier(s) is pressed; otherwise, false.
     */
    inline bool IsKeyModifierPressed(int modifier) const { return (__keyboardModifierState & modifier) != 0; }
    inline vcm::Vec2 GetMousePosition() const { return vcm::Vec2(static_cast<float>(__mousePos[0]), static_cast<float>(__mousePos[1])); }
    inline vcm::Vec2 GetMouseMove() const { return vcm::Vec2(static_cast<float>(__mousePos[0] - __mouseLastPos[0]), static_cast<float>(__mousePos[1] - __mouseLastPos[1])); }
    const vc::Vector<Screen> & GetScreens() const { return _screens; }
    const Screen & GetCurrentScreen() const { return _screens[_currentScreenIndex]; }
    const ScreenVideoMode & GetCurrentVideoMode() const { return _screens[_currentScreenIndex].GetVideoMode(_currentVideoModeIndex); }
    int GetCurrentRefreshRate() const { return _currentRefreshRate; }
    vc::Error ChangeRefreshRate(int rate);
    vc::Error ChangeVideoMode(int index);
    vc::Error ChangeScreen(int index);

    //
    // Context Abstraction
    //
public:
    inline void SetWindowTitle(const char * title) { _SetWindowTitle(title); }
    inline void * GetWindow() { return _GetWindow(); }
    inline vc::Error SetFullscreen(bool fullscreen) { _fullscreen = fullscreen; return _SetFullscreen(); }
protected:
    virtual void _SetWindowTitle(const char * title) = 0;
    virtual void * _GetWindow() = 0;
    virtual vc::Error _InitContext() = 0;
    virtual vc::Error _UpdateVideoMode() = 0;
    virtual vc::Error _UpdateRefreshRate() = 0;
    virtual vc::Error _UpdateScreen() = 0;
    virtual vc::Error _SetFullscreen() = 0;

    virtual bool _ShouldClose() = 0;
    virtual void _GetCursorPos(double * pos) = 0;
    virtual void _PollEvents() = 0;
protected:
    inline void _SetKeyboardState(KeyboardInput key, InputState state) { __keyboardState[key] = state; }
    inline void _SetMouseState(MouseButton button, InputState state) { __mouseState[button] = state; }
    inline void _AddKeyReleasedEvent(KeyboardInput key) { __keyReleasedStack.push(key); }
    inline void _AddMouseReleasedEvent(MouseButton button) { __mouseReleasedStack.push(button); }
    inline void _SetKeyboardModifierState(bool shift, bool control, bool alt, bool super) {
        __keyboardModifierState = 0x0000;
        if (shift) __keyboardModifierState |= KeyboardModifier::KeyboardModShift;
        if (control) __keyboardModifierState |= KeyboardModifier::KeyboardModControl;
        if (alt) __keyboardModifierState |= KeyboardModifier::KeyboardModAlt;
        if (super) __keyboardModifierState |= KeyboardModifier::KeyboardModSuper;
    }
protected:
    int _currentScreenIndex;
    int _currentVideoModeIndex;
    int _currentRefreshRate;
    int _width, _height;
    bool _fullscreen;
    vc::Vector<Screen> _screens;

private:
    int __ConvertKeyboardInputToGLFWEnum(KeyboardInput key) const;
    KeyboardInput __ConvertGLFWEnumToKeyboardInput(int key) const;

private:
    vc::Array<InputState, KeyboardInput::KeyboardTotal> __keyboardState;
    vc::Array<InputState, MouseButton::MouseTotal> __mouseState;
    KeyboardModifierState __keyboardModifierState;
    vc::Stack<KeyboardInput> __keyReleasedStack;
    vc::Stack<MouseButton> __mouseReleasedStack;

    double __mousePos[2];
    double __mouseLastPos[2];
    bool __shouldClose;

private:
    static Context * s_context;
};
}
}
