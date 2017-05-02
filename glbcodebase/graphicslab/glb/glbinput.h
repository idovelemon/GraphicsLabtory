//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 04 / 04
// Brief: Input mananger, include keyboard and mouse
//------------------------------------------------------------------
#ifndef GLB_GLBINPUT_H_
#define GLB_GLBINPUT_H_

#include <stdint.h>

namespace glb {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
enum ButtonMouse {
    BM_LEFT = 0x0,
    BM_RIGHT = 0x1,
    BM_CENTER = 0x2,
};

enum ButtonKey {
    BK_ESCAPE = 0x01,
    BK_1 = 0x02,
    BK_2 = 0x03,
    BK_3 = 0x04,
    BK_4 = 0x05,
    BK_5 = 0x06,
    BK_6 = 0x07,
    BK_7 = 0x08,
    BK_8 = 0x09,
    BK_9 = 0x0A,
    BK_0 = 0x0B,
    BK_Q = 0x10,
    BK_W = 0x11,
    BK_E = 0x12,
    BK_R = 0x13,
    BK_T = 0x14,
    BK_Y = 0x15,
    BK_U = 0x16,
    BK_I = 0x17,
    BK_O = 0x18,
    BK_P = 0x19,
    BK_A = 0x1E,
    BK_S = 0x1F,
    BK_D = 0x20,
    BK_F = 0x21,
    BK_G = 0x22,
    BK_H = 0x23,
    BK_J = 0x24,
    BK_K = 0x25,
    BK_L = 0x26,
    BK_Z = 0x2C,
    BK_X = 0x2D,
    BK_C = 0x2E,
    BK_V = 0x2F,
    BK_B = 0x30,
    BK_N = 0x31,
    BK_M = 0x32,
    BK_F1 = 0x3B,
    BK_F2 = 0x3C,
    BK_F3 = 0x3D,
    BK_F4 = 0x3E,
    BK_F5 = 0x3F,
    BK_F6 = 0x40,
    BK_F7 = 0x41,
    BK_F8 = 0x42,
    BK_F9 = 0x43,
    BK_F10 = 0x44,
    BK_F11 = 0x57,
    BK_F12 = 0x58,
    BK_F13 = 0x64,
    BK_F14 = 0x65,
    BK_F15 = 0x66,
    BK_UP = 0xC8,
    BK_LEFT = 0xCB,
    BK_RIGHT = 0xCD,
    BK_DOWN = 0xD0,
};

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Input DECLARATION
//----------------------------------------------------------------------------------
class Input {
public:
    static void Initialize();
    static void Update();
    static void Destroy();

    static int64_t GetMouseMoveX();
    static int64_t GetMouseMoveY();
    static bool IsMouseButtonPressed(ButtonMouse mouse);
    static bool IsKeyboardButtonPressed(ButtonKey key);
};

};  // namespace glb

#endif  // GLB_GLBINPUT_H_