///
/// Project: VenomEngineWorkspace
/// @file ContextInput.cc
/// @date Oct, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Context.h>

namespace venom
{
namespace common
{

int Context::__ConvertKeyboardInputToGLFWEnum(KeyboardInput key) const
{
    switch (key)
    {
        case KeyboardInput::KeyboardA: return GLFW_KEY_A;
        case KeyboardInput::KeyboardB: return GLFW_KEY_B;
        case KeyboardInput::KeyboardC: return GLFW_KEY_C;
        case KeyboardInput::KeyboardD: return GLFW_KEY_D;
        case KeyboardInput::KeyboardE: return GLFW_KEY_E;
        case KeyboardInput::KeyboardF: return GLFW_KEY_F;
        case KeyboardInput::KeyboardG: return GLFW_KEY_G;
        case KeyboardInput::KeyboardH: return GLFW_KEY_H;
        case KeyboardInput::KeyboardI: return GLFW_KEY_I;
        case KeyboardInput::KeyboardJ: return GLFW_KEY_J;
        case KeyboardInput::KeyboardK: return GLFW_KEY_K;
        case KeyboardInput::KeyboardL: return GLFW_KEY_L;
        case KeyboardInput::KeyboardM: return GLFW_KEY_M;
        case KeyboardInput::KeyboardN: return GLFW_KEY_N;
        case KeyboardInput::KeyboardO: return GLFW_KEY_O;
        case KeyboardInput::KeyboardP: return GLFW_KEY_P;
        case KeyboardInput::KeyboardQ: return GLFW_KEY_Q;
        case KeyboardInput::KeyboardR: return GLFW_KEY_R;
        case KeyboardInput::KeyboardS: return GLFW_KEY_S;
        case KeyboardInput::KeyboardT: return GLFW_KEY_T;
        case KeyboardInput::KeyboardU: return GLFW_KEY_U;
        case KeyboardInput::KeyboardV: return GLFW_KEY_V;
        case KeyboardInput::KeyboardW: return GLFW_KEY_W;
        case KeyboardInput::KeyboardX: return GLFW_KEY_X;
        case KeyboardInput::KeyboardY: return GLFW_KEY_Y;
        case KeyboardInput::KeyboardZ: return GLFW_KEY_Z;

        case KeyboardInput::Keyboard0: return GLFW_KEY_0;
        case KeyboardInput::Keyboard1: return GLFW_KEY_1;
        case KeyboardInput::Keyboard2: return GLFW_KEY_2;
        case KeyboardInput::Keyboard3: return GLFW_KEY_3;
        case KeyboardInput::Keyboard4: return GLFW_KEY_4;
        case KeyboardInput::Keyboard5: return GLFW_KEY_5;
        case KeyboardInput::Keyboard6: return GLFW_KEY_6;
        case KeyboardInput::Keyboard7: return GLFW_KEY_7;
        case KeyboardInput::Keyboard8: return GLFW_KEY_8;
        case KeyboardInput::Keyboard9: return GLFW_KEY_9;

        case KeyboardInput::KeyboardEscape: return GLFW_KEY_ESCAPE;
        case KeyboardInput::KeyboardEnter: return GLFW_KEY_ENTER;
        case KeyboardInput::KeyboardTab: return GLFW_KEY_TAB;
        case KeyboardInput::KeyboardBackspace: return GLFW_KEY_BACKSPACE;
        case KeyboardInput::KeyboardInsert: return GLFW_KEY_INSERT;
        case KeyboardInput::KeyboardDelete: return GLFW_KEY_DELETE;
        case KeyboardInput::KeyboardRight: return GLFW_KEY_RIGHT;
        case KeyboardInput::KeyboardLeft: return GLFW_KEY_LEFT;
        case KeyboardInput::KeyboardDown: return GLFW_KEY_DOWN;
        case KeyboardInput::KeyboardUp: return GLFW_KEY_UP;
        case KeyboardInput::KeyboardPageUp: return GLFW_KEY_PAGE_UP;
        case KeyboardInput::KeyboardPageDown: return GLFW_KEY_PAGE_DOWN;
        case KeyboardInput::KeyboardHome: return GLFW_KEY_HOME;
        case KeyboardInput::KeyboardEnd: return GLFW_KEY_END;

        case KeyboardInput::KeyboardCapsLock: return GLFW_KEY_CAPS_LOCK;
        case KeyboardInput::KeyboardScrollLock: return GLFW_KEY_SCROLL_LOCK;
        case KeyboardInput::KeyboardNumLock: return GLFW_KEY_NUM_LOCK;
        case KeyboardInput::KeyboardPrintScreen: return GLFW_KEY_PRINT_SCREEN;
        case KeyboardInput::KeyboardPause: return GLFW_KEY_PAUSE;

        case KeyboardInput::KeyboardF1: return GLFW_KEY_F1;
        case KeyboardInput::KeyboardF2: return GLFW_KEY_F2;
        case KeyboardInput::KeyboardF3: return GLFW_KEY_F3;
        case KeyboardInput::KeyboardF4: return GLFW_KEY_F4;
        case KeyboardInput::KeyboardF5: return GLFW_KEY_F5;
        case KeyboardInput::KeyboardF6: return GLFW_KEY_F6;
        case KeyboardInput::KeyboardF7: return GLFW_KEY_F7;
        case KeyboardInput::KeyboardF8: return GLFW_KEY_F8;
        case KeyboardInput::KeyboardF9: return GLFW_KEY_F9;
        case KeyboardInput::KeyboardF10: return GLFW_KEY_F10;
        case KeyboardInput::KeyboardF11: return GLFW_KEY_F11;
        case KeyboardInput::KeyboardF12: return GLFW_KEY_F12;
        case KeyboardInput::KeyboardF13: return GLFW_KEY_F13;
        case KeyboardInput::KeyboardF14: return GLFW_KEY_F14;
        case KeyboardInput::KeyboardF15: return GLFW_KEY_F15;
        case KeyboardInput::KeyboardF16: return GLFW_KEY_F16;
        case KeyboardInput::KeyboardF17: return GLFW_KEY_F17;
        case KeyboardInput::KeyboardF18: return GLFW_KEY_F18;
        case KeyboardInput::KeyboardF19: return GLFW_KEY_F19;
        case KeyboardInput::KeyboardF20: return GLFW_KEY_F20;
        case KeyboardInput::KeyboardF21: return GLFW_KEY_F21;
        case KeyboardInput::KeyboardF22: return GLFW_KEY_F22;
        case KeyboardInput::KeyboardF23: return GLFW_KEY_F23;
        case KeyboardInput::KeyboardF24: return GLFW_KEY_F24;
        case KeyboardInput::KeyboardF25: return GLFW_KEY_F25;

        case KeyboardInput::KeyboardKP0: return GLFW_KEY_KP_0;
        case KeyboardInput::KeyboardKP1: return GLFW_KEY_KP_1;
        case KeyboardInput::KeyboardKP2: return GLFW_KEY_KP_2;
        case KeyboardInput::KeyboardKP3: return GLFW_KEY_KP_3;
        case KeyboardInput::KeyboardKP4: return GLFW_KEY_KP_4;
        case KeyboardInput::KeyboardKP5: return GLFW_KEY_KP_5;
        case KeyboardInput::KeyboardKP6: return GLFW_KEY_KP_6;
        case KeyboardInput::KeyboardKP7: return GLFW_KEY_KP_7;
        case KeyboardInput::KeyboardKP8: return GLFW_KEY_KP_8;
        case KeyboardInput::KeyboardKP9: return GLFW_KEY_KP_9;
        case KeyboardInput::KeyboardKPDecimal: return GLFW_KEY_KP_DECIMAL;
        case KeyboardInput::KeyboardKPDivide: return GLFW_KEY_KP_DIVIDE;
        case KeyboardInput::KeyboardKPMultiply: return GLFW_KEY_KP_MULTIPLY;
        case KeyboardInput::KeyboardKPSubtract: return GLFW_KEY_KP_SUBTRACT;
        case KeyboardInput::KeyboardKPAdd: return GLFW_KEY_KP_ADD;
        case KeyboardInput::KeyboardKPEnter: return GLFW_KEY_KP_ENTER;
        case KeyboardInput::KeyboardKPEqual: return GLFW_KEY_KP_EQUAL;

        case KeyboardInput::KeyboardLeftShift: return GLFW_KEY_LEFT_SHIFT;
        case KeyboardInput::KeyboardLeftControl: return GLFW_KEY_LEFT_CONTROL;
        case KeyboardInput::KeyboardLeftAlt: return GLFW_KEY_LEFT_ALT;
        case KeyboardInput::KeyboardLeftSuper: return GLFW_KEY_LEFT_SUPER;
        case KeyboardInput::KeyboardRightShift: return GLFW_KEY_RIGHT_SHIFT;
        case KeyboardInput::KeyboardRightControl: return GLFW_KEY_RIGHT_CONTROL;
        case KeyboardInput::KeyboardRightAlt: return GLFW_KEY_RIGHT_ALT;
        case KeyboardInput::KeyboardRightSuper: return GLFW_KEY_RIGHT_SUPER;
        case KeyboardInput::KeyboardMenu: return GLFW_KEY_MENU;

        case KeyboardInput::KeyboardSpace: return GLFW_KEY_SPACE;
        case KeyboardInput::KeyboardApostrophe: return GLFW_KEY_APOSTROPHE;
        case KeyboardInput::KeyboardComma: return GLFW_KEY_COMMA;
        case KeyboardInput::KeyboardMinus: return GLFW_KEY_MINUS;
        case KeyboardInput::KeyboardPeriod: return GLFW_KEY_PERIOD;
        case KeyboardInput::KeyboardSlash: return GLFW_KEY_SLASH;
        case KeyboardInput::KeyboardSemicolon: return GLFW_KEY_SEMICOLON;
        case KeyboardInput::KeyboardEqual: return GLFW_KEY_EQUAL;
        case KeyboardInput::KeyboardLeftBracket: return GLFW_KEY_LEFT_BRACKET;
        case KeyboardInput::KeyboardBackslash: return GLFW_KEY_BACKSLASH;
        case KeyboardInput::KeyboardRightBracket: return GLFW_KEY_RIGHT_BRACKET;
        case KeyboardInput::KeyboardGraveAccent: return GLFW_KEY_GRAVE_ACCENT;
        case KeyboardInput::KeyboardWorld1: return GLFW_KEY_WORLD_1;
        case KeyboardInput::KeyboardWorld2: return GLFW_KEY_WORLD_2;

        default: return GLFW_KEY_UNKNOWN;
    }
}

KeyboardInput Context::__ConvertGLFWEnumToKeyboardInput(int key) const
{
    switch (key) {
        case GLFW_KEY_A: return KeyboardInput::KeyboardA;
        case GLFW_KEY_B: return KeyboardInput::KeyboardB;
        case GLFW_KEY_C: return KeyboardInput::KeyboardC;
        case GLFW_KEY_D: return KeyboardInput::KeyboardD;
        case GLFW_KEY_E: return KeyboardInput::KeyboardE;
        case GLFW_KEY_F: return KeyboardInput::KeyboardF;
        case GLFW_KEY_G: return KeyboardInput::KeyboardG;
        case GLFW_KEY_H: return KeyboardInput::KeyboardH;
        case GLFW_KEY_I: return KeyboardInput::KeyboardI;
        case GLFW_KEY_J: return KeyboardInput::KeyboardJ;
        case GLFW_KEY_K: return KeyboardInput::KeyboardK;
        case GLFW_KEY_L: return KeyboardInput::KeyboardL;
        case GLFW_KEY_M: return KeyboardInput::KeyboardM;
        case GLFW_KEY_N: return KeyboardInput::KeyboardN;
        case GLFW_KEY_O: return KeyboardInput::KeyboardO;
        case GLFW_KEY_P: return KeyboardInput::KeyboardP;
        case GLFW_KEY_Q: return KeyboardInput::KeyboardQ;
        case GLFW_KEY_R: return KeyboardInput::KeyboardR;
        case GLFW_KEY_S: return KeyboardInput::KeyboardS;
        case GLFW_KEY_T: return KeyboardInput::KeyboardT;
        case GLFW_KEY_U: return KeyboardInput::KeyboardU;
        case GLFW_KEY_V: return KeyboardInput::KeyboardV;
        case GLFW_KEY_W: return KeyboardInput::KeyboardW;
        case GLFW_KEY_X: return KeyboardInput::KeyboardX;
        case GLFW_KEY_Y: return KeyboardInput::KeyboardY;
        case GLFW_KEY_Z: return KeyboardInput::KeyboardZ;

        case GLFW_KEY_0: return KeyboardInput::Keyboard0;
        case GLFW_KEY_1: return KeyboardInput::Keyboard1;
        case GLFW_KEY_2: return KeyboardInput::Keyboard2;
        case GLFW_KEY_3: return KeyboardInput::Keyboard3;
        case GLFW_KEY_4: return KeyboardInput::Keyboard4;
        case GLFW_KEY_5: return KeyboardInput::Keyboard5;
        case GLFW_KEY_6: return KeyboardInput::Keyboard6;
        case GLFW_KEY_7: return KeyboardInput::Keyboard7;
        case GLFW_KEY_8: return KeyboardInput::Keyboard8;
        case GLFW_KEY_9: return KeyboardInput::Keyboard9;

        case GLFW_KEY_ESCAPE: return KeyboardInput::KeyboardEscape;
        case GLFW_KEY_ENTER: return KeyboardInput::KeyboardEnter;
        case GLFW_KEY_TAB: return KeyboardInput::KeyboardTab;
        case GLFW_KEY_BACKSPACE: return KeyboardInput::KeyboardBackspace;
        case GLFW_KEY_INSERT: return KeyboardInput::KeyboardInsert;
        case GLFW_KEY_DELETE: return KeyboardInput::KeyboardDelete;
        case GLFW_KEY_RIGHT: return KeyboardInput::KeyboardRight;
        case GLFW_KEY_LEFT: return KeyboardInput::KeyboardLeft;
        case GLFW_KEY_DOWN: return KeyboardInput::KeyboardDown;
        case GLFW_KEY_UP: return KeyboardInput::KeyboardUp;
        case GLFW_KEY_PAGE_UP: return KeyboardInput::KeyboardPageUp;
        case GLFW_KEY_PAGE_DOWN: return KeyboardInput::KeyboardPageDown;
        case GLFW_KEY_HOME: return KeyboardInput::KeyboardHome;
        case GLFW_KEY_END: return KeyboardInput::KeyboardEnd;

        case GLFW_KEY_CAPS_LOCK: return KeyboardInput::KeyboardCapsLock;
        case GLFW_KEY_SCROLL_LOCK: return KeyboardInput::KeyboardScrollLock;
        case GLFW_KEY_NUM_LOCK: return KeyboardInput::KeyboardNumLock;
        case GLFW_KEY_PRINT_SCREEN: return KeyboardInput::KeyboardPrintScreen;
        case GLFW_KEY_PAUSE: return KeyboardInput::KeyboardPause;

        case GLFW_KEY_F1: return KeyboardInput::KeyboardF1;
        case GLFW_KEY_F2: return KeyboardInput::KeyboardF2;
        case GLFW_KEY_F3: return KeyboardInput::KeyboardF3;
        case GLFW_KEY_F4: return KeyboardInput::KeyboardF4;
        case GLFW_KEY_F5: return KeyboardInput::KeyboardF5;
        case GLFW_KEY_F6: return KeyboardInput::KeyboardF6;
        case GLFW_KEY_F7: return KeyboardInput::KeyboardF7;
        case GLFW_KEY_F8: return KeyboardInput::KeyboardF8;
        case GLFW_KEY_F9: return KeyboardInput::KeyboardF9;
        case GLFW_KEY_F10: return KeyboardInput::KeyboardF10;
        case GLFW_KEY_F11: return KeyboardInput::KeyboardF11;
        case GLFW_KEY_F12: return KeyboardInput::KeyboardF12;
        case GLFW_KEY_F13: return KeyboardInput::KeyboardF13;
        case GLFW_KEY_F14: return KeyboardInput::KeyboardF14;
        case GLFW_KEY_F15: return KeyboardInput::KeyboardF15;
        case GLFW_KEY_F16: return KeyboardInput::KeyboardF16;
        case GLFW_KEY_F17: return KeyboardInput::KeyboardF17;
        case GLFW_KEY_F18: return KeyboardInput::KeyboardF18;
        case GLFW_KEY_F19: return KeyboardInput::KeyboardF19;
        case GLFW_KEY_F20: return KeyboardInput::KeyboardF20;
        case GLFW_KEY_F21: return KeyboardInput::KeyboardF21;
        case GLFW_KEY_F22: return KeyboardInput::KeyboardF22;
        case GLFW_KEY_F23: return KeyboardInput::KeyboardF23;
        case GLFW_KEY_F24: return KeyboardInput::KeyboardF24;
        case GLFW_KEY_F25: return KeyboardInput::KeyboardF25;

        case GLFW_KEY_KP_0: return KeyboardInput::KeyboardKP0;
        case GLFW_KEY_KP_1: return KeyboardInput::KeyboardKP1;
        case GLFW_KEY_KP_2: return KeyboardInput::KeyboardKP2;
        case GLFW_KEY_KP_3: return KeyboardInput::KeyboardKP3;
        case GLFW_KEY_KP_4: return KeyboardInput::KeyboardKP4;
        case GLFW_KEY_KP_5: return KeyboardInput::KeyboardKP5;
        case GLFW_KEY_KP_6: return KeyboardInput::KeyboardKP6;
        case GLFW_KEY_KP_7: return KeyboardInput::KeyboardKP7;
        case GLFW_KEY_KP_8: return KeyboardInput::KeyboardKP8;
        case GLFW_KEY_KP_9: return KeyboardInput::KeyboardKP9;
        case GLFW_KEY_KP_DECIMAL: return KeyboardInput::KeyboardKPDecimal;
        case GLFW_KEY_KP_DIVIDE: return KeyboardInput::KeyboardKPDivide;
        case GLFW_KEY_KP_MULTIPLY: return KeyboardInput::KeyboardKPMultiply;
        case GLFW_KEY_KP_SUBTRACT: return KeyboardInput::KeyboardKPSubtract;
        case GLFW_KEY_KP_ADD: return KeyboardInput::KeyboardKPAdd;
        case GLFW_KEY_KP_ENTER: return KeyboardInput::KeyboardKPEnter;
        case GLFW_KEY_KP_EQUAL: return KeyboardInput::KeyboardKPEqual;

        case GLFW_KEY_LEFT_SHIFT: return KeyboardInput::KeyboardLeftShift;
        case GLFW_KEY_LEFT_CONTROL: return KeyboardInput::KeyboardLeftControl;
        case GLFW_KEY_LEFT_ALT: return KeyboardInput::KeyboardLeftAlt;
        case GLFW_KEY_LEFT_SUPER: return KeyboardInput::KeyboardLeftSuper;
        case GLFW_KEY_RIGHT_SHIFT: return KeyboardInput::KeyboardRightShift;
        case GLFW_KEY_RIGHT_CONTROL: return KeyboardInput::KeyboardRightControl;
        case GLFW_KEY_RIGHT_ALT: return KeyboardInput::KeyboardRightAlt;
        case GLFW_KEY_RIGHT_SUPER: return KeyboardInput::KeyboardRightSuper;
        case GLFW_KEY_MENU: return KeyboardInput::KeyboardMenu;

        case GLFW_KEY_SPACE: return KeyboardInput::KeyboardSpace;
        case GLFW_KEY_APOSTROPHE: return KeyboardInput::KeyboardApostrophe;
        case GLFW_KEY_COMMA: return KeyboardInput::KeyboardComma;
        case GLFW_KEY_MINUS: return KeyboardInput::KeyboardMinus;
        case GLFW_KEY_PERIOD: return KeyboardInput::KeyboardPeriod;
        case GLFW_KEY_SLASH: return KeyboardInput::KeyboardSlash;
        case GLFW_KEY_SEMICOLON: return KeyboardInput::KeyboardSemicolon;
        case GLFW_KEY_EQUAL: return KeyboardInput::KeyboardEqual;
        case GLFW_KEY_LEFT_BRACKET: return KeyboardInput::KeyboardLeftBracket;
        case GLFW_KEY_BACKSLASH: return KeyboardInput::KeyboardBackslash;
        case GLFW_KEY_RIGHT_BRACKET: return KeyboardInput::KeyboardRightBracket;
        case GLFW_KEY_GRAVE_ACCENT: return KeyboardInput::KeyboardGraveAccent;
        case GLFW_KEY_WORLD_1: return KeyboardInput::KeyboardWorld1;
        case GLFW_KEY_WORLD_2: return KeyboardInput::KeyboardWorld2;

        default: return KeyboardInput::KeyboardUnknown;
    }
}
}
}