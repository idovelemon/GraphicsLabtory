//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 04 / 04
// Brief: Input mananger, include keyboard and mouse
//------------------------------------------------------------------
#include "glbinput.h"

#include "util/glbmacro.h"

namespace glb {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class InputImp;
static InputImp* s_InputImp = NULL;

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// InputImp DECLARATION
//----------------------------------------------------------------------------------
class InputImp {
public:
    InputImp();
    virtual ~InputImp();

public:
    void Initialize();
    void Destroy();

    // Mouse
    float GetMouseMoveX();
    float GetMouseMoveY();
    void SetMouseMoveX(float move);
    void SetMouseMoveY(float move);

protected:
    float           m_MouseMoveX;
    float           m_MouseMoveY;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// InputImp DEFINITION
//----------------------------------------------------------------------------------
InputImp::InputImp()
: m_MouseMoveX(0.0f)
, m_MouseMoveY(0.0f) {
}

InputImp::~InputImp() {
}

void InputImp::Initialize() {
}

void InputImp::Destroy() {
}

float InputImp::GetMouseMoveX() {
    return m_MouseMoveX;
}

float InputImp::GetMouseMoveY() {
    return m_MouseMoveY;
}

void InputImp::SetMouseMoveX(float m) {
    m_MouseMoveX = m;
}

void InputImp::SetMouseMoveY(float m) {
    m_MouseMoveY = m;
}

//-----------------------------------------------------------------------------------
// Input DEFINITION
//----------------------------------------------------------------------------------
void Input::Initialize() {
    if (s_InputImp == NULL) {
        s_InputImp = new InputImp;
        if (s_InputImp != NULL) {
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void Input::Destroy() {
    if (s_InputImp != NULL) {
        s_InputImp->Destroy();
        GLB_SAFE_DELETE(s_InputImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

float Input::GetMouseMoveX() {
    float result = 0.0f;

    if (s_InputImp != NULL) {
        result = s_InputImp->GetMouseMoveX();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void Input::SetMouseMoveX(float m) {
    if (s_InputImp != NULL) {
        s_InputImp->SetMouseMoveX(m);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

float Input::GetMouseMoveY() {
    float result = 0.0f;

    if (s_InputImp != NULL) {
        result = s_InputImp->GetMouseMoveY();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void Input::SetMouseMoveY(float m) {
    if (s_InputImp != NULL) {
        s_InputImp->SetMouseMoveY(m);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}
};  // namespace glb