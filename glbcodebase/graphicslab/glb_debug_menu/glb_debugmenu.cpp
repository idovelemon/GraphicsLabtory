//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 11 / 13
// Brief: Demostrate debug menu system
//----------------------------------------------------------------------
#include "glb_debugmenu.h"
#include "resource.h"

#define BUILD_IN_RENDER 0

#if !BUILD_IN_RENDER

#include "debugmenu.h"
#include "font.h"

class ApplicationDebugMenu : public glb::app::ApplicationBase {
public:
    ApplicationDebugMenu()
    : m_FontMesh(nullptr)
    , m_FontShader(nullptr)
    , m_EnableDisplayFPS(false)
    , m_GameSpeed(0)
    , m_GameSize(0.0f) {
    }
    virtual~ApplicationDebugMenu() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationDebugMenu;
    }

public:
    bool Initialize() {
        debugmenu2::FontMgr::Initialize("res/font.bmp", "res/font.list");
        m_FontShader = render::shader::UserProgram::Create("res/font.vs", "res/font.fs");
        m_FontMesh = render::mesh::FontMesh::Create();

        debugmenu2::Mgr::Initialize();

        bool bEnableShader = false;
        bool bEnableTexture = false;
        debugmenu2::Mgr::AddMenuItem("Debug|Shader|Enable Shader", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Debug|Shader|Disable Shader", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Debug|Shader|Pass|Forward Light", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Debug|Shader|Pass|Shadow Map", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Debug|Shader|Pass|Decal Map", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Debug|Shader|Pass|HDR", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Debug|Shader|Pass|DOF", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Debug|Shader|Pass|MotionBlur", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Debug|Shader|Pass|Gamma", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Debug|Texture|Enable Texture", bEnableTexture);
        debugmenu2::Mgr::AddMenuItem("Debug|Texture|Disable Texture", bEnableTexture);
        debugmenu2::Mgr::AddMenuItem("System|Control|Start", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("System|Control|Pause", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("System|Control|Exit", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("System|Info|Display", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("System|Info|Export to Text", bEnableShader);
        debugmenu2::Mgr::AddMenuItem("Test|Display|FPS", m_EnableDisplayFPS);
        debugmenu2::Mgr::AddMenuItem("Test|Display|Speed", m_GameSpeed, 0, 100);
        debugmenu2::Mgr::AddMenuItem("Test|Display|Size", m_GameSize, 0.001f, 0.0f, 1.0f, "%.3f");

        // TEST
        //debugmenu2::Mgr::GetRootMenu()->SetOpened(true);
        //debugmenu2::Mgr::GetRootMenu()->FindMenuItem("System")->SetOpened(true);
        //debugmenu2::Mgr::GetRootMenu()->FindMenuItem("Info")->SetOpened(true);
        //debugmenu2::Mgr::GetRootMenu()->FindMenuItem("Display")->SetSelected(true);

        m_MenuShader = render::shader::UserProgram::Create("res/menu.vs", "res/menu.fs");
        m_MenuBackgroundMesh = render::mesh::DynamicTriangleMesh::Create(64);

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();

        static int32_t sPressDelta = 0;
        if (glb::Input::IsKeyboardButtonPressed(glb::BK_F1) && sPressDelta > 15) {
            debugmenu2::Mgr::GetRootMenu()->SetOpened(!debugmenu2::Mgr::GetRootMenu()->IsOpened());
            sPressDelta = 0;
        }
        sPressDelta++;

        debugmenu2::Mgr::Update();

        render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
        render::Device::SetClearDepth(1.0f);
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        DrawMenu();

        // Reset render target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // SwapBuffers
        render::Device::SwapBuffer();

        GLB_CHECK_GL_ERROR;

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
    }

    void DrawMenu() {
        m_FontMesh->ClearAllChars();
        m_MenuBackgroundMesh->Clear();

        AddBackground();
        AddOpenedBackground();
        AddActivedBackground();
        DrawBackground();
        DrawMenuText();
        if (m_EnableDisplayFPS) {
            AddText("FPS:60fps", math::Vector(-0.95f, -0.95f, 0.0f), math::Vector(1.0f, 1.0f, 1.0f), math::Vector(0.0f, 1.0f, 0.0f));
        }
        DrawChar();
    }

    void AddBackground() {
        float xOffset = debugmenu2::ScreenPosToNDCPosWidth(debugmenu2::kMenuStartXPos);
        float yOffset = debugmenu2::ScreenPosToNDCPosHeight(debugmenu2::kMenuStartYPos);
        debugmenu2::DebugMenuItem* menuItem = debugmenu2::Mgr::GetRootMenu();

        while (menuItem && menuItem->IsOpened()) {
            float sizeX = 0.0f, sizeY = 0.0f;
            menuItem->CalculateChildrenMenuSize(sizeX, sizeY);

            m_MenuBackgroundMesh->AddRect(math::Vector(xOffset, yOffset, 0.0f), math::Vector(xOffset + sizeX, yOffset - sizeY, 0.0f), math::Vector(0.0f, 0.0f, 0.0f, 1.0f));
            xOffset = xOffset + sizeX + debugmenu2::ScreenSizeToNDCSizeWidth(debugmenu2::kPaddingXBetweenMenu);

            menuItem = menuItem->GetChildrenOpenedMenuItem();
        }
    }

    void AddOpenedBackground() {
        float xOffset = debugmenu2::ScreenPosToNDCPosWidth(debugmenu2::kMenuStartXPos);
        float yOffset = debugmenu2::ScreenPosToNDCPosHeight(debugmenu2::kMenuStartYPos);
        debugmenu2::DebugMenuItem* menuItem = debugmenu2::Mgr::GetRootMenu();

        while (menuItem && menuItem->IsOpened()) {
            float sizeX = 0.0f, sizeY = 0.0f;
            menuItem->CalculateChildrenMenuSize(sizeX, sizeY);

            std::vector<debugmenu2::DebugMenuItem*>&& childrenItems = menuItem->GetChildrenMenuItems();
            for (int32_t i = 0; i < childrenItems.size(); i++) {
                if (childrenItems[i] && childrenItems[i]->IsOpened() && !childrenItems[i]->IsSelected()) {
                    m_MenuBackgroundMesh->AddRect(math::Vector(xOffset, yOffset, 0.0f), math::Vector(xOffset + sizeX, yOffset - childrenItems[i]->GetSizeY(), 0.0f), math::Vector(0.4f, 0.4f, 0.4f, 0.25f));
                }

                yOffset = yOffset - debugmenu2::ScreenSizeToNDCSizeHeight(debugmenu2::kMenuItemSizeY);
            }

            menuItem = menuItem->GetChildrenOpenedMenuItem();

            xOffset = xOffset + sizeX + debugmenu2::ScreenSizeToNDCSizeWidth(debugmenu2::kPaddingXBetweenMenu);
            yOffset = debugmenu2::ScreenPosToNDCPosHeight(debugmenu2::kMenuStartYPos);
        }
    }

    void AddActivedBackground() {
        float xOffset = debugmenu2::ScreenPosToNDCPosWidth(debugmenu2::kMenuStartXPos);
        float yOffset = debugmenu2::ScreenPosToNDCPosHeight(debugmenu2::kMenuStartYPos);
        debugmenu2::DebugMenuItem* menuItem = debugmenu2::Mgr::GetRootMenu();

        while (menuItem && menuItem->IsOpened()) {
            float sizeX = 0.0f, sizeY = 0.0f;
            menuItem->CalculateChildrenMenuSize(sizeX, sizeY);

            std::vector<debugmenu2::DebugMenuItem*>&& childrenItems = menuItem->GetChildrenMenuItems();
            for (int32_t i = 0; i < childrenItems.size(); i++) {
                if (childrenItems[i] && childrenItems[i]->IsSelected()) {
                    m_MenuBackgroundMesh->AddRect(math::Vector(xOffset, yOffset, 0.0f), math::Vector(xOffset + sizeX, yOffset - childrenItems[i]->GetSizeY(), 0.0f), math::Vector(0.4f, 0.4f, 0.4f, 0.8f));
                    break;
                }

                yOffset = yOffset - debugmenu2::ScreenSizeToNDCSizeHeight(debugmenu2::kMenuItemSizeY);
            }

            menuItem = menuItem->GetChildrenOpenedMenuItem();

            xOffset = xOffset + sizeX + debugmenu2::ScreenSizeToNDCSizeWidth(debugmenu2::kPaddingXBetweenMenu);
            yOffset = debugmenu2::ScreenPosToNDCPosHeight(debugmenu2::kMenuStartYPos);
        }
    }

    void DrawBackground() {
        // Setup Render State
        render::Device::SetCullFaceEnable(false);
        render::Device::SetDepthTestEnable(false);
        render::Device::SetAlphaBlendEnable(true);
        render::Device::SetAlphaBlendFunc(render::FACTOR_SRC, render::FUNC_SRC_ALPHA);
        render::Device::SetAlphaBlendFunc(render::FACTOR_DST, render::FUNC_ONE_MINUS_SRC_ALPHA);

        // Setup Shader
        render::Device::SetShader(m_MenuShader);
        render::Device::SetShaderLayout(m_MenuShader->GetShaderLayout());

        // Setup Vertex
        render::Device::SetVertexBuffer(m_MenuBackgroundMesh->GetVertexBuffer());
        render::Device::SetVertexLayout(m_MenuBackgroundMesh->GetVertexLayout());

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, m_MenuBackgroundMesh->GetVertexNum());
    }

    void DrawMenuText() {
        float xOffset = debugmenu2::ScreenPosToNDCPosWidth(debugmenu2::kMenuStartXPos);
        float yOffset = debugmenu2::ScreenPosToNDCPosHeight(debugmenu2::kMenuStartYPos);
        debugmenu2::DebugMenuItem* menuItem = debugmenu2::Mgr::GetRootMenu();

        while (menuItem && menuItem->IsOpened()) {
            float sizeX = 0.0f, sizeY = 0.0f;
            menuItem->CalculateChildrenMenuSize(sizeX, sizeY);

            std::vector<debugmenu2::DebugMenuItem*>&& childItems = menuItem->GetChildrenMenuItems();
            for (int32_t i = 0; i < childItems.size(); i++) {
                AddText(childItems[i]->GetDisplayText(), math::Vector(xOffset + debugmenu2::ScreenSizeToNDCSizeWidth(debugmenu2::kMenuTextXOffset), yOffset - debugmenu2::ScreenSizeToNDCSizeHeight(debugmenu2::kMenuTextYOffset), 0.0f), math::Vector(1.0f, 1.0f, 1.0f), math::Vector(213.0f / 256.0f, 211.0f / 256.0f, 214.0f / 256.0f, 1.0f));
                if (childItems[i]->IsOpened()) {
                    AddChar('>', math::Vector(xOffset + sizeX - debugmenu2::ScreenSizeToNDCSizeWidth(debugmenu2::kMenuOpenCharacterSizeX), yOffset - debugmenu2::ScreenSizeToNDCSizeHeight(debugmenu2::kMenuTextYOffset), 0.0f), math::Vector(1.0f, 1.0f, 1.0f), math::Vector(213.0f / 256.0f, 211.0f / 256.0f, 214.0f / 256.0f, 1.0f));
                }

                yOffset = yOffset - childItems[i]->GetSizeY();
            }

            menuItem = menuItem->GetChildrenOpenedMenuItem();
            xOffset = xOffset + sizeX + debugmenu2::ScreenSizeToNDCSizeWidth(debugmenu2::kPaddingXBetweenMenu);
            yOffset = debugmenu2::ScreenPosToNDCPosHeight(debugmenu2::kMenuStartYPos);
        }
    }

    void AddChar(const char c, math::Vector pos, math::Vector size, math::Vector color) {
        float ltU = 0, ltV = 0, rbU = 0, rbV = 0;
        float ndcSizeX = 0.0f, ndcSizeY = 0.0f;
        debugmenu2::FontMgr::GetCharacter(c, ltU, ltV, rbU, rbV, ndcSizeX, ndcSizeY);

        m_FontMesh->AddChar(math::Vector(ltU, ltV, 0.0f), math::Vector(rbU, rbV, 0.0f), pos, size * math::Vector(ndcSizeX, ndcSizeY, 0.0f), color);
    }

    void AddText(const char* text, math::Vector pos, math::Vector size, math::Vector color) {
        math::Vector preCharNDCSize(0.0f, 0.0f, 0.0f);
        math::Vector preCharPos = pos;

        for (int32_t i = 0; i < strlen(text); i++) {
            float ltU = 0, ltV = 0, rbU = 0, rbV = 0;
            float ndcSizeX = 0.0f, ndcSizeY = 0.0f;
            debugmenu2::FontMgr::GetCharacter(text[i], ltU, ltV, rbU, rbV, ndcSizeX, ndcSizeY);

            math::Vector newPos = preCharPos + math::Vector(preCharNDCSize.x, 0.0f, 0.0f);
            if (text[i] != ' ') {
                m_FontMesh->AddChar(math::Vector(ltU, ltV, 0.0f), math::Vector(rbU, rbV, 0.0f), newPos, size * math::Vector(ndcSizeX, ndcSizeY, 0.0f), color);
            }

            preCharNDCSize = size * math::Vector(ndcSizeX, ndcSizeY, 0.0f);
            preCharPos = newPos;
        }
    }

    void DrawChar() {
        // Clear
        render::Device::SetCullFaceEnable(false);
        render::Device::SetDepthTestEnable(false);
        render::Device::SetAlphaBlendEnable(true);
        render::Device::SetAlphaBlendFunc(render::FACTOR_SRC, render::FUNC_SRC_ALPHA);
        render::Device::SetAlphaBlendFunc(render::FACTOR_DST, render::FUNC_ONE_MINUS_SRC_ALPHA);

        // Setup Shader
        render::Device::SetShader(m_FontShader);
        render::Device::SetShaderLayout(m_FontShader->GetShaderLayout());

        // Setup Vertex
        render::Device::SetVertexBuffer(m_FontMesh->GetVertexBuffer());
        render::Device::SetVertexLayout(m_FontMesh->GetVertexLayout());

        // Setup Texture
        render::Device::SetTexture(0, render::texture::Mgr::GetTextureById(debugmenu2::FontMgr::GetFontTexture()), 0);

        // Setup Uniform
        render::Device::SetUniformSampler2D(m_FontShader->GetUniformLocation("glb_FontMap"), 0);

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, m_FontMesh->GetVertexNum());
    }

protected:
    render::mesh::FontMesh*             m_FontMesh;
    render::shader::UserProgram*        m_FontShader;

    render::mesh::DynamicTriangleMesh*  m_MenuBackgroundMesh;
    render::shader::UserProgram*        m_MenuShader;

    bool                                m_EnableDisplayFPS;
    int32_t                             m_GameSpeed;
    float                               m_GameSize;
};

#else

class ApplicationDebugMenu : public glb::app::ApplicationBase {
public:
    ApplicationDebugMenu()
    : m_EnableDisplayFPS(false)
    , m_GameSpeed(0)
    , m_GameSize(0.0f) {}
    virtual~ApplicationDebugMenu() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationDebugMenu;
    }

public:
    bool Initialize() {
        // Camera
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 100.0f, 300.0f), math::Vector(0.0f, 0.0f, 0.0));
        scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        scene::Light ambient(scene::AMBIENT_LIGHT);
        ambient.color = math::Vector(0.1f, 0.1f, 0.1f);
        scene::Scene::SetLight(ambient, 0);

        scene::Light light(scene::PARALLEL_LIGHT);
        light.color = math::Vector(1.2f, 1.2f, 1.2f);
        light.dir = math::Vector(-1.0f, -1.0f, 1.0f);
        light.dir.Normalize();
        scene::Scene::SetLight(light, 1);

        // Perspective
        render::Render::SetPerspective(render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        render::Render::SetHighLightBase(1.5f);

        bool bEnableShader = false;
        bool bEnableTexture = false;
        debugmenu::Mgr::AddMenuItem("Debug|Shader|Enable Shader", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Debug|Shader|Disable Shader", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Debug|Shader|Pass|Forward Light", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Debug|Shader|Pass|Shadow Map", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Debug|Shader|Pass|Decal Map", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Debug|Shader|Pass|HDR", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Debug|Shader|Pass|DOF", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Debug|Shader|Pass|MotionBlur", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Debug|Shader|Pass|Gamma", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Debug|Texture|Enable Texture", bEnableTexture);
        debugmenu::Mgr::AddMenuItem("Debug|Texture|Disable Texture", bEnableTexture);
        debugmenu::Mgr::AddMenuItem("System|Control|Start", bEnableShader);
        debugmenu::Mgr::AddMenuItem("System|Control|Pause", bEnableShader);
        debugmenu::Mgr::AddMenuItem("System|Control|Exit", bEnableShader);
        debugmenu::Mgr::AddMenuItem("System|Info|Display", bEnableShader);
        debugmenu::Mgr::AddMenuItem("System|Info|Export to Text", bEnableShader);
        debugmenu::Mgr::AddMenuItem("Test|Display|FPS", m_EnableDisplayFPS);
        debugmenu::Mgr::AddMenuItem("Test|Display|Speed", m_GameSpeed, 0, 100);
        debugmenu::Mgr::AddMenuItem("Test|Display|Size", m_GameSize, 0.001f, 0.0f, 1.0f, "%.3f");

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        Input::Update();
        debugmenu::Mgr::Update();

        // Draw scene
        render::Render::Draw();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
    }

protected:
    bool                                m_EnableDisplayFPS;
    int32_t                             m_GameSpeed;
    float                               m_GameSize;
};

#endif

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_debugmenu", sizeof(L"glb_debugmenu"));
    config.screen_width = 1024;
    config.screen_height = 720;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationDebugMenu::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}