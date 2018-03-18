//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 01 / 14
// Brief: Demostrate how to convolute diffuse ibl map
//----------------------------------------------------------------------
#include "glb_ibl_diffuse.h"
#include "resource.h"

class ApplicationIBLDiffuse : public glb::app::ApplicationBase {
public:
    ApplicationIBLDiffuse()
    : m_CubeMapRT(NULL)
    , m_CubeMap(NULL)
    , m_ConvolutionCubeMapRT(NULL)
    , m_ConvolutionCubeMap(NULL)
    , m_ConvertERMapToCubeMapProgram(NULL)
    , m_ConvolutionCubeMapProgram(NULL)
    , m_PBSProgram(NULL)
    , m_TestCubeMapProgram(NULL)
    , m_ERMap(NULL)
    , m_Sphere(NULL)
    , m_Camera(NULL)
    , m_Proj()
    , m_View()
    , m_WVPLoc(-1)
    , m_ERMapLoc(-1)
    , m_ConvolutionProgram_CubeMapLoc(-1)
    , m_ConvolutionProgram_FaceIndexLoc(-1)
    , m_PBSProgram_WVPLoc(-1)
    , m_PBSProgram_WorldLoc(-1)
    , m_PBSProgram_InvTransWorldMLoc(-1)
    , m_PBSProgram_AlbedoLoc(-1)
    , m_PBSProgram_RoughnessLoc(-1)
    , m_PBSProgram_MetalicLoc(-1)
    , m_PBSProgram_EyePosLoc(-1)
    , m_PBSProgram_IrradianceMapLoc(-1)
    , m_TestCubeMap_WVPLoc(-1)
    , m_TestProgram_CubeMapLoc(-1) {
    }
    virtual~ApplicationIBLDiffuse() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationIBLDiffuse;
    }

public:
    bool Initialize() {
        // Create Render target
        m_CubeMapRT = render::RenderTarget::Create(1024, 1024);
        m_CubeMap = render::texture::Texture::CreateFloat16CubeTexture(1024, 1024);
        render::DrawColorBuffer drawBuffer[6] = {
            render::COLORBUF_COLOR_ATTACHMENT0,
            render::COLORBUF_COLOR_ATTACHMENT1,
            render::COLORBUF_COLOR_ATTACHMENT2,
            render::COLORBUF_COLOR_ATTACHMENT3,
            render::COLORBUF_COLOR_ATTACHMENT4,
            render::COLORBUF_COLOR_ATTACHMENT5,
        };
        m_CubeMapRT->AttachCubeTexture(drawBuffer, m_CubeMap);

        m_ConvolutionCubeMapRT = render::RenderTarget::Create(32, 32);
        m_ConvolutionCubeMap = render::texture::Texture::CreateFloat16CubeTexture(32, 32);
        m_ConvolutionCubeMapRT->AttachCubeTexture(drawBuffer, m_ConvolutionCubeMap);

        // Create Shader
        m_ConvertERMapToCubeMapProgram = render::shader::UserProgram::Create("res/filtering_ermap.vs", "res/filtering_ermap.fs");
        m_WVPLoc = m_ConvertERMapToCubeMapProgram->GetUniformLocation("glb_WVP");
        m_ERMapLoc = m_ConvertERMapToCubeMapProgram->GetUniformLocation("glb_EquirectangularMap");

        m_ConvolutionCubeMapProgram = render::shader::UserProgram::Create("res/convolution.vs", "res/convolution.fs");
        m_ConvolutionProgram_CubeMapLoc = m_ConvolutionCubeMapProgram->GetUniformLocation("glb_CubeMap");
        m_ConvolutionProgram_FaceIndexLoc = m_ConvolutionCubeMapProgram->GetUniformLocation("glb_FaceIndex");

        m_PBSProgram = render::shader::UserProgram::Create("res/pbs.vs", "res/pbs.fs");
        m_PBSProgram_WVPLoc = m_PBSProgram->GetUniformLocation("glb_WVP");
        m_PBSProgram_WorldLoc = m_PBSProgram->GetUniformLocation("glb_World");
        m_PBSProgram_InvTransWorldMLoc = m_PBSProgram->GetUniformLocation("glb_InvTransWorldM");
        m_PBSProgram_AlbedoLoc = m_PBSProgram->GetUniformLocation("glb_Albedo");
        m_PBSProgram_RoughnessLoc = m_PBSProgram->GetUniformLocation("glb_Roughness");
        m_PBSProgram_MetalicLoc = m_PBSProgram->GetUniformLocation("glb_Metalic");
        m_PBSProgram_EyePosLoc = m_PBSProgram->GetUniformLocation("glb_EyePos");
        m_PBSProgram_IrradianceMapLoc = m_PBSProgram->GetUniformLocation("glb_IrradianceMap");

        m_TestCubeMapProgram = render::shader::UserProgram::Create("res/test_cubemap.vs", "res/test_cubemap.fs");
        m_TestCubeMap_WVPLoc = m_TestCubeMapProgram->GetUniformLocation("glb_WVP");
        m_TestProgram_CubeMapLoc = m_TestCubeMapProgram->GetUniformLocation("glb_CubeMap");

        // Create Mesh
        m_Sphere = scene::Model::Create("res/sphere.obj");
        m_ScreenMesh = render::mesh::ScreenMesh::Create();

        // Create texture
        m_ERMap = render::texture::Texture::Create("res/ermap2.hdr");

        // Create Camera
        m_Camera = scene::ModelCamera::Create(math::Vector(0.0f, 0.0f, 800.0f), math::Vector(0.0f, 0.0f, 0.0f));

        m_Proj.MakeProjectionMatrix(1.0f * app::Application::GetWindowWidth() / app::Application::GetWindowHeight(), 60.0f, 0.01f, 10000.0f);
        m_View = m_Camera->GetViewMatrix();

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();

        // Calculate irradiance map
        static bool isIrradianceMapGenerated = false;
        if (!isIrradianceMapGenerated) {
            DrawCubeMap();
            m_CubeMap->GenerateMipmap();

            DrawConvolutionCubeMap();
            m_ConvolutionCubeMap->GenerateMipmap();

            isIrradianceMapGenerated = true;
        }

        // Draw the cube map to check it
        //DrawTestCubeMap();

        // Draw scene
        DrawScene();

        // Reset render target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // SwapBuffers
        render::Device::SwapBuffer();

        GLB_CHECK_GL_ERROR;

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
        GLB_SAFE_DELETE(m_CubeMapRT);
        GLB_SAFE_DELETE(m_CubeMap);
        GLB_SAFE_DELETE(m_ConvolutionCubeMapRT);
        GLB_SAFE_DELETE(m_ConvolutionCubeMap);
        GLB_SAFE_DELETE(m_ConvertERMapToCubeMapProgram);
        GLB_SAFE_DELETE(m_ConvolutionCubeMapProgram);
        GLB_SAFE_DELETE(m_PBSProgram);
        GLB_SAFE_DELETE(m_TestCubeMapProgram);
        GLB_SAFE_DELETE(m_ERMap);
        GLB_SAFE_DELETE(m_Sphere);
        GLB_SAFE_DELETE(m_ScreenMesh);
        GLB_SAFE_DELETE(m_Camera);
    }

    void DrawCubeMap() {
        // New perspective matrix
        math::Matrix proj;
        proj.MakeProjectionMatrix(1.0f, 90.0f, 0.1f, 1000.0f);

        // 6 View matrix(+X,-X,+Y,-Y,+Z,-Z)
        math::Matrix views[6];
        views[0].MakeViewMatrix(math::Vector(0.0f, 0.0f, 0.0f), math::Vector(0.0f, 0.0f, 0.0f) + math::Vector(1.0f, 0.0f, 0.0f));
        views[1].MakeViewMatrix(math::Vector(0.0f, 0.0f, 0.0f), math::Vector(0.0f, 0.0f, 0.0f) + math::Vector(-1.0f, 0.0f, 0.0f));
        views[2].MakeViewMatrix(math::Vector(0.0f, 0.0f, 0.0f), math::Vector(1.0f, 0.0f, 0.0f), math::Vector(0.0f, 0.0f, 1.0f), math::Vector(0.0f, -1.0f, 0.0f));
        views[3].MakeViewMatrix(math::Vector(0.0f, 0.0f, 0.0f), math::Vector(1.0f, 0.0f, 0.0f), math::Vector(0.0f, 0.0f, -1.0f), math::Vector(0.0f, 1.0f, 0.0f));
        views[4].MakeViewMatrix(math::Vector(0.0f, 0.0f, 0.0f), math::Vector(0.0f, 0.0f, 0.0f) + math::Vector(0.0f, 0.0f, 1.0f));
        views[5].MakeViewMatrix(math::Vector(0.0f, 0.0f, 0.0f), math::Vector(0.0f, 0.0f, 0.0f) + math::Vector(0.0f, 0.0f, -1.0f));

        // Render Target
        render::Device::SetRenderTarget(m_CubeMapRT);

        // View port
        render::Device::SetViewport(0, 0, 1024, 1024);

        // Setup shader
        render::Device::SetShader(m_ConvertERMapToCubeMapProgram);
        render::Device::SetShaderLayout(m_ConvertERMapToCubeMapProgram->GetShaderLayout());

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

        for (int32_t i = 0; i < 6; i++) {
            // Set Draw Color Buffer
            render::Device::SetDrawColorBuffer(static_cast<render::DrawColorBuffer>(render::COLORBUF_COLOR_ATTACHMENT0 + i));

            // Clear
            render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
            render::Device::SetClearDepth(1.0f);
            render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

            // Setup uniform
            math::Matrix world;
            world.MakeIdentityMatrix();

            math::Matrix wvp;
            wvp.MakeIdentityMatrix();
            wvp = proj * views[i] * world;
            math::Matrix inv_trans_world = world;
            inv_trans_world.Inverse();
            inv_trans_world.Transpose();

            render::Device::SetUniformMatrix(m_WVPLoc, wvp);
            render::Device::SetUniformSampler2D(m_ERMapLoc, 0);

            // Draw
            render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexNum());
        }
    }

    void DrawConvolutionCubeMap() {
        // Render Target
        render::Device::SetRenderTarget(m_ConvolutionCubeMapRT);

        // View port
        render::Device::SetViewport(0, 0, 32, 32);

        // Setup shader
        render::Device::SetShader(m_ConvolutionCubeMapProgram);
        render::Device::SetShaderLayout(m_ConvolutionCubeMapProgram->GetShaderLayout());

        // Setup texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, m_CubeMap, 0);

        // Setup mesh
        render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());
        render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());

        // Setup render state
        render::Device::SetDepthTestEnable(true);
        render::Device::SetCullFaceEnable(true);
        render::Device::SetCullFaceMode(render::CULL_BACK);

        for (int32_t i = 0; i < 6; i++) {
            // Set Draw Color Buffer
            render::Device::SetDrawColorBuffer(static_cast<render::DrawColorBuffer>(render::COLORBUF_COLOR_ATTACHMENT0 + i));

            // Clear
            render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
            render::Device::SetClearDepth(1.0f);
            render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

            // Setup uniform
            render::Device::SetUniformSamplerCube(m_ConvolutionProgram_CubeMapLoc, 0);
            render::Device::SetUniform1i(m_ConvolutionProgram_FaceIndexLoc, i);

            // Draw
            render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());
        }
    }

    void DrawTestCubeMap() {
        // Set Render Target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // Set Viewport
        render::Device::SetViewport(0, 0, app::Application::GetWindowWidth(), app::Application::GetWindowHeight());

        // Clear
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
        render::Device::SetClearDepth(1.0f);
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        // Setup shader
        render::Device::SetShader(m_TestCubeMapProgram);
        render::Device::SetShaderLayout(m_TestCubeMapProgram->GetShaderLayout());

        // Setup texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, m_ConvolutionCubeMap, 0);
        //render::Device::SetTexture(0, m_CubeMap, 0);

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
        world.Translate(0.0f, 0.0f, 200.0f);

        math::Matrix wvp;
        wvp.MakeIdentityMatrix();
        wvp = m_Proj * m_View * world;
        math::Matrix inv_trans_world = world;
        inv_trans_world.Inverse();
        inv_trans_world.Transpose();

        render::Device::SetUniformMatrix(m_TestCubeMap_WVPLoc, wvp);
        render::Device::SetUniformSamplerCube(m_TestProgram_CubeMapLoc, 0);

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexNum());
    }

    void DrawScene() {
        // Set Render Target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // Set Viewport
        render::Device::SetViewport(0, 0, app::Application::GetWindowWidth(), app::Application::GetWindowHeight());

        // Clear
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
        render::Device::SetClearDepth(1.0f);
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        math::Vector pos = m_Camera->GetPos();
        math::Vector target = m_Camera->GetTarget();
        math::Vector look = pos - target;
        math::Matrix rot;
        rot.MakeRotateYMatrix(Input::GetMouseMoveX() * 0.1f);
        look = rot * look;
        pos = target + look;
        m_Camera->SetPos(pos);
        m_Camera->Update(1.0f);
        m_View = m_Camera->GetViewMatrix();

        DrawEnv();

        DrawSphere();
    }

    void DrawEnv() {
        // Setup shader
        render::Device::SetShader(m_TestCubeMapProgram);
        render::Device::SetShaderLayout(m_TestCubeMapProgram->GetShaderLayout());

        // Setup texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, m_CubeMap, 0);

        // Setup mesh
        render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexLayout());
        render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexBuffer());

        // Setup render state
        render::Device::SetDepthTestEnable(false);
        render::Device::SetCullFaceEnable(true);
        render::Device::SetCullFaceMode(render::CULL_FRONT);

        // Setup uniform
        math::Matrix world;
        world.MakeIdentityMatrix();
        math::Vector pos = m_Camera->GetPos();
        world.Translate(pos.x, pos.y, pos.z);

        math::Matrix wvp;
        wvp.MakeIdentityMatrix();
        wvp = m_Proj * m_View * world;
        math::Matrix inv_trans_world = world;
        inv_trans_world.Inverse();
        inv_trans_world.Transpose();

        render::Device::SetUniformMatrix(m_TestCubeMap_WVPLoc, wvp);
        render::Device::SetUniformSamplerCube(m_TestProgram_CubeMapLoc, 0);

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexNum());
    }

    void DrawSphere() {
        // Setup shader
        render::Device::SetShader(m_PBSProgram);
        render::Device::SetShaderLayout(m_PBSProgram->GetShaderLayout());

        // Setup texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, m_ConvolutionCubeMap, 0);

        // Setup mesh
        render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexLayout());
        render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexBuffer());

        // Setup render state
        render::Device::SetDepthTestEnable(true);
        render::Device::SetCullFaceEnable(true);
        render::Device::SetCullFaceMode(render::CULL_BACK);

        // Setup uniform
        for (int32_t i = 0; i < 6; i++) {
            for (int32_t j = 0; j < 6; j++) {
                // Setup uniform
                math::Matrix world;
                world.MakeIdentityMatrix();
                world.Translate(-380.0f + 150.0f * i, -380.0f + 150.0f * j, 0.0f);

                math::Matrix wvp;
                wvp.MakeIdentityMatrix();
                wvp = m_Proj * m_View * world;
                math::Matrix inv_trans_world = world;
                inv_trans_world.Inverse();
                inv_trans_world.Transpose();

                render::Device::SetUniformMatrix(m_PBSProgram_WVPLoc, wvp);
                render::Device::SetUniformMatrix(m_PBSProgram_WorldLoc, world);
                render::Device::SetUniformMatrix(m_PBSProgram_InvTransWorldMLoc, inv_trans_world);
                render::Device::SetUniform3f(m_PBSProgram_AlbedoLoc, math::Vector(1.0f, 1.0f, 1.0f));
                render::Device::SetUniform1f(m_PBSProgram_RoughnessLoc, i * 0.2f);
                render::Device::SetUniform1f(m_PBSProgram_MetalicLoc, j * 0.2f);
                render::Device::SetUniform3f(m_PBSProgram_EyePosLoc, m_Camera->GetPos());
                render::Device::SetUniformSamplerCube(m_PBSProgram_IrradianceMapLoc, 0);

                // Draw
                render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexNum());
            }
        }

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_Sphere->GetMeshId())->GetVertexNum());
    }

protected:
    render::RenderTarget*           m_CubeMapRT;
    render::texture::Texture*       m_CubeMap;
    render::RenderTarget*           m_ConvolutionCubeMapRT;
    render::texture::Texture*       m_ConvolutionCubeMap;
    render::shader::UserProgram*    m_ConvertERMapToCubeMapProgram;
    render::shader::UserProgram*    m_ConvolutionCubeMapProgram;
    render::shader::UserProgram*    m_PBSProgram;
    render::shader::UserProgram*    m_TestCubeMapProgram;
    render::texture::Texture*       m_ERMap;
    scene::Model*                   m_Sphere;
    render::mesh::ScreenMesh*       m_ScreenMesh;
    scene::CameraBase*              m_Camera;
    math::Matrix                    m_Proj;
    math::Matrix                    m_View;

    int32_t                         m_WVPLoc;
    int32_t                         m_ERMapLoc;

    int32_t                         m_ConvolutionProgram_CubeMapLoc;
    int32_t                         m_ConvolutionProgram_FaceIndexLoc;

    int32_t                         m_PBSProgram_WVPLoc;
    int32_t                         m_PBSProgram_WorldLoc;
    int32_t                         m_PBSProgram_InvTransWorldMLoc;
    int32_t                         m_PBSProgram_AlbedoLoc;
    int32_t                         m_PBSProgram_RoughnessLoc;
    int32_t                         m_PBSProgram_MetalicLoc;
    int32_t                         m_PBSProgram_EyePosLoc;
    int32_t                         m_PBSProgram_IrradianceMapLoc;

    int32_t                         m_TestCubeMap_WVPLoc;
    int32_t                         m_TestProgram_CubeMapLoc;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_ibl_diffuse", sizeof(L"glb_ibl_diffuse"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationIBLDiffuse::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}