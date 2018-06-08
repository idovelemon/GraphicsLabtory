//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 07
// Brief: Core routine for light studio
//------------------------------------------------------------------
#include "glb_lightstudio_core.h"

#include "GL/glew.h"

using namespace glb;

ApplicationCore* ApplicationCore::s_Instance = NULL;

ApplicationCore::ApplicationCore()
: m_Camera(NULL)
, m_SceneMesh(NULL)
, m_SceneProgram(NULL)
, m_LightMapWidth(128)
, m_LightMapHeight(128) {
    s_Instance = this;
    memset(m_LightMap, 0, sizeof(m_LightMap));
}

ApplicationCore::~ApplicationCore() {
    s_Instance = NULL;
    Destroy();
}

app::ApplicationBase* ApplicationCore::Create() {
    return new ApplicationCore();
}

ApplicationCore* ApplicationCore::GetInstance() {
    return s_Instance;
}

bool ApplicationCore::Initialize() {
    // Create Shader
    m_SceneProgram = render::shader::UserProgram::Create("res/draw.vs", "res/draw.fs");

    // Create Camera
    m_Camera = scene::ModelCamera::Create(math::Vector(0.0f, 250.0f, 150.0f), math::Vector(0.0f, 0.0f, 0.0f));

    return true;
}

void ApplicationCore::Update(float dt) {
    util::ProfileTime time;
    time.BeginProfile();

    Input::Update();

    UpdateCamera();
    DrawScene();

    // Reset render target
    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

    // SwapBuffers
    render::Device::SwapBuffer();

    GLB_CHECK_GL_ERROR;

    time.EndProfile();
    // printf("%f\n", time.GetProfileTimeInMs());
}

void ApplicationCore::Destroy() {
    GLB_SAFE_DELETE(m_SceneProgram);
    GLB_SAFE_DELETE(m_Camera);
}

bool ApplicationCore::AddSceneMesh(const char* name) {
    if (!m_SceneMesh) {
        scene::Model* mesh = scene::Model::Create(name);
        if (!mesh->HasAlbedoTexture()) return false;
        if (!mesh->HasNormalTexture()) return false;
        if (!mesh->HasNormal()) return false;
        if (!mesh->HasTangent()) return false;
        if (!mesh->HasBinormal()) return false;

        m_SceneMesh = mesh;
        m_LightMap[0] = render::texture::Texture::CreateFloat32Texture(m_LightMapWidth, m_LightMapHeight);
        m_LightMap[1] = render::texture::Texture::CreateFloat32Texture(m_LightMapWidth, m_LightMapHeight);
        m_LightMap[2] = render::texture::Texture::CreateFloat32Texture(m_LightMapWidth, m_LightMapHeight);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return true;
}

void ApplicationCore::UpdateCamera() {
    if (Input::IsKeyboardButtonPressed(BK_LCONTROL) && Input::IsMouseButtonPressed(BM_LEFT)) {
        static float sRot = 0.0f;
        sRot = sRot + Input::GetMouseMoveX() * 0.1f;
        static float sDist = 0.0f;
        sDist = sDist + Input::GetMouseMoveY() * 0.05f;
        math::Vector pos = math::Vector(10.0f, 10.0f, 0.0) + math::Vector(1.0f, 1.0f, 0.0f) * sDist;
        math::Matrix rotY;
        rotY.MakeRotateYMatrix(sRot);
        pos = rotY * pos;
        m_Camera->SetPos(pos);
        m_Camera->Update(1.0f);
    }
}

void ApplicationCore::DrawScene() {
    // Check if need draw scene
    if (!m_SceneMesh) return;

    // Setup Render Target
    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

    // Setup Shader
    render::Device::SetShader(m_SceneProgram);
    render::Device::SetShaderLayout(m_SceneProgram->GetShaderLayout());

    // Setup Viewport
    render::Device::SetViewport(0, 0, app::Application::GetWindowWidth(), app::Application::GetWindowHeight());

    // Setup Render State
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::SetDepthTestEnable(true);
    render::Device::SetCullFaceEnable(true);
    render::Device::SetCullFaceMode(render::CULL_BACK);

    // Setup Texture
    render::Device::ClearTexture();
    render::Device::SetTexture(0, m_LightMap[0], 0);
    render::Device::SetTexture(1, m_LightMap[1], 1);
    render::Device::SetTexture(2, m_LightMap[2], 2);
    render::Device::SetTexture(3, render::texture::Mgr::GetTextureById(m_SceneMesh->GetTexId(render::TS_ALBEDO)), 3);
    render::Device::SetTexture(4, render::texture::Mgr::GetTextureById(m_SceneMesh->GetTexId(render::TS_NORMAL)), 4);

    // Clear
    render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

    {  // Draw Scene
        // Setup Vertex
        render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexLayout());
        render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexBuffer());

        // Setup Uniform
        math::Matrix proj;
        proj.MakeProjectionMatrix(app::Application::GetWindowWidth() * 1.0f / app::Application::GetWindowHeight(), 75.0f, 0.001f, 10000.0f);
        render::Device::SetUniformMatrix(m_SceneProgram->GetUniformLocation("glb_MVP"), proj * m_Camera->GetViewMatrix());
        render::Device::SetUniformSampler2D(m_SceneProgram->GetUniformLocation("glb_LightMap[0]"), 0);
        render::Device::SetUniformSampler2D(m_SceneProgram->GetUniformLocation("glb_LightMap[1]"), 1);
        render::Device::SetUniformSampler2D(m_SceneProgram->GetUniformLocation("glb_LightMap[2]"), 2);
        render::Device::SetUniformSampler2D(m_SceneProgram->GetUniformLocation("glb_AlbedoMap"), 3);
        render::Device::SetUniformSampler2D(m_SceneProgram->GetUniformLocation("glb_NormalMap"), 4);

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexNum());
    }
}