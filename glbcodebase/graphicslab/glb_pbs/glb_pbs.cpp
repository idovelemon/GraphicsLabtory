//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 12 / 26
// Brief: Demostrate the physical based shading
//----------------------------------------------------------------------
#include "glb_pbs.h"
#include "resource.h"

class ApplicationPBS : public glb::app::ApplicationBase {
public:
    ApplicationPBS()
    : m_Program(NULL)
    , m_Sphere(NULL)
    , m_Camera(NULL)
    , m_Proj()
    , m_View()
    , m_WVPLoc(-1)
    , m_InvTransWorldLoc(-1)
    , m_WorldLoc(-1)
    , m_EyeLoc(-1)
    , m_AlbedoLoc(-1)
    , m_RougnessLoc(-1)
    , m_MetalicLoc(-1) {
    }
    virtual~ApplicationPBS() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationPBS;
    }

public:
    bool Initialize() {
        // Create Shader
        m_Program = render::shader::UserProgram::Create("res/pbs.vs", "res/pbs.fs");
        m_WVPLoc = m_Program->GetUniformLocation("glb_WVP");
        m_InvTransWorldLoc = m_Program->GetUniformLocation("glb_InvTransWorldM");
        m_WorldLoc = m_Program->GetUniformLocation("glb_World");
        m_EyeLoc = m_Program->GetUniformLocation("glb_EyePos");
        m_AlbedoLoc = m_Program->GetUniformLocation("glb_Albedo");
        m_RougnessLoc = m_Program->GetUniformLocation("glb_Roughness");
        m_MetalicLoc = m_Program->GetUniformLocation("glb_Metalic");

        // Create Sphere
        m_Sphere = scene::Model::Create("res/sphere.obj");

        // Create Camera
        m_Camera = scene::ModelCamera::Create(math::Vector(0.0f, 0.0f, 200.0f), math::Vector(0.0f, 0.0f, 0.0f));

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

        DrawSphere();

        // Reset render target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // SwapBuffers
        render::Device::SwapBuffer();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
        GLB_SAFE_DELETE(m_Program);
        GLB_SAFE_DELETE(m_Sphere);
        GLB_SAFE_DELETE(m_Camera);
    }

    void DrawSphere() {
        // Setup shader
        render::Device::SetShader(m_Program);
        render::Device::SetShaderLayout(m_Program->GetShaderLayout());

        // Setup texture
        //render::Device::SetTexture(0, m_EnvTex, 0);

        // Setup mesh
        render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexLayout());
        render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexBuffer());

        // Setup render state
        render::Device::SetDepthTestEnable(true);
        render::Device::SetCullFaceEnable(true);
        render::Device::SetCullFaceMode(render::CULL_BACK);

        for (int32_t i = 0; i < 5; i++) {
            for (int32_t j = 0; j < 5; j++) {
                // Setup uniform
                math::Matrix world;
                world.MakeIdentityMatrix();
                world.Translate(-300.0f + 150.0f * i, -300.0f + 150.0f * j, -450.0f);

                math::Matrix wvp;
                wvp.MakeIdentityMatrix();
                wvp = m_Proj * m_View * world;
                math::Matrix inv_trans_world = world;
                inv_trans_world.Inverse();
                inv_trans_world.Transpose();

                render::Device::SetUniformMatrix(m_WVPLoc, wvp);
                render::Device::SetUniformMatrix(m_InvTransWorldLoc, inv_trans_world);
                render::Device::SetUniformMatrix(m_WorldLoc, world);
                render::Device::SetUniform3f(m_EyeLoc, math::Vector(0.0f, 0.0f, 200.0f));
                render::Device::SetUniform3f(m_AlbedoLoc, math::Vector(1.0f, 0.0f, 0.0f));
                render::Device::SetUniform1f(m_RougnessLoc, i * 0.2f);
                render::Device::SetUniform1f(m_MetalicLoc, j * 0.2f);

                // Draw
                render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexNum());
            }
        }
    }

protected:
    render::shader::UserProgram*    m_Program;
    scene::Model*                   m_Sphere;
    scene::CameraBase*              m_Camera;
    math::Matrix                    m_Proj;
    math::Matrix                    m_View;

    int32_t                         m_WVPLoc;
    int32_t                         m_InvTransWorldLoc;
    int32_t                         m_WorldLoc;
    int32_t                         m_EyeLoc;
    int32_t                         m_AlbedoLoc;
    int32_t                         m_RougnessLoc;
    int32_t                         m_MetalicLoc;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_pbs", sizeof(L"glb_pbs"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationPBS::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}