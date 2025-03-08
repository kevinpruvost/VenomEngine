///
/// Project: VenomEngineWorkspace
/// @file ContextInput.cc
/// @date Oct, 31 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <Context_GLFW.h>

namespace venom
{
namespace context
{
namespace glfw
{
int ContextGLFW::__ConvertKeyboardInputToGLFWEnum(vc::KeyboardInput key) const
{
    switch (key)
    {
        case vc::KeyboardInput::KeyboardA: return GLFW_KEY_A;
        case vc::KeyboardInput::KeyboardB: return GLFW_KEY_B;
        case vc::KeyboardInput::KeyboardC: return GLFW_KEY_C;
        case vc::KeyboardInput::KeyboardD: return GLFW_KEY_D;
        case vc::KeyboardInput::KeyboardE: return GLFW_KEY_E;
        case vc::KeyboardInput::KeyboardF: return GLFW_KEY_F;
        case vc::KeyboardInput::KeyboardG: return GLFW_KEY_G;
        case vc::KeyboardInput::KeyboardH: return GLFW_KEY_H;
        case vc::KeyboardInput::KeyboardI: return GLFW_KEY_I;
        case vc::KeyboardInput::KeyboardJ: return GLFW_KEY_J;
        case vc::KeyboardInput::KeyboardK: return GLFW_KEY_K;
        case vc::KeyboardInput::KeyboardL: return GLFW_KEY_L;
        case vc::KeyboardInput::KeyboardM: return GLFW_KEY_M;
        case vc::KeyboardInput::KeyboardN: return GLFW_KEY_N;
        case vc::KeyboardInput::KeyboardO: return GLFW_KEY_O;
        case vc::KeyboardInput::KeyboardP: return GLFW_KEY_P;
        case vc::KeyboardInput::KeyboardQ: return GLFW_KEY_Q;
        case vc::KeyboardInput::KeyboardR: return GLFW_KEY_R;
        case vc::KeyboardInput::KeyboardS: return GLFW_KEY_S;
        case vc::KeyboardInput::KeyboardT: return GLFW_KEY_T;
        case vc::KeyboardInput::KeyboardU: return GLFW_KEY_U;
        case vc::KeyboardInput::KeyboardV: return GLFW_KEY_V;
        case vc::KeyboardInput::KeyboardW: return GLFW_KEY_W;
        case vc::KeyboardInput::KeyboardX: return GLFW_KEY_X;
        case vc::KeyboardInput::KeyboardY: return GLFW_KEY_Y;
        case vc::KeyboardInput::KeyboardZ: return GLFW_KEY_Z;

        case vc::KeyboardInput::Keyboard0: return GLFW_KEY_0;
        case vc::KeyboardInput::Keyboard1: return GLFW_KEY_1;
        case vc::KeyboardInput::Keyboard2: return GLFW_KEY_2;
        case vc::KeyboardInput::Keyboard3: return GLFW_KEY_3;
        case vc::KeyboardInput::Keyboard4: return GLFW_KEY_4;
        case vc::KeyboardInput::Keyboard5: return GLFW_KEY_5;
        case vc::KeyboardInput::Keyboard6: return GLFW_KEY_6;
        case vc::KeyboardInput::Keyboard7: return GLFW_KEY_7;
        case vc::KeyboardInput::Keyboard8: return GLFW_KEY_8;
        case vc::KeyboardInput::Keyboard9: return GLFW_KEY_9;

        case vc::KeyboardInput::KeyboardEscape: return GLFW_KEY_ESCAPE;
        case vc::KeyboardInput::KeyboardEnter: return GLFW_KEY_ENTER;
        case vc::KeyboardInput::KeyboardTab: return GLFW_KEY_TAB;
        case vc::KeyboardInput::KeyboardBackspace: return GLFW_KEY_BACKSPACE;
        case vc::KeyboardInput::KeyboardInsert: return GLFW_KEY_INSERT;
        case vc::KeyboardInput::KeyboardDelete: return GLFW_KEY_DELETE;
        case vc::KeyboardInput::KeyboardRight: return GLFW_KEY_RIGHT;
        case vc::KeyboardInput::KeyboardLeft: return GLFW_KEY_LEFT;
        case vc::KeyboardInput::KeyboardDown: return GLFW_KEY_DOWN;
        case vc::KeyboardInput::KeyboardUp: return GLFW_KEY_UP;
        case vc::KeyboardInput::KeyboardPageUp: return GLFW_KEY_PAGE_UP;
        case vc::KeyboardInput::KeyboardPageDown: return GLFW_KEY_PAGE_DOWN;
        case vc::KeyboardInput::KeyboardHome: return GLFW_KEY_HOME;
        case vc::KeyboardInput::KeyboardEnd: return GLFW_KEY_END;

        case vc::KeyboardInput::KeyboardCapsLock: return GLFW_KEY_CAPS_LOCK;
        case vc::KeyboardInput::KeyboardScrollLock: return GLFW_KEY_SCROLL_LOCK;
        case vc::KeyboardInput::KeyboardNumLock: return GLFW_KEY_NUM_LOCK;
        case vc::KeyboardInput::KeyboardPrintScreen: return GLFW_KEY_PRINT_SCREEN;
        case vc::KeyboardInput::KeyboardPause: return GLFW_KEY_PAUSE;

        case vc::KeyboardInput::KeyboardF1: return GLFW_KEY_F1;
        case vc::KeyboardInput::KeyboardF2: return GLFW_KEY_F2;
        case vc::KeyboardInput::KeyboardF3: return GLFW_KEY_F3;
        case vc::KeyboardInput::KeyboardF4: return GLFW_KEY_F4;
        case vc::KeyboardInput::KeyboardF5: return GLFW_KEY_F5;
        case vc::KeyboardInput::KeyboardF6: return GLFW_KEY_F6;
        case vc::KeyboardInput::KeyboardF7: return GLFW_KEY_F7;
        case vc::KeyboardInput::KeyboardF8: return GLFW_KEY_F8;
        case vc::KeyboardInput::KeyboardF9: return GLFW_KEY_F9;
        case vc::KeyboardInput::KeyboardF10: return GLFW_KEY_F10;
        case vc::KeyboardInput::KeyboardF11: return GLFW_KEY_F11;
        case vc::KeyboardInput::KeyboardF12: return GLFW_KEY_F12;
        case vc::KeyboardInput::KeyboardF13: return GLFW_KEY_F13;
        case vc::KeyboardInput::KeyboardF14: return GLFW_KEY_F14;
        case vc::KeyboardInput::KeyboardF15: return GLFW_KEY_F15;
        case vc::KeyboardInput::KeyboardF16: return GLFW_KEY_F16;
        case vc::KeyboardInput::KeyboardF17: return GLFW_KEY_F17;
        case vc::KeyboardInput::KeyboardF18: return GLFW_KEY_F18;
        case vc::KeyboardInput::KeyboardF19: return GLFW_KEY_F19;
        case vc::KeyboardInput::KeyboardF20: return GLFW_KEY_F20;
        case vc::KeyboardInput::KeyboardF21: return GLFW_KEY_F21;
        case vc::KeyboardInput::KeyboardF22: return GLFW_KEY_F22;
        case vc::KeyboardInput::KeyboardF23: return GLFW_KEY_F23;
        case vc::KeyboardInput::KeyboardF24: return GLFW_KEY_F24;
        case vc::KeyboardInput::KeyboardF25: return GLFW_KEY_F25;

        case vc::KeyboardInput::KeyboardKP0: return GLFW_KEY_KP_0;
        case vc::KeyboardInput::KeyboardKP1: return GLFW_KEY_KP_1;
        case vc::KeyboardInput::KeyboardKP2: return GLFW_KEY_KP_2;
        case vc::KeyboardInput::KeyboardKP3: return GLFW_KEY_KP_3;
        case vc::KeyboardInput::KeyboardKP4: return GLFW_KEY_KP_4;
        case vc::KeyboardInput::KeyboardKP5: return GLFW_KEY_KP_5;
        case vc::KeyboardInput::KeyboardKP6: return GLFW_KEY_KP_6;
        case vc::KeyboardInput::KeyboardKP7: return GLFW_KEY_KP_7;
        case vc::KeyboardInput::KeyboardKP8: return GLFW_KEY_KP_8;
        case vc::KeyboardInput::KeyboardKP9: return GLFW_KEY_KP_9;
        case vc::KeyboardInput::KeyboardKPDecimal: return GLFW_KEY_KP_DECIMAL;
        case vc::KeyboardInput::KeyboardKPDivide: return GLFW_KEY_KP_DIVIDE;
        case vc::KeyboardInput::KeyboardKPMultiply: return GLFW_KEY_KP_MULTIPLY;
        case vc::KeyboardInput::KeyboardKPSubtract: return GLFW_KEY_KP_SUBTRACT;
        case vc::KeyboardInput::KeyboardKPAdd: return GLFW_KEY_KP_ADD;
        case vc::KeyboardInput::KeyboardKPEnter: return GLFW_KEY_KP_ENTER;
        case vc::KeyboardInput::KeyboardKPEqual: return GLFW_KEY_KP_EQUAL;

        case vc::KeyboardInput::KeyboardLeftShift: return GLFW_KEY_LEFT_SHIFT;
        case vc::KeyboardInput::KeyboardLeftControl: return GLFW_KEY_LEFT_CONTROL;
        case vc::KeyboardInput::KeyboardLeftAlt: return GLFW_KEY_LEFT_ALT;
        case vc::KeyboardInput::KeyboardLeftSuper: return GLFW_KEY_LEFT_SUPER;
        case vc::KeyboardInput::KeyboardRightShift: return GLFW_KEY_RIGHT_SHIFT;
        case vc::KeyboardInput::KeyboardRightControl: return GLFW_KEY_RIGHT_CONTROL;
        case vc::KeyboardInput::KeyboardRightAlt: return GLFW_KEY_RIGHT_ALT;
        case vc::KeyboardInput::KeyboardRightSuper: return GLFW_KEY_RIGHT_SUPER;
        case vc::KeyboardInput::KeyboardMenu: return GLFW_KEY_MENU;

        case vc::KeyboardInput::KeyboardSpace: return GLFW_KEY_SPACE;
        case vc::KeyboardInput::KeyboardApostrophe: return GLFW_KEY_APOSTROPHE;
        case vc::KeyboardInput::KeyboardComma: return GLFW_KEY_COMMA;
        case vc::KeyboardInput::KeyboardMinus: return GLFW_KEY_MINUS;
        case vc::KeyboardInput::KeyboardPeriod: return GLFW_KEY_PERIOD;
        case vc::KeyboardInput::KeyboardSlash: return GLFW_KEY_SLASH;
        case vc::KeyboardInput::KeyboardSemicolon: return GLFW_KEY_SEMICOLON;
        case vc::KeyboardInput::KeyboardEqual: return GLFW_KEY_EQUAL;
        case vc::KeyboardInput::KeyboardLeftBracket: return GLFW_KEY_LEFT_BRACKET;
        case vc::KeyboardInput::KeyboardBackslash: return GLFW_KEY_BACKSLASH;
        case vc::KeyboardInput::KeyboardRightBracket: return GLFW_KEY_RIGHT_BRACKET;
        case vc::KeyboardInput::KeyboardGraveAccent: return GLFW_KEY_GRAVE_ACCENT;
        case vc::KeyboardInput::KeyboardWorld1: return GLFW_KEY_WORLD_1;
        case vc::KeyboardInput::KeyboardWorld2: return GLFW_KEY_WORLD_2;

        default: return GLFW_KEY_UNKNOWN;
    }
}

vc::KeyboardInput ContextGLFW::__ConvertGLFWEnumToKeyboardInput(int key) const
{
    switch (key) {
        case GLFW_KEY_A: return vc::KeyboardInput::KeyboardA;
        case GLFW_KEY_B: return vc::KeyboardInput::KeyboardB;
        case GLFW_KEY_C: return vc::KeyboardInput::KeyboardC;
        case GLFW_KEY_D: return vc::KeyboardInput::KeyboardD;
        case GLFW_KEY_E: return vc::KeyboardInput::KeyboardE;
        case GLFW_KEY_F: return vc::KeyboardInput::KeyboardF;
        case GLFW_KEY_G: return vc::KeyboardInput::KeyboardG;
        case GLFW_KEY_H: return vc::KeyboardInput::KeyboardH;
        case GLFW_KEY_I: return vc::KeyboardInput::KeyboardI;
        case GLFW_KEY_J: return vc::KeyboardInput::KeyboardJ;
        case GLFW_KEY_K: return vc::KeyboardInput::KeyboardK;
        case GLFW_KEY_L: return vc::KeyboardInput::KeyboardL;
        case GLFW_KEY_M: return vc::KeyboardInput::KeyboardM;
        case GLFW_KEY_N: return vc::KeyboardInput::KeyboardN;
        case GLFW_KEY_O: return vc::KeyboardInput::KeyboardO;
        case GLFW_KEY_P: return vc::KeyboardInput::KeyboardP;
        case GLFW_KEY_Q: return vc::KeyboardInput::KeyboardQ;
        case GLFW_KEY_R: return vc::KeyboardInput::KeyboardR;
        case GLFW_KEY_S: return vc::KeyboardInput::KeyboardS;
        case GLFW_KEY_T: return vc::KeyboardInput::KeyboardT;
        case GLFW_KEY_U: return vc::KeyboardInput::KeyboardU;
        case GLFW_KEY_V: return vc::KeyboardInput::KeyboardV;
        case GLFW_KEY_W: return vc::KeyboardInput::KeyboardW;
        case GLFW_KEY_X: return vc::KeyboardInput::KeyboardX;
        case GLFW_KEY_Y: return vc::KeyboardInput::KeyboardY;
        case GLFW_KEY_Z: return vc::KeyboardInput::KeyboardZ;

        case GLFW_KEY_0: return vc::KeyboardInput::Keyboard0;
        case GLFW_KEY_1: return vc::KeyboardInput::Keyboard1;
        case GLFW_KEY_2: return vc::KeyboardInput::Keyboard2;
        case GLFW_KEY_3: return vc::KeyboardInput::Keyboard3;
        case GLFW_KEY_4: return vc::KeyboardInput::Keyboard4;
        case GLFW_KEY_5: return vc::KeyboardInput::Keyboard5;
        case GLFW_KEY_6: return vc::KeyboardInput::Keyboard6;
        case GLFW_KEY_7: return vc::KeyboardInput::Keyboard7;
        case GLFW_KEY_8: return vc::KeyboardInput::Keyboard8;
        case GLFW_KEY_9: return vc::KeyboardInput::Keyboard9;

        case GLFW_KEY_ESCAPE: return vc::KeyboardInput::KeyboardEscape;
        case GLFW_KEY_ENTER: return vc::KeyboardInput::KeyboardEnter;
        case GLFW_KEY_TAB: return vc::KeyboardInput::KeyboardTab;
        case GLFW_KEY_BACKSPACE: return vc::KeyboardInput::KeyboardBackspace;
        case GLFW_KEY_INSERT: return vc::KeyboardInput::KeyboardInsert;
        case GLFW_KEY_DELETE: return vc::KeyboardInput::KeyboardDelete;
        case GLFW_KEY_RIGHT: return vc::KeyboardInput::KeyboardRight;
        case GLFW_KEY_LEFT: return vc::KeyboardInput::KeyboardLeft;
        case GLFW_KEY_DOWN: return vc::KeyboardInput::KeyboardDown;
        case GLFW_KEY_UP: return vc::KeyboardInput::KeyboardUp;
        case GLFW_KEY_PAGE_UP: return vc::KeyboardInput::KeyboardPageUp;
        case GLFW_KEY_PAGE_DOWN: return vc::KeyboardInput::KeyboardPageDown;
        case GLFW_KEY_HOME: return vc::KeyboardInput::KeyboardHome;
        case GLFW_KEY_END: return vc::KeyboardInput::KeyboardEnd;

        case GLFW_KEY_CAPS_LOCK: return vc::KeyboardInput::KeyboardCapsLock;
        case GLFW_KEY_SCROLL_LOCK: return vc::KeyboardInput::KeyboardScrollLock;
        case GLFW_KEY_NUM_LOCK: return vc::KeyboardInput::KeyboardNumLock;
        case GLFW_KEY_PRINT_SCREEN: return vc::KeyboardInput::KeyboardPrintScreen;
        case GLFW_KEY_PAUSE: return vc::KeyboardInput::KeyboardPause;

        case GLFW_KEY_F1: return vc::KeyboardInput::KeyboardF1;
        case GLFW_KEY_F2: return vc::KeyboardInput::KeyboardF2;
        case GLFW_KEY_F3: return vc::KeyboardInput::KeyboardF3;
        case GLFW_KEY_F4: return vc::KeyboardInput::KeyboardF4;
        case GLFW_KEY_F5: return vc::KeyboardInput::KeyboardF5;
        case GLFW_KEY_F6: return vc::KeyboardInput::KeyboardF6;
        case GLFW_KEY_F7: return vc::KeyboardInput::KeyboardF7;
        case GLFW_KEY_F8: return vc::KeyboardInput::KeyboardF8;
        case GLFW_KEY_F9: return vc::KeyboardInput::KeyboardF9;
        case GLFW_KEY_F10: return vc::KeyboardInput::KeyboardF10;
        case GLFW_KEY_F11: return vc::KeyboardInput::KeyboardF11;
        case GLFW_KEY_F12: return vc::KeyboardInput::KeyboardF12;
        case GLFW_KEY_F13: return vc::KeyboardInput::KeyboardF13;
        case GLFW_KEY_F14: return vc::KeyboardInput::KeyboardF14;
        case GLFW_KEY_F15: return vc::KeyboardInput::KeyboardF15;
        case GLFW_KEY_F16: return vc::KeyboardInput::KeyboardF16;
        case GLFW_KEY_F17: return vc::KeyboardInput::KeyboardF17;
        case GLFW_KEY_F18: return vc::KeyboardInput::KeyboardF18;
        case GLFW_KEY_F19: return vc::KeyboardInput::KeyboardF19;
        case GLFW_KEY_F20: return vc::KeyboardInput::KeyboardF20;
        case GLFW_KEY_F21: return vc::KeyboardInput::KeyboardF21;
        case GLFW_KEY_F22: return vc::KeyboardInput::KeyboardF22;
        case GLFW_KEY_F23: return vc::KeyboardInput::KeyboardF23;
        case GLFW_KEY_F24: return vc::KeyboardInput::KeyboardF24;
        case GLFW_KEY_F25: return vc::KeyboardInput::KeyboardF25;

        case GLFW_KEY_KP_0: return vc::KeyboardInput::KeyboardKP0;
        case GLFW_KEY_KP_1: return vc::KeyboardInput::KeyboardKP1;
        case GLFW_KEY_KP_2: return vc::KeyboardInput::KeyboardKP2;
        case GLFW_KEY_KP_3: return vc::KeyboardInput::KeyboardKP3;
        case GLFW_KEY_KP_4: return vc::KeyboardInput::KeyboardKP4;
        case GLFW_KEY_KP_5: return vc::KeyboardInput::KeyboardKP5;
        case GLFW_KEY_KP_6: return vc::KeyboardInput::KeyboardKP6;
        case GLFW_KEY_KP_7: return vc::KeyboardInput::KeyboardKP7;
        case GLFW_KEY_KP_8: return vc::KeyboardInput::KeyboardKP8;
        case GLFW_KEY_KP_9: return vc::KeyboardInput::KeyboardKP9;
        case GLFW_KEY_KP_DECIMAL: return vc::KeyboardInput::KeyboardKPDecimal;
        case GLFW_KEY_KP_DIVIDE: return vc::KeyboardInput::KeyboardKPDivide;
        case GLFW_KEY_KP_MULTIPLY: return vc::KeyboardInput::KeyboardKPMultiply;
        case GLFW_KEY_KP_SUBTRACT: return vc::KeyboardInput::KeyboardKPSubtract;
        case GLFW_KEY_KP_ADD: return vc::KeyboardInput::KeyboardKPAdd;
        case GLFW_KEY_KP_ENTER: return vc::KeyboardInput::KeyboardKPEnter;
        case GLFW_KEY_KP_EQUAL: return vc::KeyboardInput::KeyboardKPEqual;

        case GLFW_KEY_LEFT_SHIFT: return vc::KeyboardInput::KeyboardLeftShift;
        case GLFW_KEY_LEFT_CONTROL: return vc::KeyboardInput::KeyboardLeftControl;
        case GLFW_KEY_LEFT_ALT: return vc::KeyboardInput::KeyboardLeftAlt;
        case GLFW_KEY_LEFT_SUPER: return vc::KeyboardInput::KeyboardLeftSuper;
        case GLFW_KEY_RIGHT_SHIFT: return vc::KeyboardInput::KeyboardRightShift;
        case GLFW_KEY_RIGHT_CONTROL: return vc::KeyboardInput::KeyboardRightControl;
        case GLFW_KEY_RIGHT_ALT: return vc::KeyboardInput::KeyboardRightAlt;
        case GLFW_KEY_RIGHT_SUPER: return vc::KeyboardInput::KeyboardRightSuper;
        case GLFW_KEY_MENU: return vc::KeyboardInput::KeyboardMenu;

        case GLFW_KEY_SPACE: return vc::KeyboardInput::KeyboardSpace;
        case GLFW_KEY_APOSTROPHE: return vc::KeyboardInput::KeyboardApostrophe;
        case GLFW_KEY_COMMA: return vc::KeyboardInput::KeyboardComma;
        case GLFW_KEY_MINUS: return vc::KeyboardInput::KeyboardMinus;
        case GLFW_KEY_PERIOD: return vc::KeyboardInput::KeyboardPeriod;
        case GLFW_KEY_SLASH: return vc::KeyboardInput::KeyboardSlash;
        case GLFW_KEY_SEMICOLON: return vc::KeyboardInput::KeyboardSemicolon;
        case GLFW_KEY_EQUAL: return vc::KeyboardInput::KeyboardEqual;
        case GLFW_KEY_LEFT_BRACKET: return vc::KeyboardInput::KeyboardLeftBracket;
        case GLFW_KEY_BACKSLASH: return vc::KeyboardInput::KeyboardBackslash;
        case GLFW_KEY_RIGHT_BRACKET: return vc::KeyboardInput::KeyboardRightBracket;
        case GLFW_KEY_GRAVE_ACCENT: return vc::KeyboardInput::KeyboardGraveAccent;
        case GLFW_KEY_WORLD_1: return vc::KeyboardInput::KeyboardWorld1;
        case GLFW_KEY_WORLD_2: return vc::KeyboardInput::KeyboardWorld2;

        default: return vc::KeyboardInput::KeyboardUnknown;
    }
}
}
}
}