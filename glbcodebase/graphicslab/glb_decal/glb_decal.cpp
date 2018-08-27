//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 08 / 21
// Brief: Demostrate decal system
//----------------------------------------------------------------------
#include "glb_decal.h"
#include "resource.h"

class ApplicationDecal : public glb::app::ApplicationBase {
public:
    ApplicationDecal()
    : m_ColorProgram(NULL)
    , m_Camera(NULL)
    , m_Proj()
    , m_View() {
    }
    virtual~ApplicationDecal() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationDecal;
    }

public:
    bool Initialize() {
        // Create RenderTarget
        m_DecalRenderTarget = render::RenderTarget::Create(2048, 2048);
        m_DecalMap = render::texture::Texture::CreateFloat32Texture(2048, 2048);
        m_DecalRenderTarget->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT0, m_DecalMap);

        // Create Shader
        m_ColorProgram = render::shader::UserProgram::Create("res/color.vs", "res/color.fs");
        m_ColorShaderWVPLoc = m_ColorProgram->GetUniformLocation("glb_WVP");
        m_ColorShaderDecalWVPLoc = m_ColorProgram->GetUniformLocation("glb_DecalWVP");
        m_ColorShaderAlbedoLoc = m_ColorProgram->GetUniformLocation("glb_AlbedoTex");
        m_ColorShaderDecalMapLoc = m_ColorProgram->GetUniformLocation("glb_DecalTex");

        m_DecalProgram = render::shader::UserProgram::Create("res/decal.vs", "res/decal.fs");
        m_DecalShaderAlbedoLoc = m_DecalProgram->GetUniformLocation("glb_DecalTex");
        m_DecalShaderWVPLoc = m_DecalProgram->GetUniformLocation("glb_WVP");

        // Create Floor
        m_Floor = scene::Model::Create("res/floor.obj");

        // Create Decal
        m_Decal = scene::Model::Create("res/decal.obj");

        // Create Texture
        m_FloorAlbedoMap = render::texture::Texture::Create("res/Material_Base_Color.bmp");

        // Change Texture parameters
        glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GLuint>(m_DecalMap->GetNativeTex()));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // Create Camera
        m_Camera = scene::ModelCamera::Create(math::Vector(0.0f, 10.0f, 10.0f), math::Vector(0.0f, 0.0f, 0.0f));

        m_Proj.MakeProjectionMatrix(1.0f * app::Application::GetWindowWidth() / app::Application::GetWindowHeight(), 60.0f, 0.01f, 1000.0f);
        m_View = m_Camera->GetViewMatrix();

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();

        // Setup render target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // Clear
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
        render::Device::SetClearDepth(1.0f);
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        UpdateDecalPos();

        DrawDecal();

        m_DecalMap->GenerateMipmap();

        DrawFloor();

        // Reset render target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // SwapBuffers
        render::Device::SwapBuffer();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
        GLB_SAFE_DELETE(m_ColorProgram);
        GLB_SAFE_DELETE(m_DecalProgram);
        GLB_SAFE_DELETE(m_Camera);
        GLB_SAFE_DELETE(m_Floor);
        GLB_SAFE_DELETE(m_Decal);
        GLB_SAFE_DELETE(m_FloorAlbedoMap);
        GLB_SAFE_DELETE(m_DecalMap);
    }

    void DrawDecal() {
        // Setup render target
        render::Device::SetRenderTarget(m_DecalRenderTarget);
        render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

        // Setup viewport
        render::Device::SetViewport(0, 0, 2048, 2048);

        // Clear
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        render::Device::SetClearDepth(1.0f);
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        // Setup shader
        render::Device::SetShader(m_DecalProgram);
        render::Device::SetShaderLayout(m_DecalProgram->GetShaderLayout());

        // Setup texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, render::texture::Mgr::GetTextureById(m_Decal->GetTexId(scene::Model::MT_ALBEDO)), 0);

        // Setup mesh
        render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_Decal->GetMeshId())->GetVertexLayout());
        render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_Decal->GetMeshId())->GetVertexBuffer());

        // Setup render state
        render::Device::SetDepthTestEnable(true);
        render::Device::SetCullFaceEnable(false);
        render::Device::SetCullFaceMode(render::CULL_BACK);

        for (math::Vector& pos : m_DecalPos) {
            math::Matrix wvp = m_DecalViewProjM * math::Matrix::CreateTranslateMatrix(pos.x, pos.y, pos.z);

            // Setup uniform
            render::Device::SetUniformMatrix(m_DecalShaderWVPLoc, wvp);
            render::Device::SetUniformSampler2D(m_DecalShaderAlbedoLoc, 0);

            // Draw
            render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_Decal->GetMeshId())->GetVertexNum());
        }

        // Reset viewport
        render::Device::SetViewport(0, 0, app::Application::GetWindowWidth(), app::Application::GetWindowHeight());

        // Reset render target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());
    }

    void DrawFloor() {
        // Setup shader
        render::Device::SetShader(m_ColorProgram);
        render::Device::SetShaderLayout(m_ColorProgram->GetShaderLayout());

        // Setup texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, m_FloorAlbedoMap, 0);
        render::Device::SetTexture(1, m_DecalMap, 1);

        // Setup mesh
        render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_Floor->GetMeshId())->GetVertexLayout());
        render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_Floor->GetMeshId())->GetVertexBuffer());

        // Setup render state
        render::Device::SetDepthTestEnable(true);
        render::Device::SetCullFaceEnable(true);
        render::Device::SetCullFaceMode(render::CULL_BACK);

        // Setup uniform
        static float sRotX = 0.0f, sRotY = 0.0f;
        static float sPosX = 0.0f, sPosY = 0.0f, sPosZ = 0.0f;
        math::Matrix world;
        world.MakeIdentityMatrix();
        float mouseMoveX = Input::GetMouseMoveX();
        float mouseMoveY = Input::GetMouseMoveY();
        sRotX = sRotX + mouseMoveX * 0.1f;
        sRotY = sRotY + mouseMoveY * 0.1f;
        world.RotateY(sRotX);
        world.RotateX(sRotY);

        if (Input::IsKeyboardButtonPressed(BK_A)) {
            sPosX = sPosX + 0.1f;
        } else if (Input::IsKeyboardButtonPressed(BK_D)) {
            sPosX = sPosX - 0.1f;
        }

        if (Input::IsKeyboardButtonPressed(BK_Q)) {
            sPosY = sPosY + 0.1f;
        } else if (Input::IsKeyboardButtonPressed(BK_E)) {
            sPosY = sPosY - 0.1f;
        }

        if (Input::IsKeyboardButtonPressed(BK_W)) {
            sPosZ = sPosZ + 0.1f;
        } else if (Input::IsKeyboardButtonPressed(BK_S)) {
            sPosZ = sPosZ - 0.1f;
        }
        world.Translate(sPosX, sPosY, sPosZ);

        math::Matrix wvp;
        wvp.MakeIdentityMatrix();
        wvp = m_Proj * m_View * world;
        math::Matrix inv_trans_world = world;
        inv_trans_world.Inverse();
        inv_trans_world.Transpose();

        render::Device::SetUniformMatrix(m_ColorShaderWVPLoc, wvp);
        render::Device::SetUniformMatrix(m_ColorShaderDecalWVPLoc, m_DecalViewProjM);
        render::Device::SetUniformSampler2D(m_ColorShaderAlbedoLoc, 0);
        render::Device::SetUniformSampler2D(m_ColorShaderDecalMapLoc, 1);

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_Floor->GetMeshId())->GetVertexNum());
    }

    void UpdateDecalPos() {
        static int32_t sFrame = 0;
        if (sFrame == 0) {
            // Create Decal position
            auto RandRange = [](int min, int max) {
                return min + rand() % (max - min);
            };

            for (math::Vector& pos : m_DecalPos) {
                pos = math::Vector(1.0f * RandRange(-20.0f, 20.0f), 0.0f, 1.0f * RandRange(-20.0f, 20.0f));
            }

            // Create Decal View Projection Matrix
            m_DecalViewProjM = CalculateDecalViewProjMatrix();
        }
        sFrame = sFrame + 1;
        if (sFrame > 100) sFrame = 0;
    }

    math::Matrix CalculateDecalViewProjMatrix() {
        math::Vector minPos(FLT_MAX, FLT_MAX, FLT_MAX);
        math::Vector maxPos(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (math::Vector& pos : m_DecalPos) {
            if (pos.x < minPos.x) minPos.x = pos.x;
            if (pos.y < minPos.y) minPos.y = pos.y;
            if (pos.z < minPos.z) minPos.z = pos.z;
            if (pos.x > maxPos.x) maxPos.x = pos.x;
            if (pos.y > maxPos.y) maxPos.y = pos.y;
            if (pos.z > maxPos.z) maxPos.z = pos.z;
        }

        maxPos = maxPos + m_Decal->GetBoundBoxMax();
        minPos = minPos + m_Decal->GetBoundBoxMin();

        math::Matrix mat;

        // Size
        float width = (maxPos.x - minPos.x);
        float height = (maxPos.z - minPos.z);
        width = height = max(width, height);
        float depth = 10.0f + (maxPos.y - minPos.y);

        // Camera position in world space
        math::Vector pos = (maxPos + minPos) * 0.5f;

        // Target position in world space
        math::Vector look_at = math::Vector(0.0f, 1.0f, 0.01f);
        look_at.w = 0.0f;
        math::Vector target = pos + look_at;

        // Orthogonal projection matrix
        math::Matrix proj;
        proj.MakeOrthogonalMatrix(-width / 2.0f, width / 2.0f, - height / 2.0f, height / 2.0f, - depth / 2.0f, depth / 2.0f);

        // View matrix
        math::Matrix view;
        view.MakeViewMatrix(pos, target);

        // Shadow matrix
        mat.MakeIdentityMatrix();
        mat.Mul(proj);
        mat.Mul(view);

        return mat;
    }

protected:
    render::RenderTarget*           m_DecalRenderTarget;
    render::texture::Texture*       m_DecalMap;

    render::shader::UserProgram*    m_ColorProgram;
    render::shader::UserProgram*    m_DecalProgram;
    scene::CameraBase*              m_Camera;
    scene::Model*                   m_Floor;
    scene::Model*                   m_Decal;
    render::texture::Texture*       m_FloorAlbedoMap;
    math::Matrix                    m_Proj;
    math::Matrix                    m_View;

    math::Vector                    m_DecalPos[10];
    math::Matrix                    m_DecalViewProjM;

    int32_t                         m_ColorShaderWVPLoc;
    int32_t                         m_ColorShaderDecalWVPLoc;
    int32_t                         m_ColorShaderAlbedoLoc;
    int32_t                         m_ColorShaderDecalMapLoc;

    int32_t                         m_DecalShaderAlbedoLoc;
    int32_t                         m_DecalShaderWVPLoc;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_decal", sizeof(L"glb_decal"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationDecal::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}