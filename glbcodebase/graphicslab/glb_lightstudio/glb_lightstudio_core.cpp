//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 07
// Brief: Core routine for light studio
//------------------------------------------------------------------
#include "glb_lightstudio_core.h"

#include "GL/glew.h"

using namespace glb;

static const int32_t kLightPatchSize = 1024; // At least 128*128
ApplicationCore* ApplicationCore::s_Instance = NULL;
int ApplicationCore::sIdGen = 0;

ApplicationCore::ApplicationCore()
: m_Camera(NULL)
, m_SceneMesh(NULL)
, m_ScreenMesh(NULL)
, m_SceneProgram(NULL)
, m_LightMapWidth(128)
, m_LightMapHeight(128)
, m_TotalBakeIterate(1)
, m_CurBakeIterate(0)
, m_EnableBake(false)
, m_CurValidPatchIndex(0)
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
, m_Patch(NULL)
, m_LightPatchSceneProgram(NULL)
, m_LightPatchLightProgram(NULL)
, m_LightPatchMVPLoc(-1)
, m_NormlaizeWeightMapLoc(-1)
, m_LightPatchAlbedoMapLoc(-1)
, m_LightPatchNormalMapLoc(-1)
, m_LightPatchFaceLoc(-1) {
    s_Instance = this;
    memset(m_LightMap, 0, sizeof(m_LightMap));
    memset(m_SceneMeshName, 0, sizeof(m_SceneMeshName));
    m_LightSource.clear();
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
    // Turn off VSync
    render::Device::SetupVSync(false);

    // Create Shader
    m_SceneProgram = render::shader::UserProgram::Create("res/draw.vs", "res/draw.fs");

    m_LightProgram = render::shader::UserProgram::Create("res/light.vs", "res/light.fs");

    m_WeightProgram = render::shader::UserProgram::Create("res/weight.vs", "res/weight.fs");
    m_WeightFaceLoc = m_WeightProgram->GetUniformLocation("glb_Face");
    m_WeightLightPatchWidthLoc = m_WeightProgram->GetUniformLocation("glb_LightPatchWidth");
    m_WeightLightPatchHeightLoc = m_WeightProgram->GetUniformLocation("glb_LightPatchHeight");

    m_NormalizeWeightProgram = render::shader::UserProgram::Create("res/normalizeWeight.vs", "res/normalizeWeight.fs");
    m_TotalWeightColorLoc = m_NormalizeWeightProgram->GetUniformLocation("glb_TotalColor");
    m_WeightMapLoc = m_NormalizeWeightProgram->GetUniformLocation("glb_WeightMap");

    m_LightPatchSceneProgram = render::shader::UserProgram::Create("res/lightPatch.vs", "res/lightPatchScene.fs");
    m_LightPatchMVPLoc = m_LightPatchSceneProgram->GetUniformLocation("glb_MVP");
    m_NormlaizeWeightMapLoc = m_LightPatchSceneProgram->GetUniformLocation("glb_NormalizeWeightMap");
    m_LightMapLoc[0] = m_LightPatchSceneProgram->GetUniformLocation("glb_LightMap[0]");
    m_LightMapLoc[1] = m_LightPatchSceneProgram->GetUniformLocation("glb_LightMap[1]");
    m_LightMapLoc[2] = m_LightPatchSceneProgram->GetUniformLocation("glb_LightMap[2]");
    m_LightPatchAlbedoMapLoc = m_LightPatchSceneProgram->GetUniformLocation("glb_AlbedoMap");
    m_LightPatchNormalMapLoc = m_LightPatchSceneProgram->GetUniformLocation("glb_NormalMap");
    m_LightPatchFaceLoc = m_LightPatchSceneProgram->GetUniformLocation("glb_Face");

    m_LightPatchLightProgram = render::shader::UserProgram::Create("res/lightPatch.vs", "res/lightPatchLight.fs");

    // Create Camera
    m_Camera = scene::ModelCamera::Create(math::Vector(0.0f, 250.0f, 150.0f), math::Vector(0.0f, 0.0f, 0.0f));

    // Create Screen Mesh
    m_ScreenMesh = render::mesh::ScreenMesh::Create();

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

    return true;
}

void ApplicationCore::Update(float dt) {
    util::ProfileTime time;
    time.BeginProfile();

    Input::Update();

    static bool sPreGenerateOK = false;
    if (!sPreGenerateOK) {
        PreGenerate();
        sPreGenerateOK = true;
    }

    UpdateCamera();
    BakeLightMap();
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
    GLB_SAFE_DELETE(m_Camera);

    for (LightSourceArray::iterator it = m_LightSource.begin(); it != m_LightSource.end(); ++it) {
        GLB_SAFE_DELETE(it->second.obj);
    }
    m_LightSource.clear();

    GLB_SAFE_DELETE(m_SceneProgram);
    GLB_SAFE_DELETE(m_SceneMesh);

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
        GLB_SAFE_DELETE(m_LightPatchRT[i]);
        for (int32_t j = 0; j < 5; j++) {
            GLB_SAFE_DELETE(m_LightPatchMap[i][j]);
        }
    }
    GLB_SAFE_DELETE(m_LightPatchSceneProgram);
    GLB_SAFE_DELETE(m_LightPatchLightProgram);
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
        memcpy(m_SceneMeshName, name, strlen(name));
        m_SceneMeshName[strlen(name)] = '\0';
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return true;
}

int ApplicationCore::AddLightMesh(int queryID, const char* name) {
    int result = -1;

    scene::Object* light = scene::Object::Create(name);

    if (light) {
        LightSourceEntry entry;
        entry.obj = light;
        entry.id  = queryID != -1 ? queryID : sIdGen++;
        m_LightSource.insert(std::pair<int, LightSourceEntry>(entry.id, entry));

        result = entry.id;

        if (queryID != -1 && queryID >= sIdGen) {
            sIdGen = queryID + 1;
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void ApplicationCore::ChangeLightMapSize(int width, int height) {
    if (width != m_LightMapWidth || height != m_LightMapHeight) {
        m_LightMapWidth = width;
        m_LightMapHeight = height;

        GLB_SAFE_DELETE(m_LightMap[0]);
        GLB_SAFE_DELETE(m_LightMap[1]);
        GLB_SAFE_DELETE(m_LightMap[2]);

        m_LightMap[0] = render::texture::Texture::CreateFloat32Texture(m_LightMapWidth, m_LightMapHeight);
        m_LightMap[1] = render::texture::Texture::CreateFloat32Texture(m_LightMapWidth, m_LightMapHeight);
        m_LightMap[2] = render::texture::Texture::CreateFloat32Texture(m_LightMapWidth, m_LightMapHeight);
    }
}

void ApplicationCore::SetBakeIterate(int iterate) {
    m_TotalBakeIterate = iterate;
}

void ApplicationCore::Bake() {
    m_EnableBake = true;
    PrepareBake();
}

void ApplicationCore::CancelBake() {
    m_EnableBake = false;
    ResetLightPatch();
}

float ApplicationCore::GetCurProgress() {
    float total = 1.0f * m_ValidPatch.size() * m_TotalBakeIterate;
    float cur = 1.0f * (m_ValidPatch.size() * m_CurBakeIterate + m_CurValidPatchIndex);
    return cur / total;
}

bool ApplicationCore::IsBaking() const {
    return m_EnableBake;
}

void ApplicationCore::SetLightSourcePos(int id, float px, float py, float pz) {
    LightSourceArray::iterator it = m_LightSource.find(id);
    if (it != m_LightSource.end()) {
        it->second.obj->SetPos(math::Vector(px, py, pz));
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ApplicationCore::SetLightSourceRot(int id, float rx, float ry, float rz) {
    LightSourceArray::iterator it = m_LightSource.find(id);
    if (it != m_LightSource.end()) {
        it->second.obj->SetRotation(math::Vector(rx, ry, rz));
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ApplicationCore::SetLightSourceScale(int id, float sx, float sy, float sz) {
    LightSourceArray::iterator it = m_LightSource.find(id);
    if (it != m_LightSource.end()) {
        it->second.obj->SetScale(math::Vector(sx, sy, sz));
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ApplicationCore::SetLightSourceColor(int id, float cx, float cy, float cz) {
    LightSourceArray::iterator it = m_LightSource.find(id);
    if (it != m_LightSource.end()) {
        it->second.color = math::Vector(cx, cy, cz);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ApplicationCore::SaveLightMap(const char* path) {
    std::string pathString = std::string(path);
    int32_t index = pathString.find_last_of(".");
    std::string lightMap0Path = pathString;
    lightMap0Path.insert(index, "0");
    std::string lightMap1Path = pathString;
    lightMap1Path.insert(index, "1");
    std::string lightMap2Path = pathString;
    lightMap2Path.insert(index, "2");
    m_LightMap[0]->Save(lightMap0Path.c_str(), util::TFT_HDR);
    m_LightMap[1]->Save(lightMap1Path.c_str(), util::TFT_HDR);
    m_LightMap[2]->Save(lightMap2Path.c_str(), util::TFT_HDR);
}

void ApplicationCore::PreGenerate() {
    DrawWeightMap();

    CalcWeightMapColor();

    DrawNormalizeWeightMap();
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

void ApplicationCore::BakeLightMap() {
    if (m_EnableBake) {
        if (m_CurBakeIterate < m_TotalBakeIterate) {
            DrawHemiCube();

            CalcLightPatchColor();

            if (!FindNextValidLightPatch()) {
                m_CurBakeIterate = m_CurBakeIterate + 1;

                BuildLightMap();

                if (m_CurBakeIterate >= m_TotalBakeIterate) {
                    m_EnableBake = false;
                }
            }
        }
    }
}

void ApplicationCore::DrawScene() {
    // Check if need draw scene
    if (!m_SceneMesh) return;

    // Setup Render Target
    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

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
        // Setup Shader
        render::Device::SetShader(m_SceneProgram);
        render::Device::SetShaderLayout(m_SceneProgram->GetShaderLayout());

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

    {  // Draw Light
        for (LightSourceArray::iterator it = m_LightSource.begin(); it != m_LightSource.end(); ++it) {
            scene::Model* lightModel = it->second.obj->GetModel();
            math::Vector color = it->second.color;

            // Setup Shader
            render::Device::SetShader(m_LightProgram);
            render::Device::SetShaderLayout(m_LightProgram->GetShaderLayout());

            // Setup Vertex
            render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(lightModel->GetMeshId())->GetVertexLayout());
            render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(lightModel->GetMeshId())->GetVertexBuffer());

            // Setup Uniform
            math::Matrix proj;
            proj.MakeProjectionMatrix(app::Application::GetWindowWidth() * 1.0f / app::Application::GetWindowHeight(), 75.0f, 0.001f, 10000.0f);
            math::Matrix world;
            it->second.obj->Update();
            world = it->second.obj->GetWorldMatrix();
            render::Device::SetUniformMatrix(m_LightProgram->GetUniformLocation("glb_MVP"), proj * m_Camera->GetViewMatrix() * world);
            render::Device::SetUniform3f(m_LightProgram->GetUniformLocation("glb_LightColor"), color);

            // Draw
            render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(lightModel->GetMeshId())->GetVertexNum());
        }
    }
}

 void ApplicationCore::DrawWeightMap() {
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

void ApplicationCore::CalcWeightMapColor() {
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

void ApplicationCore::DrawNormalizeWeightMap() {
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

void ApplicationCore::PrepareBake() {
    ResetLightPatch();
    PrepareLightPatch();
}

void ApplicationCore::ResetLightPatch() {
    if (m_Patch) {
        delete[] m_Patch;
        m_Patch = NULL;
    }

    m_ValidPatch.clear();
    m_CurBakeIterate = 0;
    m_CurValidPatchIndex = 0;
}

void ApplicationCore::PrepareLightPatch() {
    m_Patch = new LightPatch[m_LightMapHeight * m_LightMapWidth];
    memset(m_Patch, 0, sizeof(LightPatch) * m_LightMapHeight * m_LightMapWidth);

    // Calculate uv for every patch
    for (int32_t h = 0; h < m_LightMapHeight; h++) {
        for (int32_t w = 0; w < m_LightMapWidth; w++) {
            m_Patch[h * m_LightMapWidth + w].uv = math::Vector((w + 0.5f) * 1.0f / m_LightMapWidth, (m_LightMapHeight - h - 0.5f) * 1.0f / m_LightMapHeight, 0.0f);
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

    scene::ModelEffectParam effectParam;
    scene::ModelMaterialParam materialParam;
    float* vertexBuf = NULL;
    float* texBuf = NULL;
    float* normalBuf = NULL;
    float* tangentBuf = NULL;
    float* binormalBuf = NULL;
    int32_t faceNum = scene::ModelFile::ExtractModelData(m_SceneMeshName, effectParam, materialParam, &vertexBuf, &texBuf, &normalBuf, &tangentBuf, &binormalBuf);

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

    scene::ModelFile::RelaseBuf(&vertexBuf, &texBuf, &normalBuf, &tangentBuf, &binormalBuf);

    // Calculate data for every patch
    for (int32_t h = 0; h < m_LightMapHeight; h++) {
        for (int32_t w = 0; w < m_LightMapWidth; w++) {
            math::Vector uv = m_Patch[h * m_LightMapWidth + w].uv;

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

                m_Patch[h * m_LightMapWidth + w].pos = faces[i].vertex[0].pos * lambda0 + faces[i].vertex[1].pos * lambda1 + faces[i].vertex[2].pos * lambda2;
                m_Patch[h * m_LightMapWidth + w].valid = true;

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

                m_Patch[h * m_LightMapWidth + w].bais[0] = tbn * math::Vector(sqrt(2.0f / 3.0f), 0.0f, sqrt(1.0f / 3.0f), 0.0f);
                m_Patch[h * m_LightMapWidth + w].bais[1] = tbn * math::Vector(-sqrt(1.0f / 6.0f), sqrt(1.0f / 2.0f), sqrt(1.0f / 3.0f), 0.0f);
                m_Patch[h * m_LightMapWidth + w].bais[2] = tbn * math::Vector(-sqrt(1.0f / 6.0f), -sqrt(1.0f / 2.0f), sqrt(1.0f / 3.0f), 0.0f);
                m_Patch[h * m_LightMapWidth + w].bais[0].Normalize();
                m_Patch[h * m_LightMapWidth + w].bais[1].Normalize();
                m_Patch[h * m_LightMapWidth + w].bais[2].Normalize();

                m_ValidPatch.push_back(&m_Patch[h * m_LightMapWidth + w]);

                found = true;
                break;
            }

            if (found == false) {
                m_Patch[h * m_LightMapWidth + w].valid = false;
            }
        }
    }
}

void ApplicationCore::DrawHemiCube() {
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
                // Setup Shader
                render::Device::SetShader(m_LightPatchSceneProgram);
                render::Device::SetShaderLayout(m_LightPatchSceneProgram->GetShaderLayout());

                // Setup Texture
                render::Device::ClearTexture();
                render::Device::SetTexture(0, m_NormalizeWeightMap[j], 0);
                render::Device::SetTexture(1, m_LightMap[0], 1);
                render::Device::SetTexture(2, m_LightMap[1], 2);
                render::Device::SetTexture(3, m_LightMap[2], 3);
                render::Device::SetTexture(4, render::texture::Mgr::GetTextureById(m_SceneMesh->GetTexId(render::TS_ALBEDO)), 4);
                render::Device::SetTexture(5, render::texture::Mgr::GetTextureById(m_SceneMesh->GetTexId(render::TS_NORMAL)), 5);

                // Setup Vertex
                render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexLayout());
                render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexBuffer());

                // Setup Uniform
                math::Matrix world;
                world.MakeIdentityMatrix();
                world.Inverse();
                world.Transpose();
                math::Matrix proj;
                proj.MakeProjectionMatrix(1.0f, 90.0f, 0.001f, 10000.0f);
                render::Device::SetUniformMatrix(m_LightPatchMVPLoc, proj * views[j]);
                render::Device::SetUniform1i(m_LightPatchFaceLoc, j);
                render::Device::SetUniformSampler2D(m_NormlaizeWeightMapLoc, 0);
                render::Device::SetUniformSampler2D(m_LightMapLoc[0], 1);
                render::Device::SetUniformSampler2D(m_LightMapLoc[1], 2);
                render::Device::SetUniformSampler2D(m_LightMapLoc[2], 3);
                render::Device::SetUniformSampler2D(m_LightPatchAlbedoMapLoc, 4);
                render::Device::SetUniformSampler2D(m_LightPatchNormalMapLoc, 5);

                // Draw
                render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(m_SceneMesh->GetMeshId())->GetVertexNum());
            }

            {  // Draw Light
                for (LightSourceArray::iterator it = m_LightSource.begin(); it != m_LightSource.end(); ++it) {
                    scene::Model* lightModel = it->second.obj->GetModel();
                    math::Vector color = it->second.color;

                    // Setup Shader
                    render::Device::SetShader(m_LightPatchLightProgram);
                    render::Device::SetShaderLayout(m_LightPatchLightProgram->GetShaderLayout());

                    // Setup Texture
                    render::Device::ClearTexture();
                    render::Device::SetTexture(0, m_NormalizeWeightMap[j], 0);

                    // Setup Vertex
                    render::Device::SetVertexLayout(render::mesh::Mgr::GetMeshById(lightModel->GetMeshId())->GetVertexLayout());
                    render::Device::SetVertexBuffer(render::mesh::Mgr::GetMeshById(lightModel->GetMeshId())->GetVertexBuffer());

                    // Setup Uniform
                    math::Matrix proj;
                    proj.MakeProjectionMatrix(1.0f, 90.0f, 0.001f, 10000.0f);
                    math::Matrix world;
                    it->second.obj->Update();
                    world = it->second.obj->GetWorldMatrix();
                    render::Device::SetUniformMatrix(m_LightPatchLightProgram->GetUniformLocation("glb_MVP"), proj * views[j] * world);
                    render::Device::SetUniform3f(m_LightPatchLightProgram->GetUniformLocation("glb_LightColor"), color);
                    render::Device::SetUniform1i(m_LightPatchLightProgram->GetUniformLocation("glb_Face"), j);
                    render::Device::SetUniformSampler2D(m_LightPatchLightProgram->GetUniformLocation("glb_NormalizeWeightMap"), 0);

                    // Draw
                    render::Device::Draw(render::PT_TRIANGLES, 0, render::mesh::Mgr::GetMeshById(lightModel->GetMeshId())->GetVertexNum());
                }
            }
        }
    }
}

bool ApplicationCore::FindNextValidLightPatch() {
    bool result = true;

    m_CurValidPatchIndex = m_CurValidPatchIndex + 1;
    if (m_CurValidPatchIndex >= m_ValidPatch.size()) {
        m_CurValidPatchIndex = 0;
        result = false;
    }

    return result;
}

void ApplicationCore::CalcLightPatchColor() {
    for (int32_t i = 0; i < 3; i++) {
        math::Vector color(0.0f, 0.0f, 0.0f);
        float* pixelData = new float[4];

        int32_t maxLevel = log(max(kLightPatchSize, kLightPatchSize)) / log(2);

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

void ApplicationCore::BuildLightMap() {
    for (int32_t m = 0; m < 3; m++) {
        float* pixelData = new float[m_LightMapWidth * m_LightMapHeight * 4];

        // Flip map in vertical axis
        for (int32_t i = 0; i < m_LightMapHeight; i++) {
            for (int32_t j = 0; j < m_LightMapWidth; j++) {
                pixelData[(m_LightMapHeight - 1 - i) * m_LightMapWidth * 4 + j * 4 + 0] = m_Patch[i * m_LightMapWidth + j].color[m].x;
                pixelData[(m_LightMapHeight - 1 - i) * m_LightMapWidth * 4 + j * 4 + 1] = m_Patch[i * m_LightMapWidth + j].color[m].y;
                pixelData[(m_LightMapHeight - 1 - i) * m_LightMapWidth * 4 + j * 4 + 2] = m_Patch[i * m_LightMapWidth + j].color[m].z;
                pixelData[(m_LightMapHeight - 1 - i) * m_LightMapWidth * 4 + j * 4 + 3] = 1.0f;
            }
        }

        m_LightMap[m]->UpdateTextureData(pixelData);
        m_LightMap[m]->GenerateMipmap();

        delete[] pixelData;
        pixelData = NULL;
    }
}