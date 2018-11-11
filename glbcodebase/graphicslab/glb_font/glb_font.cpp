//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 11 / 10
// Brief: Demostrate how to rendering font
//----------------------------------------------------------------------
#include "glb_font.h"
#include "resource.h"

#include <fstream>

#define BUILD_IN_RENDER 0

#if BUILD_IN_RENDER
//--------------------------------------------------------------------------------------------

class FontMgrImp {
public:
    FontMgrImp();
    virtual~ FontMgrImp();

    void Initialize(const char* fontImage, const char* fontList);
    void Update();
    void Destroy();

    void GetCharacter(const char c, float& ltX, float& ltY, float& rbX, float& rbY, float& ndcSizeX, float& ndcSizeY);
    int32_t GetFontTexture();

protected:
    struct FontEntry {
        char c;
        int32_t ltX;
        int32_t ltY;
        int32_t rbX;
        int32_t rbY;

        FontEntry()
        : c(' ')
        , ltX(0)
        , ltY(0)
        , rbX(0)
        , rbY(0) {
        }
    };

    int32_t m_FontTexture;
    std::map<const char, FontEntry> m_FontTbl;
};

FontMgrImp::FontMgrImp()
: m_FontTexture(-1) {
    m_FontTbl.clear();
}

FontMgrImp::~FontMgrImp() {
    m_FontTexture = -1;
    m_FontTbl.clear();
}

void FontMgrImp::Initialize(const char* fontImage, const char* fontList) {
    m_FontTexture = glb::render::texture::Mgr::LoadTexture(fontImage);

    std::ifstream input(fontList);

    if (!input.fail()) {
        while (!input.eof()) {
            char c = ' ';
            int32_t ltX = 0, ltY = 0, rbX = 0, rbY = 0;
            input >> c >> ltX >> ltY >> rbX >> rbY;

            FontEntry entry;
            entry.c = c;
            entry.ltX = ltX;
            entry.ltY = ltY;
            entry.rbX = rbX;
            entry.rbY = rbY;

            m_FontTbl[c] = entry;
        }
    }
}

void FontMgrImp::Update() {
}

void FontMgrImp::Destroy() {
}

void FontMgrImp::GetCharacter(const char c, float& ltU, float& ltV, float& rbU, float& rbV, float& ndcSizeX, float& ndcSizeY) {
    render::texture::Texture* fontTex = render::texture::Mgr::GetTextureById(m_FontTexture);
    int32_t width = fontTex->GetWidth();
    int32_t height = fontTex->GetHeight();
    int32_t screenWidth = app::Application::GetWindowWidth();
    int32_t screenHeight = app::Application::GetWindowHeight();

    char realC = c;
    if (c == ' ') {
        realC = ',';  // If come with space, using ','
    }

    int32_t ltX = m_FontTbl[realC].ltX;
    int32_t ltY = m_FontTbl[realC].ltY;
    int32_t rbX = m_FontTbl[realC].rbX;
    int32_t rbY = m_FontTbl[realC].rbY;

    // Calculate character original size in NDC
    ndcSizeX = ((rbX - ltX) * 1.0f / screenWidth) * 2.0f;
    ndcSizeY = ((rbY - ltY) * 1.0f / screenHeight) * 2.0f;

    // Inverse Y coordinate
    ltY = height - ltY;
    rbY = height - rbY;

    ltU = ltX * 1.0f / width;
    ltV = ltY * 1.0f / height;
    rbU = rbX * 1.0f / width;
    rbV = rbY * 1.0f / height;
}

int32_t FontMgrImp::GetFontTexture() {
    return m_FontTexture;
}

//--------------------------------------------------------------------------------------------

class FontMgr {
public:
    static void Initialize(const char* fontImage, const char* fontList);
    static void Update();
    static void Destroy();

    static void GetCharacter(const char c, float& ltX, float& ltY, float& rbX, float& rbY, float& ndcSizeX, float& ndcSizeY);
    static int32_t GetFontTexture();

protected:
    static FontMgrImp* s_FontMgrImp;
};

FontMgrImp* FontMgr::s_FontMgrImp = nullptr;

void FontMgr::Initialize(const char* fontImage, const char* fontList) {
    if (s_FontMgrImp == NULL) {
        s_FontMgrImp = new FontMgrImp;
        if (s_FontMgrImp != NULL) {
            s_FontMgrImp->Initialize(fontImage, fontList);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void FontMgr::Update() {
    if (s_FontMgrImp != NULL) {
        s_FontMgrImp->Update();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void FontMgr::Destroy() {
    if (s_FontMgrImp != NULL) {
        s_FontMgrImp->Destroy();
        GLB_SAFE_DELETE(s_FontMgrImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void FontMgr::GetCharacter(const char c, float& ltX, float& ltY, float& rbX, float& rbY, float& ndcSizeX, float& ndcSizeY) {
    if (s_FontMgrImp != NULL) {
        s_FontMgrImp->GetCharacter(c, ltX, ltY, rbX, rbY, ndcSizeX, ndcSizeY);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t FontMgr::GetFontTexture() {
    int32_t result = -1;

    if (s_FontMgrImp != nullptr) {
        result = s_FontMgrImp->GetFontTexture();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

//--------------------------------------------------------------------------------------------

class ApplicationFont : public glb::app::ApplicationBase {
public:
    ApplicationFont()
    : m_FontMesh(nullptr)
    , m_FontShader(nullptr) {
    }
    virtual~ApplicationFont() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationFont;
    }

public:
    bool Initialize() {
        FontMgr::Initialize("res/font.bmp", "res/font.list");

        m_FontShader = render::shader::UserProgram::Create("res/font.vs", "res/font.fs");

        m_FontMesh = render::mesh::FontMesh::Create();

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();

        render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
        render::Device::SetClearDepth(1.0f);
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        m_FontMesh->ClearAllChars();
        AddText("Hello Font!", math::Vector(-1.0f, 0.0f, 0.0f), math::Vector(1.0f, 1.0f, 0.0f), math::Vector(1.0f, 1.0f, 1.0f));
        DrawChar();

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

    void AddChar(const char c, math::Vector pos, math::Vector size, math::Vector color) {
        float ltU = 0, ltV = 0, rbU = 0, rbV = 0;
        float ndcSizeX = 0.0f, ndcSizeY = 0.0f;
        FontMgr::GetCharacter(c, ltU, ltV, rbU, rbV, ndcSizeX, ndcSizeY);

        m_FontMesh->AddChar(math::Vector(ltU, ltV, 0.0f), math::Vector(rbU, rbV, 0.0f), pos, size * math::Vector(ndcSizeX, ndcSizeY, 0.0f), color);
    }

    void AddText(const char* text, math::Vector pos, math::Vector size, math::Vector color) {
        math::Vector preCharNDCSize(0.0f, 0.0f, 0.0f);
        math::Vector preCharPos = pos;

        for (int32_t i = 0; i < strlen(text); i++) {
            float ltU = 0, ltV = 0, rbU = 0, rbV = 0;
            float ndcSizeX = 0.0f, ndcSizeY = 0.0f;
            FontMgr::GetCharacter(text[i], ltU, ltV, rbU, rbV, ndcSizeX, ndcSizeY);

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
        render::Device::SetDepthTestEnable(true);
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
        render::Device::SetTexture(0, render::texture::Mgr::GetTextureById(FontMgr::GetFontTexture()), 0);

        // Setup Uniform
        render::Device::SetUniformSampler2D(m_FontShader->GetUniformLocation("glb_FontMap"), 0);

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, m_FontMesh->GetVertexNum());
    }

protected:
    render::mesh::FontMesh*             m_FontMesh;
    render::shader::UserProgram*        m_FontShader;
};

#else

class ApplicationFont : public glb::app::ApplicationBase {
public:
    ApplicationFont() {}
    virtual~ApplicationFont() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationFont;
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
        render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        glb::render::Render::SetHighLightBase(1.5f);

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();

        render::Render::AddText("Hello Font!", math::Vector(-1.0f, 0.0f, 0.0f), math::Vector(1.0f, 1.0f, 1.0f), 1.0f);

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
    }
};

#endif

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_font", sizeof(L"glb_font"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationFont::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}