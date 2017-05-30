//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 04 / 04
// Brief: Input mananger, include keyboard and mouse
//------------------------------------------------------------------
#include "glbinput.h"

#include <dinput.h>

#include "glbapplication.h"
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
    void Update();
    void Destroy();

    int64_t GetMouseMoveX();
    int64_t GetMouseMoveY();
    int64_t GetMousePosX();
    int64_t GetMousePosY();
    bool IsMouseButtonPressed(ButtonMouse mouse);
    bool IsKeyboardButtonPressed(ButtonKey key);

protected:
    LPDIRECTINPUT8          m_Input;
    LPDIRECTINPUTDEVICE8    m_Mouse;
    LPDIRECTINPUTDEVICE8    m_Keyboard;
    DIMOUSESTATE            m_MouseState;
    int8_t                  m_KeyboardState[256];
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// InputImp DEFINITION
//----------------------------------------------------------------------------------
InputImp::InputImp()
: m_Input(NULL)
, m_Mouse(NULL)
, m_Keyboard(NULL)
, m_MouseState() {
    memset(m_KeyboardState, 0, sizeof(m_KeyboardState));
}

InputImp::~InputImp() {
}

void InputImp::Initialize() {
    if (FAILED(DirectInput8Create(app::Application::GetWindowInst(), 0x0800, IID_IDirectInput8,
        reinterpret_cast<void**>(&m_Input), NULL))) {
        GLB_SAFE_ASSERT(false);
        return;
    }

    if (FAILED(m_Input->CreateDevice(GUID_SysMouse, &m_Mouse, NULL))) {
        GLB_SAFE_ASSERT(false);
        return;
    }

    if (FAILED(m_Input->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL))) {
        GLB_SAFE_ASSERT(false);
        return;
    }

    m_Mouse->SetDataFormat(&c_dfDIMouse);
    m_Keyboard->SetDataFormat(&c_dfDIKeyboard);

    m_Mouse->SetCooperativeLevel(app::Application::GetWindowHandle(), DISCL_BACKGROUND |DISCL_NONEXCLUSIVE);
    m_Keyboard->SetCooperativeLevel(app::Application::GetWindowHandle(), DISCL_BACKGROUND |DISCL_NONEXCLUSIVE);

    if (FAILED(m_Mouse->Acquire())) {
        GLB_SAFE_ASSERT(false);
        return;
    }

    if (FAILED(m_Keyboard->Acquire())) {
        GLB_SAFE_ASSERT(false);
        return;
    }
}

void InputImp::Update() {
    if (m_Mouse != NULL) {
        if (FAILED(m_Mouse->Acquire())) {
            GLB_SAFE_ASSERT(false);
            return;
        }

        if (FAILED(m_Mouse->Poll())) {
            GLB_SAFE_ASSERT(false);
            return;
        }

        if (FAILED(m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState))) {
            GLB_SAFE_ASSERT(false);
            return;
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    if (m_Keyboard != NULL) {
        if (FAILED(m_Keyboard->Acquire())) {
            GLB_SAFE_ASSERT(false);
            return;
        }

        if (FAILED(m_Keyboard->Poll())) {
            GLB_SAFE_ASSERT(false);
            return;
        }

        if (FAILED(m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), &m_KeyboardState))) {
            GLB_SAFE_ASSERT(false);
            return;
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void InputImp::Destroy() {
    if (m_Mouse != NULL) {
        m_Mouse->Unacquire();
        m_Mouse->Release();
        m_Mouse = NULL;
    }

    if (m_Keyboard != NULL) {
        m_Keyboard->Unacquire();
        m_Keyboard->Release();
        m_Keyboard = NULL;
    }

    if (m_Input != NULL) {
        m_Input->Release();
        m_Input = NULL;
    }
}

int64_t InputImp::GetMouseMoveX() {
    return m_MouseState.lX;
}

int64_t InputImp::GetMouseMoveY() {
    return m_MouseState.lY;
}

int64_t InputImp::GetMousePosX() {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(app::Application::GetWindowHandle(), &point);

    return point.x;
}

int64_t InputImp::GetMousePosY() {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(app::Application::GetWindowHandle(), &point);

    return point.y;
}

bool InputImp::IsMouseButtonPressed(ButtonMouse mouse) {
    bool result = false;

    if (m_MouseState.rgbButtons[mouse] & 0x80) {
        result = true;
    }

    return result;
}

bool InputImp::IsKeyboardButtonPressed(ButtonKey key) {
    bool result = false;

    if (m_KeyboardState[key] & 0x80) {
        result = true;
    }

    return result;
}

//-----------------------------------------------------------------------------------
// Input DEFINITION
//----------------------------------------------------------------------------------
void Input::Initialize() {
    if (s_InputImp == NULL) {
        s_InputImp = new InputImp;
        if (s_InputImp != NULL) {
            s_InputImp->Initialize();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void Input::Update() {
    if (s_InputImp != NULL) {
        s_InputImp->Update();
    } else {
        GLB_SAFE_ASSERT(false);
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

int64_t Input::GetMouseMoveX() {
    int64_t result = 0;

    if (s_InputImp != NULL) {
        result = s_InputImp->GetMouseMoveX();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int64_t Input::GetMouseMoveY() {
    int64_t result = 0;

    if (s_InputImp != NULL) {
        result = s_InputImp->GetMouseMoveY();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int64_t Input::GetMousePosX() {
    int64_t result = 0;

    if (s_InputImp != NULL) {
        result = s_InputImp->GetMousePosX();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int64_t Input::GetMousePosY() {
    int64_t result = 0;

    if (s_InputImp != NULL) {
        result = s_InputImp->GetMousePosY();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

bool Input::IsMouseButtonPressed(ButtonMouse mouse) {
    bool result = false;

    if (s_InputImp != NULL) {
        result = s_InputImp->IsMouseButtonPressed(mouse);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

bool Input::IsKeyboardButtonPressed(ButtonKey key) {
    bool result = false;

    if (s_InputImp != NULL) {
        result = s_InputImp->IsKeyboardButtonPressed(key);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}
};  // namespace glb