//-------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 12
// Brief: Define the application to manage detail of win32
//-------------------------------------------------------------------
#include "glbapplication.h"

#include "glbeditorcomm.h"
#include "glblog.h"
#include "glbmacro.h"
#include "glbmaterial.h"
#include "glbmesh.h"
#include "glbmodel.h"
#include "glbrender.h"
#include "glbrenderdevice.h"
#include "glbscene.h"
#include "glbshader.h"
#include "glbtexture.h"

namespace glb {

namespace app {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class ApplicationImp;
static ApplicationImp* s_ApplicationImp = NULL;

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// ApplicationImp DECLARATION
//----------------------------------------------------------------------------------
class ApplicationImp {
public:
    ApplicationImp();
    virtual~ApplicationImp();

public:
    virtual bool Initialize(
        APPLICATION_CREATOR creator
        , HINSTANCE hInstance
        , int32_t width, int32_t height
        , const wchar_t* caption);
    virtual void Update();
    virtual void Destroy();

    HWND GetWindowHandle();

protected:
    bool CreateWnd(HINSTANCE hInstance, int32_t width, int32_t height, const wchar_t* caption);
    bool SetupGLB(int32_t width, int32_t height);

private:
    ApplicationBase*                m_Application;
    HWND                            m_WndHandle;
};

//----------------------------------------------------------------------------------
// HELP METHOD
//----------------------------------------------------------------------------------
LRESULT CALLBACK GLB_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// ApplicationImp DEFINITION
//-----------------------------------------------------------------------------------
ApplicationImp::ApplicationImp()
: m_Application(NULL)
, m_WndHandle(NULL) {
}

ApplicationImp::~ApplicationImp() {
}

bool ApplicationImp::Initialize(
        APPLICATION_CREATOR creator
        , HINSTANCE hInstance
        , int32_t width, int32_t height
        , const wchar_t* caption) {
    bool result = true;

    // Create user application
    m_Application = creator();
    if (!m_Application) {
        result = false;
        GLB_SAFE_ASSERT(false);
        return result;
    }

    // Create window
    if (!CreateWnd(hInstance, width, height, caption)) {
        result = false;
        GLB_SAFE_ASSERT(false);
        return result;
    }

    // Setup glb
    if (!SetupGLB(width, height)) {
        result = false;
        GLB_SAFE_ASSERT(false);
        return result;
    }

    // Initialize user application
    if (!m_Application->Initialize()) {
        result = false;
        GLB_SAFE_ASSERT(false);
        return result;
    }

    return result;
}

void ApplicationImp::Update() {
    MSG msg = {0};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            if (m_Application) {
                m_Application->Update(1.0f);
            }
        }
    }
}

void ApplicationImp::Destroy() {
    if (m_Application) {
        m_Application->Destroy();
        GLB_SAFE_DELETE(m_Application);
    }

    //EditorComm::Destroy();
    render::Render::Destroy();
    render::Device::Destroy();
    ModelMgr::Destroy();
    mesh::Mgr::Destroy();
    material::Mgr::Destroy();
    texture::Mgr::Destroy();
    shader::Mgr::Destroy();
    scene::Scene::Destroy();
    log::Destroy();
}

HWND ApplicationImp::GetWindowHandle() {
    return m_WndHandle;
}

bool ApplicationImp::CreateWnd(HINSTANCE hInstance, int32_t width, int32_t height, const wchar_t* caption) {
    // Register window class
    WNDCLASSEX wnd;
    wnd.cbClsExtra = 0;
    wnd.cbSize = sizeof(wnd);
    wnd.cbWndExtra = NULL;
    wnd.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
    wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd.hIcon = NULL;
    wnd.hIconSm = NULL;
    wnd.hInstance = hInstance;
    wnd.lpfnWndProc = GLB_WndProc;
    wnd.lpszClassName = L"GraphicsLab";
    wnd.lpszMenuName = NULL;
    wnd.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&wnd)) {
        return false;
    }

    // Create window
    RECT client_rect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, FALSE);
    m_WndHandle = CreateWindow(L"GraphicsLab", caption, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT
        , client_rect.right - client_rect.left, client_rect.bottom - client_rect.top, NULL, NULL, hInstance, NULL);
    if (m_WndHandle == NULL) {
        return false;
    }

    ShowWindow(m_WndHandle, SW_SHOW);

    return true;
}

bool ApplicationImp::SetupGLB(int32_t width, int32_t height) {
    log::Initialize();
    scene::Scene::Initialize();
    shader::Mgr::Initialize();
    texture::Mgr::Initialize();
    material::Mgr::Initialize();
    mesh::Mgr::Initialize();
    ModelMgr::Initialize();
    render::Device::Initialize();
    render::Render::Initialize(width, height);
    //EditorComm::Initialize();
    //EditorComm::PostData();

    return true;
}

//-----------------------------------------------------------------------------------
// Application DEFINITION
//-----------------------------------------------------------------------------------
bool Application::Initialize(
        APPLICATION_CREATOR creator
        , HINSTANCE hInstance
        , int32_t width, int32_t height
        , const wchar_t* caption) {
    bool result = true;
    if (s_ApplicationImp == NULL) {
        s_ApplicationImp = new ApplicationImp;
        if (s_ApplicationImp != NULL) {
            result = s_ApplicationImp->Initialize(creator, hInstance, width, height, caption);
        } else {
            GLB_SAFE_ASSERT(false);
            result = false;
        }
    } else {
        GLB_SAFE_ASSERT(false);
        result = false;
    }

    return result;
}

void Application::Update() {
    if (s_ApplicationImp != NULL) {
        s_ApplicationImp->Update();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Application::Destroy() {
    if (s_ApplicationImp != NULL) {
        s_ApplicationImp->Destroy();
        GLB_SAFE_DELETE(s_ApplicationImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

HWND Application::GetWindowHandle() {
    HWND result = NULL;
    if (s_ApplicationImp != NULL) {
        result = s_ApplicationImp->GetWindowHandle();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}
};  // namespace app

};  // namespace glb