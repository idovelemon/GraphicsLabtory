//-------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 12
// Brief: Define the application to manage detail of win32
//-------------------------------------------------------------------
#include "glbapplication.h"

#include "glbinput.h"
#include "render/glbmaterial.h"
#include "render/glbmesh.h"
#include "render/glbrender.h"
#include "render/glbrenderdevice.h"
#include "render/glbshader.h"
#include "render/glbtexture.h"

#include "scene/glbmodel.h"
#include "scene/glbscene.h"

#include "util/glblog.h"
#include "util/glbmacro.h"

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
    virtual bool Initialize(APPLICATION_CREATOR creator, HINSTANCE hInstance, AppConfig config);
    virtual void Update();
    virtual void Destroy();

    HWND GetWindowHandle();
    HINSTANCE GetWindowInst();
    int32_t GetWindowWidth();
    int32_t GetWindowHeight();
    int32_t GetShadowMapWidth();
    int32_t GetShadowMapHeight();

protected:
    bool CreateWnd(HINSTANCE hInstance, HWND hWnd, int32_t width, int32_t height, const wchar_t* caption, int32_t icon);
    bool SetupGLBBeforeUserApp(int32_t width, int32_t height);
    bool SetupGLBAfterUserApp();

private:
    ApplicationBase*                m_Application;
    HWND                            m_WndHandle;
    HINSTANCE                       m_WndInstance;
    AppConfig                       m_Config;
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
, m_WndHandle(NULL)
, m_WndInstance(NULL)
, m_Config() {
}

ApplicationImp::~ApplicationImp() {
}

bool ApplicationImp::Initialize(APPLICATION_CREATOR creator, HINSTANCE hInstance, AppConfig config) {
    bool result = true;

    // Save config
    m_Config = config;

    // Create user application
    m_Application = creator();
    if (!m_Application) {
        result = false;
        GLB_SAFE_ASSERT(false);
        return result;
    }

    // Create window
    if (!CreateWnd(hInstance, config.wnd, config.screen_width, config.screen_height, config.caption, config.icon)) {
        result = false;
        GLB_SAFE_ASSERT(false);
        return result;
    }

    // Setup glb before user app
    if (!SetupGLBBeforeUserApp(config.screen_width, config.screen_height)) {
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

    // Setup glb aflter user app
    if (!SetupGLBAfterUserApp()) {
        result = false;
        GLB_SAFE_ASSERT(false);
        return result;
    }

    return result;
}

void ApplicationImp::Update() {
    if (!m_Config.wnd) {
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
    } else {
        if (m_Application) {
            m_Application->Update(1.0f);
        }
    }
}

void ApplicationImp::Destroy() {
    if (m_Application) {
        m_Application->Destroy();
        GLB_SAFE_DELETE(m_Application);
    }

    //EditorComm::Destroy();
    Input::Destroy();
    render::Render::Destroy();
    render::Device::Destroy();
    scene::ModelMgr::Destroy();
    render::mesh::Mgr::Destroy();
    render::material::Mgr::Destroy();
    render::texture::Mgr::Destroy();
    render::shader::Mgr::Destroy();
    scene::Scene::Destroy();
    util::log::Destroy();
}

HWND ApplicationImp::GetWindowHandle() {
    return m_WndHandle;
}

HINSTANCE ApplicationImp::GetWindowInst() {
    return m_WndInstance;
}

int32_t ApplicationImp::GetWindowWidth() {
    return m_Config.screen_width;
}

int32_t ApplicationImp::GetWindowHeight() {
    return m_Config.screen_height;
}

int32_t ApplicationImp::GetShadowMapWidth() {
    return m_Config.shadow_map_width;
}

int32_t ApplicationImp::GetShadowMapHeight() {
    return m_Config.shadow_map_height;
}

bool ApplicationImp::CreateWnd(HINSTANCE hInstance, HWND hWnd, int32_t width, int32_t height, const wchar_t* caption, int32_t icon) {
    if (!hWnd) {
        // Register window class
        WNDCLASSEX wnd;
        wnd.cbClsExtra = 0;
        wnd.cbSize = sizeof(wnd);
        wnd.cbWndExtra = NULL;
        wnd.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
        wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
        wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(icon));
        wnd.hIconSm = NULL;
        wnd.hInstance = hInstance;
        wnd.lpfnWndProc = GLB_WndProc;
        wnd.lpszClassName = L"GraphicsLab";
        wnd.lpszMenuName = NULL;
        wnd.style = CS_HREDRAW | CS_VREDRAW;
        int32_t err = GetLastError();

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
    } else {
        m_WndHandle = hWnd;
    }

    m_WndInstance = hInstance;

    return true;
}

bool ApplicationImp::SetupGLBBeforeUserApp(int32_t width, int32_t height) {
    util::log::Initialize();
    scene::Scene::Initialize();
    render::shader::Mgr::Initialize();
    render::texture::Mgr::Initialize();
    render::material::Mgr::Initialize();
    render::mesh::Mgr::Initialize();
    scene::ModelMgr::Initialize();
    render::Device::Initialize();
    render::Render::Initialize(width, height);
    Input::Initialize();
    //EditorComm::Initialize();
    //EditorComm::PostData();

    return true;
}

bool ApplicationImp::SetupGLBAfterUserApp() {
    render::Render::InitializeAfterUserApp();

    return true;
}

//-----------------------------------------------------------------------------------
// Application DEFINITION
//-----------------------------------------------------------------------------------
bool Application::Initialize(APPLICATION_CREATOR creator, HINSTANCE hInstance, AppConfig config) {
    bool result = true;
    if (s_ApplicationImp == NULL) {
        s_ApplicationImp = new ApplicationImp;
        if (s_ApplicationImp != NULL) {
            result = s_ApplicationImp->Initialize(creator, hInstance, config);
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

HINSTANCE Application::GetWindowInst() {
    HINSTANCE result = NULL;

    if (s_ApplicationImp != NULL) {
        result = s_ApplicationImp->GetWindowInst();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Application::GetWindowWidth() {
    int32_t result = 0;
    if (s_ApplicationImp != NULL) {
        result = s_ApplicationImp->GetWindowWidth();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Application::GetWindowHeight() {
    int32_t result = 0;
    if (s_ApplicationImp != NULL) {
        result = s_ApplicationImp->GetWindowHeight();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Application::GetShadowMapWidth() {
    int32_t result = 0;
    if (s_ApplicationImp != NULL) {
        result = s_ApplicationImp->GetShadowMapWidth();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Application::GetShadowMapHeight() {
    int32_t result = 0;
    if (s_ApplicationImp != NULL) {
        result = s_ApplicationImp->GetShadowMapHeight();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

};  // namespace app

};  // namespace glb