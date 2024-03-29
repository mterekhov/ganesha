#ifndef SPCGANESHAENGINE_MACKEYCODE_H
#define SPCGANESHAENGINE_MACKEYCODE_H

namespace spcGaneshaEngine {

enum EMacKeyCode {
    MACKEYCODE_A = 0x00,
    MACKEYCODE_S = 0x01,
    MACKEYCODE_D = 0x02,
    MACKEYCODE_F = 0x03,
    MACKEYCODE_H = 0x04,
    MACKEYCODE_G = 0x05,
    MACKEYCODE_Z = 0x06,
    MACKEYCODE_X = 0x07,
    MACKEYCODE_C = 0x08,
    MACKEYCODE_V = 0x09,
    MACKEYCODE_B = 0x0B,
    MACKEYCODE_Q = 0x0C,
    MACKEYCODE_W = 0x0D,
    MACKEYCODE_E = 0x0E,
    MACKEYCODE_R = 0x0F,
    MACKEYCODE_Y = 0x10,
    MACKEYCODE_T = 0x11,
    MACKEYCODE_1 = 0x12,
    MACKEYCODE_2 = 0x13,
    MACKEYCODE_3 = 0x14,
    MACKEYCODE_4 = 0x15,
    MACKEYCODE_6 = 0x16,
    MACKEYCODE_5 = 0x17,
    MACKEYCODE_Equal = 0x18,
    MACKEYCODE_9 = 0x19,
    MACKEYCODE_7 = 0x1A,
    MACKEYCODE_Minus = 0x1B,
    MACKEYCODE_8 = 0x1C,
    MACKEYCODE_0 = 0x1D,
    MACKEYCODE_RightBracket = 0x1E,
    MACKEYCODE_O = 0x1F,
    MACKEYCODE_U = 0x20,
    MACKEYCODE_LeftBracket = 0x21,
    MACKEYCODE_I = 0x22,
    MACKEYCODE_P = 0x23,
    MACKEYCODE_L = 0x25,
    MACKEYCODE_J = 0x26,
    MACKEYCODE_Quote = 0x27,
    MACKEYCODE_K = 0x28,
    MACKEYCODE_Semicolon = 0x29,
    MACKEYCODE_Backslash = 0x2A,
    MACKEYCODE_Comma = 0x2B,
    MACKEYCODE_Slash = 0x2C,
    MACKEYCODE_N = 0x2D,
    MACKEYCODE_M = 0x2E,
    MACKEYCODE_Period = 0x2F,
    MACKEYCODE_Grave = 0x32,
    MACKEYCODE_KeypadDecimal = 0x41,
    MACKEYCODE_KeypadMultiply = 0x43,
    MACKEYCODE_KeypadPlus = 0x45,
    MACKEYCODE_KeypadClear = 0x47,
    MACKEYCODE_KeypadDivide = 0x4B,
    MACKEYCODE_KeypadEnter = 0x4C,
    MACKEYCODE_KeypadMinus = 0x4E,
    MACKEYCODE_KeypadEquals = 0x51,
    MACKEYCODE_Keypad0 = 0x52,
    MACKEYCODE_Keypad1 = 0x53,
    MACKEYCODE_Keypad2 = 0x54,
    MACKEYCODE_Keypad3 = 0x55,
    MACKEYCODE_Keypad4 = 0x56,
    MACKEYCODE_Keypad5 = 0x57,
    MACKEYCODE_Keypad6 = 0x58,
    MACKEYCODE_Keypad7 = 0x59,
    MACKEYCODE_Keypad8 = 0x5B,
    MACKEYCODE_Keypad9 = 0x5C,
    
    // keycodes for keys that are independent of keyboard layout
    MACKEYCODE_Return = 0x24,
    MACKEYCODE_Tab = 0x30,
    MACKEYCODE_Space = 0x31,
    MACKEYCODE_Delete = 0x33,
    MACKEYCODE_Escape = 0x35,
    MACKEYCODE_Command = 0x37,
    MACKEYCODE_Shift = 0x38,
    MACKEYCODE_CapsLock = 0x39,
    MACKEYCODE_Option = 0x3A,
    MACKEYCODE_Control = 0x3B,
    MACKEYCODE_RightCommand = 0x36, // Out of order
    MACKEYCODE_RightShift = 0x3C,
    MACKEYCODE_RightOption = 0x3D,
    MACKEYCODE_RightControl = 0x3E,
    MACKEYCODE_Function = 0x3F,
    MACKEYCODE_F17 = 0x40,
    MACKEYCODE_VolumeUp = 0x48,
    MACKEYCODE_VolumeDown = 0x49,
    MACKEYCODE_Mute = 0x4A,
    MACKEYCODE_F18 = 0x4F,
    MACKEYCODE_F19 = 0x50,
    MACKEYCODE_F20 = 0x5A,
    MACKEYCODE_F5 = 0x60,
    MACKEYCODE_F6 = 0x61,
    MACKEYCODE_F7 = 0x62,
    MACKEYCODE_F3 = 0x63,
    MACKEYCODE_F8 = 0x64,
    MACKEYCODE_F9 = 0x65,
    MACKEYCODE_F11 = 0x67,
    MACKEYCODE_F13 = 0x69,
    MACKEYCODE_F16 = 0x6A,
    MACKEYCODE_F14 = 0x6B,
    MACKEYCODE_F10 = 0x6D,
    MACKEYCODE_F12 = 0x6F,
    MACKEYCODE_F15 = 0x71,
    MACKEYCODE_Help = 0x72,
    MACKEYCODE_Home = 0x73,
    MACKEYCODE_PageUp = 0x74,
    MACKEYCODE_ForwardDelete = 0x75,
    MACKEYCODE_F4 = 0x76,
    MACKEYCODE_End = 0x77,
    MACKEYCODE_F2 = 0x78,
    MACKEYCODE_PageDown = 0x79,
    MACKEYCODE_F1 = 0x7A,
    MACKEYCODE_LeftArrow = 0x7B,
    MACKEYCODE_RightArrow = 0x7C,
    MACKEYCODE_DownArrow = 0x7D,
    MACKEYCODE_UpArrow = 0x7E,
    
    // ISO keyboards only
    MACKEYCODE_ISO_Section = 0x0A,
    
    // JIS keyboards only
    MACKEYCODE_JIS_Yen = 0x5D,
    MACKEYCODE_JIS_Underscore = 0x5E,
    MACKEYCODE_JIS_KeypadComma = 0x5F,
    MACKEYCODE_JIS_Eisu = 0x66,
    MACKEYCODE_JIS_Kana = 0x68
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_MACKEYCODE_H
