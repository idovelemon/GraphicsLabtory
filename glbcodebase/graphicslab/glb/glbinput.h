//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 04 / 04
// Brief: Input mananger, include keyboard and mouse
//------------------------------------------------------------------
#ifndef GLB_GLBINPUT_H_
#define GLB_GLBINPUT_H_

namespace glb {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------

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
    static void Destroy();

    // Mouse
    static float GetMouseMoveX();
    static float GetMouseMoveY();
    static void SetMouseMoveX(float move);
    static void SetMouseMoveY(float move);
};

};  // namespace glb

#endif  // GLB_GLBINPUT_H_