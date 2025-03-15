///
/// Project: VenomEngineWorkspace
/// @file Context_Apple_Input.cc
/// @date Mar, 09 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/context/apple/ContextAppleViewController.h>

vc::KeyboardInput convertAppleKeyToVCKey(unsigned short keyCode) {
    switch (keyCode) {
        case 0x00: return vc::KeyboardInput::KeyboardA;
        case 0x0B: return vc::KeyboardInput::KeyboardB;
        case 0x08: return vc::KeyboardInput::KeyboardC;
        case 0x02: return vc::KeyboardInput::KeyboardD;
        case 0x0E: return vc::KeyboardInput::KeyboardE;
        case 0x03: return vc::KeyboardInput::KeyboardF;
        case 0x05: return vc::KeyboardInput::KeyboardG;
        case 0x04: return vc::KeyboardInput::KeyboardH;
        case 0x22: return vc::KeyboardInput::KeyboardI;
        case 0x26: return vc::KeyboardInput::KeyboardJ;
        case 0x28: return vc::KeyboardInput::KeyboardK;
        case 0x25: return vc::KeyboardInput::KeyboardL;
        case 0x2E: return vc::KeyboardInput::KeyboardM;
        case 0x2D: return vc::KeyboardInput::KeyboardN;
        case 0x1F: return vc::KeyboardInput::KeyboardO;
        case 0x23: return vc::KeyboardInput::KeyboardP;
        case 0x0C: return vc::KeyboardInput::KeyboardQ;
        case 0x0F: return vc::KeyboardInput::KeyboardR;
        case 0x01: return vc::KeyboardInput::KeyboardS;
        case 0x11: return vc::KeyboardInput::KeyboardT;
        case 0x20: return vc::KeyboardInput::KeyboardU;
        case 0x09: return vc::KeyboardInput::KeyboardV;
        case 0x0D: return vc::KeyboardInput::KeyboardW;
        case 0x07: return vc::KeyboardInput::KeyboardX;
        case 0x10: return vc::KeyboardInput::KeyboardY;
        case 0x06: return vc::KeyboardInput::KeyboardZ;

        case 0x1D: return vc::KeyboardInput::Keyboard0;
        case 0x12: return vc::KeyboardInput::Keyboard1;
        case 0x13: return vc::KeyboardInput::Keyboard2;
        case 0x14: return vc::KeyboardInput::Keyboard3;
        case 0x15: return vc::KeyboardInput::Keyboard4;
        case 0x17: return vc::KeyboardInput::Keyboard5;
        case 0x16: return vc::KeyboardInput::Keyboard6;
        case 0x1A: return vc::KeyboardInput::Keyboard7;
        case 0x1C: return vc::KeyboardInput::Keyboard8;
        case 0x19: return vc::KeyboardInput::Keyboard9;

        case 0x35: return vc::KeyboardInput::KeyboardEscape;
        case 0x24: return vc::KeyboardInput::KeyboardEnter;
        case 0x30: return vc::KeyboardInput::KeyboardTab;
        case 0x33: return vc::KeyboardInput::KeyboardBackspace;
        case 0x72: return vc::KeyboardInput::KeyboardInsert;
        case 0x75: return vc::KeyboardInput::KeyboardDelete;
        case 0x7C: return vc::KeyboardInput::KeyboardRight;
        case 0x7B: return vc::KeyboardInput::KeyboardLeft;
        case 0x7D: return vc::KeyboardInput::KeyboardDown;
        case 0x7E: return vc::KeyboardInput::KeyboardUp;
        case 0x74: return vc::KeyboardInput::KeyboardPageUp;
        case 0x79: return vc::KeyboardInput::KeyboardPageDown;
        case 0x73: return vc::KeyboardInput::KeyboardHome;
        case 0x77: return vc::KeyboardInput::KeyboardEnd;

        case 0x39: return vc::KeyboardInput::KeyboardCapsLock;
        case 0x6B: return vc::KeyboardInput::KeyboardScrollLock;
        case 0x47: return vc::KeyboardInput::KeyboardNumLock;
        case 0x69: return vc::KeyboardInput::KeyboardPrintScreen;
        case 0x71: return vc::KeyboardInput::KeyboardPause;

        case 0x7A: return vc::KeyboardInput::KeyboardF1;
        case 0x78: return vc::KeyboardInput::KeyboardF2;
        case 0x63: return vc::KeyboardInput::KeyboardF3;
        case 0x76: return vc::KeyboardInput::KeyboardF4;
        case 0x60: return vc::KeyboardInput::KeyboardF5;
        case 0x61: return vc::KeyboardInput::KeyboardF6;
        case 0x62: return vc::KeyboardInput::KeyboardF7;
        case 0x64: return vc::KeyboardInput::KeyboardF8;
        case 0x65: return vc::KeyboardInput::KeyboardF9;
        case 0x6D: return vc::KeyboardInput::KeyboardF10;
        case 0x67: return vc::KeyboardInput::KeyboardF11;
        case 0x6F: return vc::KeyboardInput::KeyboardF12;
        case 0x6A: return vc::KeyboardInput::KeyboardF16;
        case 0x40: return vc::KeyboardInput::KeyboardF17;
        case 0x4F: return vc::KeyboardInput::KeyboardF18;
        case 0x50: return vc::KeyboardInput::KeyboardF19;
        case 0x5A: return vc::KeyboardInput::KeyboardF20;

        case 0x52: return vc::KeyboardInput::KeyboardKP0;
        case 0x53: return vc::KeyboardInput::KeyboardKP1;
        case 0x54: return vc::KeyboardInput::KeyboardKP2;
        case 0x55: return vc::KeyboardInput::KeyboardKP3;
        case 0x56: return vc::KeyboardInput::KeyboardKP4;
        case 0x57: return vc::KeyboardInput::KeyboardKP5;
        case 0x58: return vc::KeyboardInput::KeyboardKP6;
        case 0x59: return vc::KeyboardInput::KeyboardKP7;
        case 0x5B: return vc::KeyboardInput::KeyboardKP8;
        case 0x5C: return vc::KeyboardInput::KeyboardKP9;
        case 0x41: return vc::KeyboardInput::KeyboardKPDecimal;
        case 0x4B: return vc::KeyboardInput::KeyboardKPDivide;
        case 0x43: return vc::KeyboardInput::KeyboardKPMultiply;
        case 0x4E: return vc::KeyboardInput::KeyboardKPSubtract;
        case 0x45: return vc::KeyboardInput::KeyboardKPAdd;
        case 0x4C: return vc::KeyboardInput::KeyboardKPEnter;
        case 0x51: return vc::KeyboardInput::KeyboardKPEqual;

        case 0x38: return vc::KeyboardInput::KeyboardLeftShift;
        case 0x3B: return vc::KeyboardInput::KeyboardLeftControl;
        case 0x3A: return vc::KeyboardInput::KeyboardLeftAlt;
        case 0x37: return vc::KeyboardInput::KeyboardLeftSuper;
        case 0x3C: return vc::KeyboardInput::KeyboardRightShift;
        case 0x3E: return vc::KeyboardInput::KeyboardRightControl;
        case 0x3D: return vc::KeyboardInput::KeyboardRightAlt;
        case 0x36: return vc::KeyboardInput::KeyboardRightSuper;
        case 0x6E: return vc::KeyboardInput::KeyboardMenu;

        case 0x31: return vc::KeyboardInput::KeyboardSpace;
        case 0x27: return vc::KeyboardInput::KeyboardApostrophe;
        case 0x2B: return vc::KeyboardInput::KeyboardComma;
        case 0x1B: return vc::KeyboardInput::KeyboardMinus;
        case 0x2F: return vc::KeyboardInput::KeyboardPeriod;
        case 0x2C: return vc::KeyboardInput::KeyboardSlash;
        case 0x29: return vc::KeyboardInput::KeyboardSemicolon;
        case 0x18: return vc::KeyboardInput::KeyboardEqual;
        case 0x21: return vc::KeyboardInput::KeyboardLeftBracket;
        case 0x2A: return vc::KeyboardInput::KeyboardBackslash;
        case 0x1E: return vc::KeyboardInput::KeyboardRightBracket;
        case 0x32: return vc::KeyboardInput::KeyboardGraveAccent;

        default: return vc::KeyboardInput::KeyboardUnknown;
    }
}

unsigned short convertVCKeyToAppleKey(vc::KeyboardInput key) {
    switch (key) {
        case vc::KeyboardInput::KeyboardA: return 0x00;
        case vc::KeyboardInput::KeyboardB: return 0x0B;
        case vc::KeyboardInput::KeyboardC: return 0x08;
        case vc::KeyboardInput::KeyboardD: return 0x02;
        case vc::KeyboardInput::KeyboardE: return 0x0E;
        case vc::KeyboardInput::KeyboardF: return 0x03;
        case vc::KeyboardInput::KeyboardG: return 0x05;
        case vc::KeyboardInput::KeyboardH: return 0x04;
        case vc::KeyboardInput::KeyboardI: return 0x22;
        case vc::KeyboardInput::KeyboardJ: return 0x26;
        case vc::KeyboardInput::KeyboardK: return 0x28;
        case vc::KeyboardInput::KeyboardL: return 0x25;
        case vc::KeyboardInput::KeyboardM: return 0x2E;
        case vc::KeyboardInput::KeyboardN: return 0x2D;
        case vc::KeyboardInput::KeyboardO: return 0x1F;
        case vc::KeyboardInput::KeyboardP: return 0x23;
        case vc::KeyboardInput::KeyboardQ: return 0x0C;
        case vc::KeyboardInput::KeyboardR: return 0x0F;
        case vc::KeyboardInput::KeyboardS: return 0x01;
        case vc::KeyboardInput::KeyboardT: return 0x11;
        case vc::KeyboardInput::KeyboardU: return 0x20;
        case vc::KeyboardInput::KeyboardV: return 0x09;
        case vc::KeyboardInput::KeyboardW: return 0x0D;
        case vc::KeyboardInput::KeyboardX: return 0x07;
        case vc::KeyboardInput::KeyboardY: return 0x10;
        case vc::KeyboardInput::KeyboardZ: return 0x06;
        
        case vc::KeyboardInput::Keyboard0: return 0x1D;
        case vc::KeyboardInput::Keyboard1: return 0x12;
        case vc::KeyboardInput::Keyboard2: return 0x13;
        case vc::KeyboardInput::Keyboard3: return 0x14;
        case vc::KeyboardInput::Keyboard4: return 0x15;
        case vc::KeyboardInput::Keyboard5: return 0x17;
        case vc::KeyboardInput::Keyboard6: return 0x16;
        case vc::KeyboardInput::Keyboard7: return 0x1A;
        case vc::KeyboardInput::Keyboard8: return 0x1C;
        case vc::KeyboardInput::Keyboard9: return 0x19;
        
        case vc::KeyboardInput::KeyboardEscape: return 0x35;
        case vc::KeyboardInput::KeyboardEnter: return 0x24;
        case vc::KeyboardInput::KeyboardTab: return 0x30;
        case vc::KeyboardInput::KeyboardBackspace: return 0x33;
        case vc::KeyboardInput::KeyboardInsert: return 0x72;
        case vc::KeyboardInput::KeyboardDelete: return 0x75;
        case vc::KeyboardInput::KeyboardRight: return 0x7C;
        case vc::KeyboardInput::KeyboardLeft: return 0x7B;
        case vc::KeyboardInput::KeyboardDown: return 0x7D;
        case vc::KeyboardInput::KeyboardUp: return 0x7E;
        case vc::KeyboardInput::KeyboardPageUp: return 0x74;
        case vc::KeyboardInput::KeyboardPageDown: return 0x79;
        case vc::KeyboardInput::KeyboardHome: return 0x73;
        case vc::KeyboardInput::KeyboardEnd: return 0x77;
        
        case vc::KeyboardInput::KeyboardSpace: return 0x31;
        case vc::KeyboardInput::KeyboardApostrophe: return 0x27;
        case vc::KeyboardInput::KeyboardComma: return 0x2B;
        case vc::KeyboardInput::KeyboardMinus: return 0x1B;
        case vc::KeyboardInput::KeyboardPeriod: return 0x2F;
        case vc::KeyboardInput::KeyboardSlash: return 0x2C;
        case vc::KeyboardInput::KeyboardSemicolon: return 0x29;
        case vc::KeyboardInput::KeyboardEqual: return 0x18;
        case vc::KeyboardInput::KeyboardLeftBracket: return 0x21;
        case vc::KeyboardInput::KeyboardBackslash: return 0x2A;
        case vc::KeyboardInput::KeyboardRightBracket: return 0x1E;
        case vc::KeyboardInput::KeyboardGraveAccent: return 0x32;
        
        default: return 0xFF; // Invalid or unmapped key
    }
}
