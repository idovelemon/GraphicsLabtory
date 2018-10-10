//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 01 / 11
// Brief: Demostrate how to sampling Equirectangular Map
//----------------------------------------------------------------------
#include "glb_equirectangularmap.h"
#include "resource.h"

class ApplicationERMap : public glb::app::ApplicationBase {
public:
    ApplicationERMap()
    : m_Program(NULL)
    , m_ERMap(NULL)
    , m_Sphere(NULL)
    , m_Camera(NULL)
    , m_Proj()
    , m_View()
    , m_WVPLoc(-1)
    , m_ERMapLoc(-1) {
    }
    virtual~ApplicationERMap() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationERMap;
    }

public:
    bool Initialize() {
        // Create Shader
        m_Program = render::shader::UserProgram::Create("res/equirectangularmap.vs", "res/equirectangularmap.fs");
        m_WVPLoc = m_Program->GetUniformLocation("glb_WVP");
        m_ERMapLoc = m_Program->GetUniformLocation("glb_EquirectangularMap");

        // Create Sphere
        m_Sphere = scene::Model::Create("res/sphere.obj");

        // Create texture
        m_ERMap = render::texture::Texture::Create("res/ermap_8k.bmp");

        // Create Camera
        m_Camera = scene::ModelCamera::Create(math::Vector(0.0f, 0.0f, 0.0f), math::Vector(0.0f, 0.0f, -1.0f));

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
        GLB_SAFE_DELETE(m_ERMap);
        GLB_SAFE_DELETE(m_Sphere);
        GLB_SAFE_DELETE(m_Camera);
    }

    void DrawSphere() {
        // Setup shader
        render::Device::SetShader(m_Program);
        render::Device::SetShaderLayout(m_Program->GetShaderLayout());

        // Setup texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, m_ERMap, 0);

        // Setup mesh
        render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexLayout());
        render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexBuffer());

        // Setup render state
        render::Device::SetDepthTestEnable(true);
        render::Device::SetCullFaceEnable(true);
        render::Device::SetCullFaceMode(render::CULL_FRONT);

        // Setup uniform
        static float sRotX = 0.0f, sRotY = 0.0f;
        math::Matrix world;
        world.MakeIdentityMatrix();
        float mouseMoveX = Input::GetMouseMoveX();
        float mouseMoveY = Input::GetMouseMoveY();
        sRotX = sRotX + mouseMoveX * 0.1f;
        sRotY = sRotY + mouseMoveY * 0.1f;
        world.RotateY(sRotX);
        world.RotateX(sRotY);

        math::Matrix wvp;
        wvp.MakeIdentityMatrix();
        wvp = m_Proj * m_View * world;
        math::Matrix inv_trans_world = world;
        inv_trans_world.Inverse();
        inv_trans_world.Transpose();

        render::Device::SetUniformMatrix(m_WVPLoc, wvp);
        render::Device::SetUniformSampler2D(m_ERMapLoc, 0);

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexNum());
    }

protected:
    render::shader::UserProgram*    m_Program;
    render::texture::Texture*       m_ERMap;
    scene::Model*                   m_Sphere;
    scene::CameraBase*              m_Camera;
    math::Matrix                    m_Proj;
    math::Matrix                    m_View;

    int32_t                         m_WVPLoc;
    int32_t                         m_ERMapLoc;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_equirectangularmap", sizeof(L"glb_equirectangularmap"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationERMap::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}