//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 05 / 19
// Brief: Implement Radiosity Normal Mapping
//----------------------------------------------------------------------
#include "glb_rnm.h"
#include "resource.h"

#include <fstream>

static const char* kSceneModelFile = "res/plane.obj";

class ApplicationRadiosityNormalMap : public glb::app::ApplicationBase {
public:
    ApplicationRadiosityNormalMap()
    : m_CurIterateNum(0)
    , m_TotalIterateNum(1)
    , m_CurValidPatchIndex(0)
    , m_NormalMap(NULL)
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
    , m_LightPatchProgram(NULL)
    , m_LightPatchMVPLoc(-1)
    , m_LightPatchLightValueLoc(-1)
    , m_NormlaizeWeightMapLoc(-1)
    , m_LightPatchNormalMapLoc(-1)
    , m_LightPatchFaceLoc(-1)
    , m_DrawProgram(NULL)
    , m_DrawMVPLoc(-1)
    , m_DrawNormalMapLoc(-1) {
        for (int32_t j = 0; j < 5; j++) {
            m_WeightMap[j] = NULL;
            m_LightPatchRT[j] = NULL;
        }

        for (int32_t i = 0; i < 3; i++) {
            m_LightMap[i] = NULL;
            m_LightMapLoc[i] = -1;
            m_DrawLightMapLoc[i] = -1;
            for (int32_t j = 0; j < 5; j++) {
                m_LightPatchMap[i][j] = NULL;
            }
        }
        m_ValidPatch.clear();
    }
    virtual~ApplicationRadiosityNormalMap() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationRadiosityNormalMap;
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
                math::Vector tangent;
                math::Vector binormal;
            } vertex[3];
        };
        std::vector<Face> faces;
        faces.clear();

        float* vertexBuf = NULL;
        float* texBuf = NULL;
        float* lightMapTexBuf = NULL;
        float* normalBuf = NULL;
        float* tangentBuf = NULL;
        float* binormalBuf = NULL;
        math::Vector boundBoxMax(0.0f, 0.0f, 0.0f);
        math::Vector boundBoxMin(0.0f, 0.0f, 0.0f);
        int32_t faceNum = util::MeshReader::ExtractModelData(kSceneModelFile, boundBoxMin, boundBoxMax, &vertexBuf, &texBuf, &lightMapTexBuf, &normalBuf, &tangentBuf, &binormalBuf);

        int32_t vertexOffset = 0, uvOffset = 0, normalOffset = 0, tangentOffset = 0, binormalOffset = 0;
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
                face.vertex[j].tangent.x = tangentBuf[tangentOffset++];
                face.vertex[j].tangent.y = tangentBuf[tangentOffset++];
                face.vertex[j].tangent.z = tangentBuf[tangentOffset++];
                face.vertex[j].tangent.w = 0.0f;
                face.vertex[j].binormal.x = binormalBuf[binormalOffset++];
                face.vertex[j].binormal.y = binormalBuf[binormalOffset++];
                face.vertex[j].binormal.z = binormalBuf[binormalOffset++];
                face.vertex[j].binormal.w = 0.0f;
            }

            faces.push_back(face);
        }

        util::MeshReader::RelaseBuf(&vertexBuf, &texBuf, &normalBuf, &tangentBuf, &binormalBuf);

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
                    m_Patch[h][w].valid = true;

                    //math::Vector tangent = faces[i].vertex[0].tangent * lambda0 + faces[i].vertex[1].tangent * lambda1 + faces[i].vertex[2].tangent * lambda2;
                    //math::Vector binormal = faces[i].vertex[0].binormal * lambda0 + faces[i].vertex[1].binormal * lambda1 + faces[i].vertex[2].binormal * lambda2;
                    //math::Vector normal = faces[i].vertex[0].normal * lambda0 + faces[i].vertex[1].normal * lambda1 + faces[i].vertex[2].normal * lambda2;
                    //tangent.Normalize();
                    //binormal.Normalize();
                    //normal.Normalize();
                    math::Vector tangent = faces[i].vertex[0].tangent;
                    math::Vector binormal = faces[i].vertex[0].binormal;
                    math::Vector normal = math::Cross(tangent, binormal);

                    math::Matrix tbn;
                    tbn.MakeIdentityMatrix();
                    tbn.m_Matrix.m[0][0] = tangent.x;
                    tbn.m_Matrix.m[1][0] = tangent.y;
                    tbn.m_Matrix.m[2][0] = tangent.z;
                    tbn.m_Matrix.m[0][1] = binormal.x;
                    tbn.m_Matrix.m[1][1] = binormal.y;
                    tbn.m_Matrix.m[2][1] = binormal.z;
                    tbn.m_Matrix.m[0][2] = normal.x;
                    tbn.m_Matrix.m[1][2] = normal.y;
                    tbn.m_Matrix.m[2][2] = normal.z;

                    m_Patch[h][w].bais[0] = tbn * math::Vector(sqrt(2.0f / 3.0f), 0.0f, sqrt(1.0f / 3.0f), 0.0f);
                    m_Patch[h][w].bais[1] = tbn * math::Vector(-sqrt(1.0f / 6.0f), sqrt(1.0f / 2.0f), sqrt(1.0f / 3.0f), 0.0f);
                    m_Patch[h][w].bais[2] = tbn * math::Vector(-sqrt(1.0f / 6.0f), -sqrt(1.0f / 2.0f), sqrt(1.0f / 3.0f), 0.0f);
                    m_Patch[h][w].bais[0].Normalize();
                    m_Patch[h][w].bais[1].Normalize();
                    m_Patch[h][w].bais[2].Normalize();

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
        if (m_CurValidPatchIndex >= static_cast<int32_t>(m_ValidPatch.size())) {
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
        for (int32_t i = 0; i < 3; i++) {
            m_LightPatchRT[i] = render::RenderTarget::Create(kLightPatchSize, kLightPatchSize);
            for (int32_t j = 0; j < 5; j++) {
                m_LightPatchMap[i][j] = render::texture::Texture::CreateFloat32Texture(kLightPatchSize, kLightPatchSize);
                m_LightPatchRT[i]->AttachColorTexture(drawBuf[j], m_LightPatchMap[i][j]);
            }
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
        m_LightMapLoc[0] = m_LightPatchProgram->GetUniformLocation("glb_LightMap[0]");
        m_LightMapLoc[1] = m_LightPatchProgram->GetUniformLocation("glb_LightMap[1]");
        m_LightMapLoc[2] = m_LightPatchProgram->GetUniformLocation("glb_LightMap[2]");
        m_LightPatchNormalMapLoc = m_LightPatchProgram->GetUniformLocation("glb_NormalMap");
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
        m_DrawLightMapLoc[0] = m_DrawProgram->GetUniformLocation("glb_LightMap[0]");
        m_DrawLightMapLoc[1] = m_DrawProgram->GetUniformLocation("glb_LightMap[1]");
        m_DrawLightMapLoc[2] = m_DrawProgram->GetUniformLocation("glb_LightMap[2]");
        m_DrawNormalMapLoc = m_DrawProgram->GetUniformLocation("glb_NormalMap");

        // Create Mesh
        m_SceneMesh = render::mesh::Mgr::GetMeshById(render::mesh::Mgr::AddMesh(kSceneModelFile));
        m_EnvLightBallMesh = render::mesh::Mgr::GetMeshById(render::mesh::Mgr::AddMesh("res/envBall.obj"));
        m_InDoorLightBallMesh = render::mesh::Mgr::GetMeshById(render::mesh::Mgr::AddMesh("res/sphereLight.obj"));
        m_ScreenMesh = render::mesh::ScreenMesh::Create();

        // Create Light Map
        m_LightMap[0] = render::texture::Texture::CreateFloat32Texture(kLightMapWidth, kLightMapHeight);
        m_LightMap[1] = render::texture::Texture::CreateFloat32Texture(kLightMapWidth, kLightMapHeight);
        m_LightMap[2] = render::texture::Texture::CreateFloat32Texture(kLightMapWidth, kLightMapHeight);
        m_NormalMap = render::texture::Texture::Create("res/plane_DefaultMaterial_normal_gl.bmp");
        //m_NormalMap = render::texture::Texture::Create("res/scene_Material_normal_gl.bmp");

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
        static float sDist = 0.0f;
        sDist = sDist + Input::GetMouseMoveY() * 0.05f;
        math::Vector pos = math::Vector(10.0f, 10.0f, 0.0) + math::Vector(1.0f, 1.0f, 0.0f) * sDist;
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

                if (m_CurIterateNum >= m_TotalIterateNum) {
                    m_LightMap[0]->Save("res/lightmap0.hdr", util::TFT_HDR);
                    m_LightMap[1]->Save("res/lightmap1.hdr", util::TFT_HDR);
                    m_LightMap[2]->Save("res/lightmap2.hdr", util::TFT_HDR);
                }
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
        GLB_SAFE_DELETE(m_NormalMap);
        GLB_SAFE_DELETE(m_ScreenMesh);

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

        for (int32_t i = 0; i < 3; i++) {
            GLB_SAFE_DELETE(m_LightMap[i]);
            GLB_SAFE_DELETE(m_LightPatchRT[i]);
            for (int32_t j = 0; j < 5; j++) {
                GLB_SAFE_DELETE(m_LightPatchMap[i][j]);
            }
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
        // For each bais
        for (int32_t i = 0; i < 3; i++) {
            // Setup Camera
            // 5 View matrix(Right,Left,Up,Down,Front)
            math::Vector pos = m_ValidPatch[m_CurValidPatchIndex]->pos;
            math::Vector normal = -m_ValidPatch[m_CurValidPatchIndex]->bais[i];
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
            render::Device::SetRenderTarget(m_LightPatchRT[i]);

            // Setup Render State
            render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
            render::Device::SetClearDepth(1.0f);
            render::Device::SetDepthTestEnable(true);
            render::Device::SetCullFaceEnable(true);
            render::Device::SetCullFaceMode(render::CULL_BACK);

            // Setup Texture
            render::Device::ClearTexture();
            for (int32_t j = 0; j < 5; j++) {
                render::Device::SetTexture(j, m_NormalizeWeightMap[j], j);
            }
            render::Device::SetTexture(5, m_LightMap[0], 5);
            render::Device::SetTexture(6, m_LightMap[1], 6);
            render::Device::SetTexture(7, m_LightMap[2], 7);
            render::Device::SetTexture(8, m_NormalMap, 8);

            render::DrawColorBuffer drawBuf[] = {
                render::COLORBUF_COLOR_ATTACHMENT0,
                render::COLORBUF_COLOR_ATTACHMENT1,
                render::COLORBUF_COLOR_ATTACHMENT2,
                render::COLORBUF_COLOR_ATTACHMENT3,
                render::COLORBUF_COLOR_ATTACHMENT4,
            };
            for (int32_t j = 0; j < 5; j++) {
                // Setup DrawBuffer
                render::Device::SetDrawColorBuffer(drawBuf[j]);

                // Clear
                render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

                {  // Draw Scene
                    // Setup Vertex
                    render::Device::SetVertexLayout(m_SceneMesh->GetVertexLayout());
                    render::Device::SetVertexBuffer(m_SceneMesh->GetVertexBuffer());

                    // Setup Uniform
                    math::Matrix world;
                    world.MakeIdentityMatrix();
                    world.Inverse();
                    world.Transpose();
                    render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[j]);
                    render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(0.0f, 0.0f, 0.0f));
                    render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, j);
                    render::Device::SetUniformSampler2D(m_LightMapLoc[0], 5);
                    render::Device::SetUniformSampler2D(m_LightMapLoc[1], 6);
                    render::Device::SetUniformSampler2D(m_LightMapLoc[2], 7);
                    render::Device::SetUniformSampler2D(m_LightPatchNormalMapLoc, 8);
                    render::Device::SetUniform1i(m_LightPatchFaceLoc, j);

                    // Draw
                    render::Device::Draw(render::PT_TRIANGLES, 0, m_SceneMesh->GetVertexNum());
                }

                //{  // Draw Env ball
                //    // Setup Vertex
                //    render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_EnvLightBallMesh->GetMeshId())->GetVertexLayout());
                //    render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_EnvLightBallMesh->GetMeshId())->GetVertexBuffer());

                //    // Setup Uniform
                //    math::Matrix world;
                //    world.MakeScaleMatrix(5000.0f, 5000.0f, 5000.0f);
                //    render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[j] * world);
                //    render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(6.0f, 6.0f, 3.0f));
                //    render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, j);
                //    render::Device::SetUniformSampler2D(m_LightMapLoc[0], 5);
                //    render::Device::SetUniformSampler2D(m_LightMapLoc[1], 6);
                //    render::Device::SetUniformSampler2D(m_LightMapLoc[2], 7);
                //    render::Device::SetUniformSampler2D(m_LightPatchNormalMapLoc, 8);
                //    render::Device::SetUniform1i(m_LightPatchFaceLoc, j);

                //    // Draw
                //    render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_EnvLightBallMesh->GetMeshId())->GetVertexNum());
                //}

                {  // Draw InDoor ball
                    // Right Light Ball
                    // Setup Vertex
                    render::Device::SetVertexLayout(m_InDoorLightBallMesh->GetVertexLayout());
                    render::Device::SetVertexBuffer(m_InDoorLightBallMesh->GetVertexBuffer());

                    // Setup Uniform
                    math::Matrix world;
                    world.MakeScaleMatrix(0.1f, 0.1f, 0.1f);
                    world.Translate(0.0f, 1.0f, 0.0f);
                    render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[j] * world);
                    render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(300.0f, 300.0f, 300.0f));
                    render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, j);
                    render::Device::SetUniformSampler2D(m_LightMapLoc[0], 5);
                    render::Device::SetUniformSampler2D(m_LightMapLoc[1], 6);
                    render::Device::SetUniformSampler2D(m_LightMapLoc[2], 7);
                    render::Device::SetUniformSampler2D(m_LightPatchNormalMapLoc, 8);
                    render::Device::SetUniform1i(m_LightPatchFaceLoc, j);

                    // Draw
                    render::Device::Draw(render::PT_TRIANGLES, 0, m_InDoorLightBallMesh->GetVertexNum());

                    //// Front Light Ball
                    //// Setup Vertex
                    //render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexLayout());
                    //render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexBuffer());

                    //// Setup Uniform
                    //world.MakeScaleMatrix(0.5f, 0.5f, 0.5f);
                    //world.Translate(0.0f, 30.0f, 50.0f);
                    //render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[j] * world);
                    //render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(600.0f, 0.0f, 0.0f));
                    //render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, j);
                    //render::Device::SetUniformSampler2D(m_LightMapLoc[0], 5);
                    //render::Device::SetUniformSampler2D(m_LightMapLoc[1], 6);
                    //render::Device::SetUniformSampler2D(m_LightMapLoc[2], 7);
                    //render::Device::SetUniformSampler2D(m_LightPatchNormalMapLoc, 8);
                    //render::Device::SetUniform1i(m_LightPatchFaceLoc, j);

                    //// Draw
                    //render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexNum());

                    //// Back Light Ball
                    //// Setup Uniform
                    //world.MakeScaleMatrix(0.5f, 0.5f, 0.5f);
                    //world.Translate(0.0f, 30.0f, -50.0f);
                    //render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[j] * world);
                    //render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(0.0f, 600.0f, 0.0f));
                    //render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, j);
                    //render::Device::SetUniformSampler2D(m_LightMapLoc[0], 5);
                    //render::Device::SetUniformSampler2D(m_LightMapLoc[1], 6);
                    //render::Device::SetUniformSampler2D(m_LightMapLoc[2], 7);
                    //render::Device::SetUniformSampler2D(m_LightPatchNormalMapLoc, 8);
                    //render::Device::SetUniform1i(m_LightPatchFaceLoc, j);

                    //// Draw
                    //render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexNum());

                    //// Left Light Ball
                    //// Setup Uniform
                    //world.MakeScaleMatrix(0.5f, 0.5f, 0.5f);
                    //world.Translate(-50.0f, 30.0f, 0.0f);
                    //render::Device::SetUniformMatrix(m_LightPatchMVPLoc, m_Proj * views[j] * world);
                    //render::Device::SetUniform3f(m_LightPatchLightValueLoc, math::Vector(0.0f, 0.0f, 600.0f));
                    //render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, j);
                    //render::Device::SetUniformSampler2D(m_LightMapLoc[0], 5);
                    //render::Device::SetUniformSampler2D(m_LightMapLoc[1], 6);
                    //render::Device::SetUniformSampler2D(m_LightMapLoc[2], 7);
                    //render::Device::SetUniformSampler2D(m_LightPatchNormalMapLoc, 8);
                    //render::Device::SetUniform1i(m_LightPatchFaceLoc, j);

                    //// Draw
                    //render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_InDoorLightBallMesh->GetMeshId())->GetVertexNum());
                }
            }
        }
    }

    void CalcLightPatchColor() {
        for (int32_t i = 0; i < 3; i++) {
            math::Vector color(0.0f, 0.0f, 0.0f);
            float* pixelData = new float[4];

            int32_t maxLevel = static_cast<int32_t>(log(max(kLightPatchSize, kLightPatchSize)) / log(2));

            for (int32_t j = 0; j < 5; j++) {
                m_LightPatchMap[i][j]->GenerateMipmap();
                m_LightPatchMap[i][j]->GetTextureData(pixelData, maxLevel);
                color.x = color.x + pixelData[0] * (kLightPatchSize * kLightPatchSize);
                color.y = color.y + pixelData[1] * (kLightPatchSize * kLightPatchSize);
                color.z = color.z + pixelData[2] * (kLightPatchSize * kLightPatchSize);
            }

            delete[] pixelData;
            pixelData = NULL;
            m_ValidPatch[m_CurValidPatchIndex]->color[i] = color;
            // m_ValidPatch[m_CurValidPatchIndex]->color = color * (1.0f / (3.0f * kLightPatchSize * kLightPatchSize));
        }
    }

    void BuildLightMap() {
        for (int32_t m = 0; m < 3; m++) {
            float* pixelData = new float[kLightMapWidth * kLightMapHeight * 4];

            // Flip map in vertical axis
            for (int32_t i = 0; i < kLightMapHeight; i++) {
                for (int32_t j = 0; j < kLightMapWidth; j++) {
                    pixelData[(kLightMapHeight - 1 - i) * kLightMapWidth * 4 + j * 4 + 0] = m_Patch[i][j].color[m].x;
                    pixelData[(kLightMapHeight - 1 - i) * kLightMapWidth * 4 + j * 4 + 1] = m_Patch[i][j].color[m].y;
                    pixelData[(kLightMapHeight - 1 - i) * kLightMapWidth * 4 + j * 4 + 2] = m_Patch[i][j].color[m].z;
                    pixelData[(kLightMapHeight - 1 - i) * kLightMapWidth * 4 + j * 4 + 3] = 1.0f;
                }
            }

            m_LightMap[m]->UpdateTextureData(pixelData);
            m_LightMap[m]->GenerateMipmap();

            delete[] pixelData;
            pixelData = NULL;
        }
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
        render::Device::SetTexture(0, m_LightMap[0], 0);
        render::Device::SetTexture(1, m_LightMap[1], 1);
        render::Device::SetTexture(2, m_LightMap[2], 2);
        render::Device::SetTexture(3, m_NormalMap, 3);

        // Clear
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        {  // Draw Scene
            // Setup Vertex
            render::Device::SetVertexLayout(m_SceneMesh->GetVertexLayout());
            render::Device::SetVertexBuffer(m_SceneMesh->GetVertexBuffer());

            // Setup Uniform
            math::Matrix proj;
            proj.MakeProjectionMatrix(800.0f / 600.0f, 75.0f, 0.001f, 10000.0f);
            render::Device::SetUniformMatrix(m_DrawMVPLoc, proj * m_View);
            render::Device::SetUniformSampler2D(m_DrawLightMapLoc[0], 0);
            render::Device::SetUniformSampler2D(m_DrawLightMapLoc[1], 1);
            render::Device::SetUniformSampler2D(m_DrawLightMapLoc[2], 2);
            render::Device::SetUniformSampler2D(m_DrawNormalMapLoc, 3);

            // Draw
            render::Device::Draw(render::PT_TRIANGLES, 0, m_SceneMesh->GetVertexNum());
        }
    }

protected:
    int32_t                         m_CurIterateNum;
    int32_t                         m_TotalIterateNum;
    int32_t                         m_CurValidPatchIndex;

    render::texture::Texture*       m_LightMap[3];
    render::texture::Texture*       m_NormalMap;

    static const int32_t kLightMapWidth = 128;
    static const int32_t kLightMapHeight = 128;
    struct LightPatch {
        math::Vector uv;
        math::Vector pos;
        math::Vector bais[3];
        math::Vector color[3];
        bool valid;  // Some of light patches is not valid, because model's UV coordinates may not cover all range of uv.
    };
    LightPatch m_Patch[kLightMapHeight][kLightMapWidth];
    std::vector<LightPatch*> m_ValidPatch;

    scene::CameraBase*              m_Camera;
    math::Matrix                    m_Proj;
    math::Matrix                    m_View;
    render::mesh::MeshBase*         m_SceneMesh;
    render::mesh::MeshBase*         m_EnvLightBallMesh;
    render::mesh::MeshBase*         m_InDoorLightBallMesh;
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

    render::RenderTarget*           m_LightPatchRT[3];
    render::texture::Texture*       m_LightPatchMap[3][5];
    render::shader::UserProgram*    m_LightPatchProgram;
    int32_t                         m_LightPatchMVPLoc;
    int32_t                         m_LightPatchLightValueLoc;
    int32_t                         m_NormlaizeWeightMapLoc;
    int32_t                         m_LightMapLoc[3];
    int32_t                         m_LightPatchNormalMapLoc;
    int32_t                         m_LightPatchFaceLoc;

    render::shader::UserProgram*    m_DrawProgram;
    int32_t                         m_DrawMVPLoc;
    int32_t                         m_DrawLightMapLoc[3];
    int32_t                         m_DrawNormalMapLoc;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
#if _DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
#endif

    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_radiositynormalmap", sizeof(L"glb_radiositynormalmap"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationRadiosityNormalMap::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

#if _DEBUG
    FreeConsole();
#endif

    return 0;
}