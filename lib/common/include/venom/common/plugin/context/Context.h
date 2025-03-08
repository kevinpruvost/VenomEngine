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

#include <GLFW/glfw3.h>
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

class VENOM_COMMON_API Context
{
public:
    Context();
    ~Context();
public:
    enum class ContextType
    {
        GLFW = 0,
#ifdef __APPLE__
        UIKit,
#endif
        Count
    };

    static inline Context * Get() { return s_context; }

    vc::Error InitContext();
    bool ShouldClose();
    void PollEvents();
    inline GLFWwindow * GetWindow() { return __window; }
    inline const GLFWwindow * GetWindow() const { return __window; }
    inline void SetWindowTitle(const char * title) { glfwSetWindowTitle(__window, title); }
    inline InputState GetKeyState(KeyboardInput key) const { return __keyboardState[key]; }
    inline bool IsKeyPressed(KeyboardInput key) const { return (__keyboardState[key] & (InputState::Pressed | InputState::Repeat)) > 0; }
    inline bool IsKeyReleased(KeyboardInput key) const { return __keyboardState[key] == InputState::Released; }
    inline bool IsKeyRepeat(KeyboardInput key) const { return __keyboardState[key] == InputState::Repeat; }
    static inline int GetWindowWidth() { return s_context->__width; }
    static inline int GetWindowHeight() { return s_context->__height; }
    
    /**
     * @brief Checks if a specified keyboard modifier key is currently pressed.
     * @param modifier A bitmask of one or more @ref KeyboardModifier values.
     * @return true if the specified modifier(s) is pressed; otherwise, false.
     */
    inline bool IsKeyModifierPressed(int modifier) const { return (__keyboardModifierState & modifier) != 0; }

    inline vcm::Vec2 GetMousePosition() const { return vcm::Vec2(static_cast<float>(__mousePos[0]), static_cast<float>(__mousePos[1])); }
    inline vcm::Vec2 GetMouseMove() const { return vcm::Vec2(static_cast<float>(__mousePos[0] - __mouseLastPos[0]), static_cast<float>(__mousePos[1] - __mouseLastPos[1])); }

private:
    int __ConvertKeyboardInputToGLFWEnum(KeyboardInput key) const;
    KeyboardInput __ConvertGLFWEnumToKeyboardInput(int key) const;
    inline int __GetWindowWidth() const { return __width; }
    inline int __GetWindowHeight() const { return __height; }

private:
    GLFWwindow * __window;
    vc::Vector<GLFWvidmode> __modes;
    vc::Array<InputState, KeyboardInput::KeyboardTotal> __keyboardState;
    vc::Array<InputState, MouseButton::MouseTotal> __mouseState;
    int8_t __keyboardModifierState;
    vc::Stack<KeyboardInput> __keyReleasedStack;
    vc::Stack<MouseButton> __mouseReleasedStack;

    double __mousePos[2];
    double __mouseLastPos[2];
    int __width, __height;
    bool __shouldClose;

private:
    static Context * s_context;
};
}
}
