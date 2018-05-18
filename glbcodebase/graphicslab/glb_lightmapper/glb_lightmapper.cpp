//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 05 / 03
// Brief: A light mapper tool using Radiosity Algorithm
//----------------------------------------------------------------------
#include "glb_lightmapper.h"
#include "resource1.h"

#include <fstream>

static const char* kSceneModelFile = "res/scene.obj";

class ApplicationLightMapper : public glb::app::ApplicationBase {
public:
    ApplicationLightMapper()
    : m_CurIterateNum(0)
    , m_TotalIterateNum(3)
    , m_CurValidPatchIndex(0)
    , m_LightMap(NULL)
    , m_Camera(NULL)
    , m_View()
    , m_Proj()
    , m_SceneMesh(NULL)
    , m_EnvLightBallMesh(NULL)
    , m_InDoorLightBallMesh(NULL)
    , m_ScreenMesh(NULL)
    , m_CurLightMapW(0)
    , m_CurLightMapH(0)
    , m_WeightRT(NULL)
    , m_WeightProgram(NULL)
    , m_WeightFaceLoc(-1)
    , m_WeightLightPatchWidthLoc(-1)
    , m_WeightLightPatchHeightLoc(-1)
    , m_TotalWeightColor(0.0f, 0.0f, 0.0f)
    , m_NormalizeWeightRT(NULL)
    , m_NormalizeWeightProgram(NULL)
    , m_TotalWeightColorLoc(-1)
    , m_WeightMapLoc(-1)
    , m_LightPatchRT(NULL)
    , m_LightPatchProgram(NULL)
    , m_LightPatchMVPLoc(-1)
    , m_LightPatchLightValueLoc(-1)
    , m_NormlaizeWeightMapLoc(-1)
    , m_LightMapLoc(-1)
    , m_LightPatchFaceLoc(-1)
    , m_DrawProgram(NULL)
    , m_DrawMVPLoc(-1)
    , m_DrawLightMapLoc(-1) {
        for (int32_t i = 0; i < 5; i++) {
            m_WeightMap[i] = NULL;
            m_LightPatchMap[i] = NULL;
        }
        m_ValidPatch.clear();
    }
    virtual~ApplicationLightMapper() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationLightMapper;
    }

public:
    void PrepareLightPatch() {
        memset(m_Patch, 0, sizeof(m_Patch));

        // Calculate uv for every patch
        for (int32_t h = 0; h < kLightMapHeight; h++) {
            for (int32_t w = 0; w < kLightMapWidth; w++) {
                m_Patch[h][w].uv = math::Vector((w + 0.5f) * 1.0f / kLightMapWidth, (kLightMapHeight - h - 0.5f) * 1.0f / kLightMapHeight, 0.0f);
            }
        }

        // Collect all faces
        struct Face {
            struct {
                math::Vector uv;
                math::Vector pos;
                math::Vector normal;
            } vertex[3];
        };
        std::vector<Face> faces;
        faces.clear();

        scene::ModelEffectParam effectParam;
        scene::ModelMaterialParam materialParam;
        float* vertexBuf = NULL;
        float* texBuf = NULL;
        float* normalBuf = NULL;
        int32_t faceNum = scene::ModelFile::ExtractModelData(kSceneModelFile, effectParam, materialParam, &vertexBuf, &texBuf, &normalBuf);

        int32_t vertexOffset = 0, uvOffset = 0, normalOffset = 0;
        for (int32_t i = 0; i < faceNum; i++) {
            Face face;

            for (int32_t j = 0; j < 3; j++) {
                face.vertex[j].uv.x = texBuf[uvOffset++];
                face.vertex[j].uv.y = texBuf[uvOffset++];
                face.vertex[j].uv.z = 0.0f;
                face.vertex[j].uv.w = 0.0f;
                face.vertex[j].pos.x = vertexBuf[vertexOffset++];
                face.vertex[j].pos.y = vertexBuf[vertexOffset++];
                face.vertex[j].pos.z = vertexBuf[vertexOffset++];
                face.vertex[j].pos.w = 1.0f;
                face.vertex[j].normal.x = normalBuf[normalOffset++];
                face.vertex[j].normal.y = normalBuf[normalOffset++];
                face.vertex[j].normal.z = normalBuf[normalOffset++];
                face.vertex[j].normal.w = 0.0f;
            }

            faces.push_back(face);
        }

        scene::ModelFile::RelaseBuf(&vertexBuf, &texBuf, &normalBuf);

        // Calculate data for every patch
        for (int32_t h = 0; h < kLightMapHeight; h++) {
            for (int32_t w = 0; w < kLightMapWidth; w++) {
                math::Vector uv = m_Patch[h][w].uv;

                bool found = false;
                for (int32_t i = 0; i < faceNum; i++) {
                    // Using triangle's barycentric coordinate system to calculate world position of light patch
                    // https://en.wikipedia.org/wiki/Barycentric_coordinate_system
                    float x = uv.x;
                    float y = uv.y;
                    float x1 = faces[i].vertex[0].uv.x;
                    float x2 = faces[i].vertex[1].uv.x;
                    float x3 = faces[i].vertex[2].uv.x;
                    float y1 = faces[i].vertex[0].uv.y;
                    float y2 = faces[i].vertex[1].uv.y;
                    float y3 = faces[i].vertex[2].uv.y;

                    float lambda0 = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
                    if (lambda0 < 0.0f || lambda0 > 1.0f) continue;

                    float lambda1 = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
                    if (lambda1 < 0.0f || lambda1 > 1.0f) continue;

                    float lambda2 = 1.0f - lambda0 - lambda1;
                    if (lambda2 < 0.0f || lambda2 > 1.0f) continue;

                    m_Patch[h][w].pos = faces[i].vertex[0].pos * lambda0 + faces[i].vertex[1].pos * lambda1 + faces[i].vertex[2].pos * lambda2;
                    m_Patch[h][w].normal = faces[i].vertex[0].normal;
                    m_Patch[h][w].valid = true;

                    m_ValidPatch.push_back(&m_Patch[h][w]);

                    found = true;
                    break;
                }

                if (found == false) {
                    m_Patch[h][w].valid = false;
                }
            }
        }
    }

    bool FindNextValidLightPatch() {
        bool result = true;

        m_CurValidPatchIndex = m_CurValidPatchIndex + 1;
        if (m_CurValidPatchIndex >= m_ValidPatch.size()) {
            m_CurValidPatchIndex = 0;
            result = false;
        }

        return result;
    }

    bool Initialize() {
        // Disable VSync
        render::Device::SetupVSync(false);

        // Prepare light patch
        PrepareLightPatch();

        // Create Render target
        render::DrawColorBuffer drawBuf[] = {
            render::COLORBUF_COLOR_ATTACHMENT0,
            render::COLORBUF_COLOR_ATTACHMENT1,
            render::COLORBUF_COLOR_ATTACHMENT2,
            render::COLORBUF_COLOR_ATTACHMENT3,
            render::COLORBUF_COLOR_ATTACHMENT4,
        };
        m_LightPatchRT = render::RenderTarget::Create(kLightPatchSize, kLightPatchSize);
        for (int32_t i = 0; i < 5; i++) {
            m_LightPatchMap[i] = render::texture::Texture::CreateFloat32Texture(kLightPatchSize, kLightPatchSize);
            m_LightPatchRT->AttachColorTexture(drawBuf[i], m_LightPatchMap[i]);
        }

        m_WeightRT = render::RenderTarget::Create(kLightPatchSize, kLightPatchSize);
        for (int32_t i = 0; i < 5; i++) {
            m_WeightMap[i] = render::texture::Texture::CreateFloat32Texture(kLightPatchSize, kLightPatchSize);
            m_WeightRT->AttachColorTexture(drawBuf[i], m_WeightMap[i]);
        }

        m_NormalizeWeightRT = render::RenderTarget::Create(kLightPatchSize, kLightPatchSize);
        for (int32_t i = 0; i < 5; i++) {
            m_NormalizeWeightMap[i] = render::texture::Texture::CreateFloat32Texture(kLightPatchSize, kLightPatchSize);
            m_NormalizeWeightRT->AttachColorTexture(drawBuf[i], m_NormalizeWeightMap[i]);
        }

        // Create Shader
        m_LightPatchProgram = render::shader::UserProgram::Create("res/lightPatch.vs", "res/lightPatch.fs");
        m_LightPatchMVPLoc = m_LightPatchProgram->GetUniformLocation("glb_MVP");
        m_LightPatchLightValueLoc = m_LightPatchProgram->GetUniformLocation("glb_LightColor");
        m_NormlaizeWeightMapLoc = m_LightPatchProgram->GetUniformLocation("glb_NormalizeWeightMap");
        m_LightMapLoc = m_LightPatchProgram->GetUniformLocation("glb_LightMap");
        m_LightPatchFaceLoc = m_LightPatchProgram->GetUniformLocation("glb_Face");

        m_WeightProgram = render::shader::UserProgram::Create("res/weight.vs", "res/weight.fs");
        m_WeightFaceLoc = m_WeightProgram->GetUniformLocation("glb_Face");
        m_WeightLightPatchWidthLoc = m_WeightProgram->GetUniformLocation("glb_LightPatchWidth");
        m_WeightLightPatchHeightLoc = m_WeightProgram->GetUniformLocation("glb_LightPatchHeight");

        m_NormalizeWeightProgram = render::shader::UserProgram::Create("res/normalizeWeight.vs", "res/normalizeWeight.fs");
        m_TotalWeightColorLoc = m_NormalizeWeightProgram->GetUniformLocation("glb_TotalColor");
        m_WeightMapLoc = m_NormalizeWeightProgram->GetUniformLocation("glb_WeightMap");

        m_DrawProgram = render::shader::UserProgram::Create("res/draw.vs", "res/draw.fs");
        m_DrawMVPLoc = m_DrawProgram->GetUniformLocation("glb_MVP");
        m_DrawLightMapLoc = m_DrawProgram->GetUniformLocation("glb_LightMap");

        // Create Mesh
        m_SceneMesh = scene::Model::Create(kSceneModelFile);
        m_EnvLightBallMesh = scene::Model::Create("res/envBall.obj");
        m_InDoorLightBallMesh = scene::Model::Create("res/sphereLight.obj");
        m_ScreenMesh = render::mesh::ScreenMesh::Create();

        // Create Light Map
        m_LightMap = render::texture::Texture::CreateFloat32Texture(kLightMapWidth, kLightMapHeight);

        // Create Camera
        m_Camera = scene::ModelCamera::Create(math::Vector(0.0f, 250.0f, 150.0f), math::Vector(0.0f, 0.0f, 0.0f));

        m_Proj.MakeProjectionMatrix(1.0f, 90.0f, 0.001f, 10000.0f);
        m_View = m_Camera->GetViewMatrix();

        // Setup light patch
        GLB_SAFE_ASSERT(m_ValidPatch.size() > 0);

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();

        // Control camera
        static float sRot = 0.0f;
        sRot = sRot + Input::GetMouseMoveX() * 0.1f;
        math::Vector pos = math::Vector(170.0f, 170.0f, 0.0);
        math::Matrix rotY;
        rotY.MakeRotateYMatrix(sRot);
        pos = rotY * pos;
        m_Camera->SetPos(pos);
        m_Camera->Update(1.0f);
        m_View = m_Camera->GetViewMatrix();

        // Normalize Weight Map Precompute
        static bool isGenerateHemi = false;
        if (!isGenerateHemi) {
            DrawWeightMap();

            CalcWeightMapColor();

            DrawNormalizeWeightMap();

            isGenerateHemi = true;
        }

        // Main loop for light map baking
        if (m_CurIterateNum < m_TotalIterateNum) {
            DrawHemiCube();

            CalcLightPatchColor();

            printf(">> Baking Light Map: %f\%\n", m_CurValidPatchIndex * 100.0f / m_ValidPatch.size());

            if (!FindNextValidLightPatch()) {
                m_CurIterateNum = m_CurIterateNum + 1;

                BuildLightMap();

                printf(">> Baking Light Map([%d]): OK", m_CurIterateNum);
                //MessageBox(NULL, L"Baking Light Map: OK", L"Message", MB_OK);
            }
        }

        // Draw Normal Scene
        DrawScene();

        // Reset render target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // SwapBuffers
        render::Device::SwapBuffer();

        GLB_CHECK_GL_ERROR;

        time.EndProfile();
        // printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
        GLB_SAFE_DELETE(m_Camera);
        GLB_SAFE_DELETE(m_SceneMesh);
        GLB_SAFE_DELETE(m_EnvLightBallMesh);
        GLB_SAFE_DELETE(m_InDoorLightBallMesh);
        GLB_SAFE_DELETE(m_ScreenMesh);
        GLB_SAFE_DELETE(m_LightMap);

        GLB_SAFE_DELETE(m_WeightRT);
        for (int32_t i = 0; i < 5; i++) {
            GLB_SAFE_DELETE(m_WeightMap[i]);
        }
        GLB_SAFE_DELETE(m_WeightProgram);

        GLB_SAFE_DELETE(m_NormalizeWeightRT);
        for (int32_t i = 0; i < 5; i++) {
            GLB_SAFE_DELETE(m_NormalizeWeightMap[i]);
        }
        GLB_SAFE_DELETE(m_NormalizeWeightProgram);

        GLB_SAFE_DELETE(m_LightPatchRT);
        for (int32_t i = 0; i < 5; i++) {
            GLB_SAFE_DELETE(m_LightPatchMap[i]);
        }
        GLB_SAFE_DELETE(m_LightPatchProgram);

        m_ValidPatch.clear();
    }

    void DrawWeightMap() {
        // Setup Shader
        render::Device::SetShader(m_WeightProgram);
        render::Device::SetShaderLayout(m_WeightProgram->GetShaderLayout());

        // Setup Viewport
        render::Device::SetViewport(0, 0, kLightPatchSize, kLightPatchSize);

        // Setup Render Target
        render::Device::SetRenderTarget(m_WeightRT);

        // Setup Vertex
        render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());
        render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());

        // Setup Texture
        render::Device::ClearTexture();

        render::DrawColorBuffer drawBuf[] = {
            render::COLORBUF_COLOR_ATTACHMENT0,
            render::COLORBUF_COLOR_ATTACHMENT1,
            render::COLORBUF_COLOR_ATTACHMENT2,
            render::COLORBUF_COLOR_ATTACHMENT3,
            render::COLORBUF_COLOR_ATTACHMENT4,
        };
        for (int32_t i = 0; i < 5; i++) {
            // Setup DrawBuffer
            render::Device::SetDrawColorBuffer(drawBuf[i]);

            // Clear
            render::Device::Clear(render::CLEAR_COLOR);

            // Setup Uniform
            render::Device::SetUniform1i(m_WeightFaceLoc, i);
            render::Device::SetUniform1i(m_WeightLightPatchWidthLoc, kLightPatchSize);
            render::Device::SetUniform1i(m_WeightLightPatchHeightLoc, kLightPatchSize);

            // Draw
            render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());
        }
    }

    void CalcWeightMapColor() {
        math::Vector color(0.0f, 0.0f, 0.0f);

        float* pixelData = new float[kLightPatchSize * kLightPatchSize * 4];

        for (int32_t i = 0; i < 5; i++) {
            m_WeightMap[i]->GetTextureData(pixelData);

            for (int32_t j = 0; j < kLightPatchSize; j++) {
                for (int32_t k = 0; k < kLightPatchSize; k++) {
                    color.x = color.x + pixelData[j * kLightPatchSize * 4 + k * 4 + 0];
                    color.y = color.y + pixelData[j * kLightPatchSize * 4 + k * 4 + 1];
                    color.z = color.z + pixelData[j * kLightPatchSize * 4 + k * 4 + 2];
                }
            }
        }

        delete[] pixelData;
        pixelData = NULL;

        m_TotalWeightColor = color;
    }

    void DrawNormalizeWeightMap() {
        // Setup Shader
        render::Device::SetShader(m_NormalizeWeightProgram);
        render::Device::SetShaderLayout(m_NormalizeWeightProgram->GetShaderLayout());

        // Setup Viewport
        render::Device::SetViewport(0, 0, kLightPatchSize, kLightPatchSize);

        // Setup Render Target
        render::Device::SetRenderTarget(m_NormalizeWeightRT);

        // Setup Vertex
        render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());
        render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());

        // Setup Texture
        render::Device::ClearTexture();
        for (int32_t i = 0; i < 5; i++) {
            render::Device::SetTexture(i, m_WeightMap[i], i);
        }

        render::DrawColorBuffer drawBuf[] = {
            render::COLORBUF_COLOR_ATTACHMENT0,
            render::COLORBUF_COLOR_ATTACHMENT1,
            render::COLORBUF_COLOR_ATTACHMENT2,
            render::COLORBUF_COLOR_ATTACHMENT3,
            render::COLORBUF_COLOR_ATTACHMENT4,
        };
        for (int32_t i = 0; i < 5; i++) {
            // Setup DrawBuffer
            render::Device::SetDrawColorBuffer(drawBuf[i]);

            // Clear
            render::Device::Clear(render::CLEAR_COLOR);

            // Setup Uniform
            render::Device::SetUniform3f(m_TotalWeightColorLoc, m_TotalWeightColor);
            render::Device::SetUniformSampler2D(m_WeightMapLoc, i);

            // Draw
            render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());
        }
    }

    void DrawHemiCube() {
        // Setup Camera
        // 5 View matrix(Right,Left,Up,Down,Front)
        math::Vector pos = m_ValidPatch[m_CurValidPatchIndex]->pos;
        math::Vector normal = -m_ValidPatch[m_CurValidPatchIndex]->normal;
        math::Vector right = math::Vector(0.0f, 0.0f, 0.0f);
        if (normal.y > 0.99999f) {
            right = math::Cross(math::Vector(0.0f, 0.0f, -1.0f), normal);
        } else if (normal.y < -0.99999f) {
            right = math::Cross(math::Vector(0.0f, 0.0f, 1.0f), normal);
        } else {
            right = math::Cross(math::Vector(0.0f, 1.0f, 0.0f), normal);
        }
        right.Normalize();
        math::Vector up = math::Cross(normal, right);
        up.Normalize();

        math::Matrix views[5];
        views[0].MakeViewMatrix(pos - normal * 0.001f, normal, up, -right);  // Right
        views[1].MakeViewMatrix(pos - normal * 0.001f, -normal, up, right);  // Left
        views[2].MakeViewMatrix(pos - normal * 0.001f, right, normal, -up);  // Up
        views[3].MakeViewMatrix(pos - normal * 0.001f, right, -normal, up);  // Down
        views[4].MakeViewMatrix(pos - normal * 0.001f, right, up, normal);  // Front

        // Setup Shader
        render::Device::SetShader(m_LightPatchProgram);
        render::Device::SetShaderLayout(m_LightPatchProgram->GetShaderLayout());

        // Setup Viewport
        render::Device::SetViewport(0, 0, kLightPatchSize, kLightPatchSize);

        // Setup Render Target
        render::Device::SetRenderTarget(m_LightPatchRT);

        // Setup Render State
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
        render::Device::SetClearDepth(1.0f);
        render::Device::SetDepthTestEnable(true);
        render::Device::SetCullFaceEnable(true);
        render::Device::SetCullFaceMode(render::CULL_BACK);

        // Setup Texture
        render::Device::ClearTexture();
        for (int32_t i = 0; i < 5; i++) {
            render::Device::SetTexture(i, m_NormalizeWeightMap[i], i);
        }
        render::Device::SetTexture(5, m_LightMap, 5);

        render::DrawColorBuffer drawBuf[] = {
            render::COLORBUF_COLOR_ATTACHMENT0,
            render::COLORBUF_COLOR_ATTACHMENT1,
            render::COLORBUF_COLOR_ATTACHMENT2,
            render::COLORBUF_COLOR_ATTACHMENT3,
            render::COLORBUF_COLOR_ATTACHMENT4,
        };
        for (int32_t i = 0; i < 5; i++) {
            // Setup DrawBuffer
            render::Device::SetDrawColorBuffer(drawBuf[i]);

            // Clear
            render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

            {  // Draw Scene
                // Setup Vertex
                render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexLayout());
                render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexBuffer());

                // Setup Uniform
                render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[i]);
                render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(0.0f, 0.0f, 0.0f));
                render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, i);
                render::Device::SetUniformSampler2D(m_LightMapLoc, 5);
                render::Device::SetUniform1i(m_LightPatchFaceLoc, i);

                // Draw
                render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexNum());
            }

            {  // Draw Env ball
                // Setup Vertex
                render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_EnvLightBallMesh->GetMeshId())->GetVertexLayout());
                render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_EnvLightBallMesh->GetMeshId())->GetVertexBuffer());

                // Setup Uniform
                math::Matrix world;
                world.MakeScaleMatrix(5000.0f, 5000.0f, 5000.0f);
                render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[i] * world);
                render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(6.0f, 6.0f, 3.0f));
                render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, i);
                render::Device::SetUniformSampler2D(m_LightMapLoc, 5);
                render::Device::SetUniform1i(m_LightPatchFaceLoc, i);

                // Draw
                render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_EnvLightBallMesh->GetMeshId())->GetVertexNum());
            }

            //{  // Draw InDoor ball
            //    // Right Light Ball
            //    // Setup Vertex
            //    render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexLayout());
            //    render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexBuffer());

            //    // Setup Uniform
            //    math::Matrix world;
            //    world.MakeScaleMatrix(0.5f, 0.5f, 0.5f);
            //    world.Translate(50.0f, 30.0f, 0.0f);
            //    render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[i] * world);
            //    render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(600.0f, 600.0f, 0.0f));
            //    render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, i);
            //    render::Device::SetUniformSampler2D(m_LightMapLoc, 5);
            //    render::Device::SetUniform1i(m_LightPatchFaceLoc, i);

            //    // Draw
            //    render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexNum());

            //    // Front Light Ball
            //    // Setup Vertex
            //    render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexLayout());
            //    render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexBuffer());

            //    // Setup Uniform
            //    world.MakeScaleMatrix(0.5f, 0.5f, 0.5f);
            //    world.Translate(0.0f, 30.0f, 50.0f);
            //    render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[i] * world);
            //    render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(600.0f, 0.0f, 0.0f));
            //    render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, i);
            //    render::Device::SetUniformSampler2D(m_LightMapLoc, 5);
            //    render::Device::SetUniform1i(m_LightPatchFaceLoc, i);

            //    // Draw
            //    render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexNum());

            //    // Back Light Ball
            //    // Setup Uniform
            //    world.MakeScaleMatrix(0.5f, 0.5f, 0.5f);
            //    world.Translate(0.0f, 30.0f, -50.0f);
            //    render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[i] * world);
            //    render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(0.0f, 600.0f, 0.0f));
            //    render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, i);
            //    render::Device::SetUniformSampler2D(m_LightMapLoc, 5);
            //    render::Device::SetUniform1i(m_LightPatchFaceLoc, i);

            //    // Draw
            //    render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexNum());

            //    // Left Light Ball
            //    // Setup Uniform
            //    world.MakeScaleMatrix(0.5f, 0.5f, 0.5f);
            //    world.Translate(-50.0f, 30.0f, 0.0f);
            //    render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[i] * world);
            //    render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(0.0f, 0.0f, 600.0f));
            //    render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, i);
            //    render::Device::SetUniformSampler2D(m_LightMapLoc, 5);
            //    render::Device::SetUniform1i(m_LightPatchFaceLoc, i);

            //    // Draw
            //    render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexNum());
            //}
        }
    }

    void CalcLightPatchColor() {
        math::Vector color(0.0f, 0.0f, 0.0f);
        float* pixelData = new float[4];

        int32_t maxLevel = log(max(kLightPatchSize, kLightPatchSize)) / log(2);

        for (int32_t i = 0; i < 5; i++) {
            m_LightPatchMap[i]->GenerateMipmap();
            m_LightPatchMap[i]->GetTextureData(pixelData, maxLevel);
            color.x = color.x + pixelData[0] * (kLightPatchSize * kLightPatchSize);
            color.y = color.y + pixelData[1] * (kLightPatchSize * kLightPatchSize);
            color.z = color.z + pixelData[2] * (kLightPatchSize * kLightPatchSize);
        }

        delete[] pixelData;
        pixelData = NULL;
        m_ValidPatch[m_CurValidPatchIndex]->color = color;
        // m_ValidPatch[m_CurValidPatchIndex]->color = color * (1.0f / (3.0f * kLightPatchSize * kLightPatchSize));
    }

    void BuildLightMap() {
        float* pixelData = new float[kLightMapWidth * kLightMapHeight * 4];

        // Flip map in vertical axis
        for (int32_t i = 0; i < kLightMapHeight; i++) {
            for (int32_t j = 0; j < kLightMapWidth; j++) {
                pixelData[(kLightMapHeight - 1 - i) * kLightMapWidth * 4 + j * 4 + 0] = m_Patch[i][j].color.x;
                pixelData[(kLightMapHeight - 1 - i) * kLightMapWidth * 4 + j * 4 + 1] = m_Patch[i][j].color.y;
                pixelData[(kLightMapHeight - 1 - i) * kLightMapWidth * 4 + j * 4 + 2] = m_Patch[i][j].color.z;
                pixelData[(kLightMapHeight - 1 - i) * kLightMapWidth * 4 + j * 4 + 3] = 1.0f;
            }
        }

        m_LightMap->UpdateTextureData(pixelData);
        m_LightMap->GenerateMipmap();

        delete[] pixelData;
        pixelData = NULL;
    }

    void DrawScene() {
        // Setup Render Target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // Setup Shader
        render::Device::SetShader(m_DrawProgram);
        render::Device::SetShaderLayout(m_DrawProgram->GetShaderLayout());

        // Setup Viewport
        render::Device::SetViewport(0, 0, 800, 600);

        // Setup Render State
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
        render::Device::SetClearDepth(1.0f);
        render::Device::SetDepthTestEnable(true);
        render::Device::SetCullFaceEnable(true);
        render::Device::SetCullFaceMode(render::CULL_BACK);

        // Setup Texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, m_LightMap, 0);

        // Clear
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        {  // Draw Scene
            // Setup Vertex
            render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexLayout());
            render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexBuffer());

            // Setup Uniform
            math::Matrix proj;
            proj.MakeProjectionMatrix(800.0f / 600.0f, 75.0f, 0.001f, 10000.0f);
            render::Device::SetUniformMatrix(m_DrawMVPLoc, proj * m_View);
            render::Device::SetUniformSampler2D(m_DrawLightMapLoc, 0);

            // Draw
            render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexNum());
        }
    }

protected:
    int32_t                         m_CurIterateNum;
    int32_t                         m_TotalIterateNum;
    int32_t                         m_CurValidPatchIndex;

    render::texture::Texture*       m_LightMap;

    static const int32_t kLightMapWidth = 512;
    static const int32_t kLightMapHeight = 512;
    struct LightPatch {
        math::Vector uv;
        math::Vector pos;
        math::Vector normal;
        math::Vector color;
        bool valid;  // Some of light patches is not valid, because model's UV coordinates may not cover all range of uv.
    };
    LightPatch m_Patch[kLightMapHeight][kLightMapWidth];
    std::vector<LightPatch*> m_ValidPatch;

    scene::CameraBase*              m_Camera;
    math::Matrix                    m_Proj;
    math::Matrix                    m_View;
    scene::Model*                   m_SceneMesh;
    scene::Model*                   m_EnvLightBallMesh;
    scene::Model*                   m_InDoorLightBallMesh;
    render::mesh::ScreenMesh*       m_ScreenMesh;

    int32_t                         m_CurLightMapW;
    int32_t                         m_CurLightMapH;

    static const int32_t kLightPatchSize = 1024; // At least 128*128

    render::RenderTarget*           m_WeightRT;
    render::texture::Texture*       m_WeightMap[5];
    render::shader::UserProgram*    m_WeightProgram;
    int32_t                         m_WeightFaceLoc;
    int32_t                         m_WeightLightPatchWidthLoc;
    int32_t                         m_WeightLightPatchHeightLoc;

    math::Vector                    m_TotalWeightColor;

    render::RenderTarget*           m_NormalizeWeightRT;
    render::texture::Texture*       m_NormalizeWeightMap[5];
    render::shader::UserProgram*    m_NormalizeWeightProgram;
    int32_t                         m_TotalWeightColorLoc;
    int32_t                         m_WeightMapLoc;

    render::RenderTarget*           m_LightPatchRT;
    render::texture::Texture*       m_LightPatchMap[5];
    render::shader::UserProgram*    m_LightPatchProgram;
    int32_t                         m_LightPatchMVPLoc;
    int32_t                         m_LightPatchLightValueLoc;
    int32_t                         m_NormlaizeWeightMapLoc;
    int32_t                         m_LightMapLoc;
    int32_t                         m_LightPatchFaceLoc;

    render::shader::UserProgram*    m_DrawProgram;
    int32_t                         m_DrawMVPLoc;
    int32_t                         m_DrawLightMapLoc;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
#if _DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
#endif

    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_lightmapper", sizeof(L"glb_lightmapper"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationLightMapper::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

#if _DEBUG
    FreeConsole();
#endif

    return 0;
}