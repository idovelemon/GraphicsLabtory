//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 11
// Brief: Core routine for model editor
//------------------------------------------------------------------
#include "glb_modeleditor_core.h"

#include <fstream>

#include "GL/glew.h"

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"

using namespace glb;

ApplicationCore* ApplicationCore::s_Instance = NULL;

ApplicationCore::ApplicationCore()
: m_SceneMesh(-1)
, m_Material(-1)
, m_DefaultTexture2D(-1)
, m_DefaultTextureCube(-1) {
    s_Instance = this;
    memset(m_SceneMeshName, 0, sizeof(m_SceneMeshName));
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
    // Camera
    scene::ModelCamera* cam = scene::ModelCamera::Create(math::Vector(0.0f, 10.0f, 10.0), math::Vector(0.0f, 0.0f, 0.0));
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

    // Default texture
    m_DefaultTexture2D = render::texture::Mgr::LoadTexture("res/default2d.bmp");
    m_DefaultTextureCube = render::texture::Mgr::LoadPFCTexture("res/defaultcube.pfc");

    return true;
}

void ApplicationCore::Update(float dt) {
    util::ProfileTime time;
    time.BeginProfile();

    Input::Update();

    UpdateCamera();

    // Update Scene
    scene::Scene::Update();
    render::Render::Draw();

    GLB_CHECK_GL_ERROR;

    time.EndProfile();
    // printf("%f\n", time.GetProfileTimeInMs());
}

void ApplicationCore::Destroy() {
}

bool ApplicationCore::AddModel(const char* name) {
    Assimp::Importer importer;
    GLB_SAFE_ASSERT(importer.ValidateFlags(aiProcess_CalcTangentSpace | aiProcess_Triangulate) == true);
    const aiScene* scene = importer.ReadFile(name, aiProcess_CalcTangentSpace | aiProcess_Triangulate);

    bool result = false;
    if (scene && scene->HasMeshes())
    {
        // Transform vertex, normal, tangent and binormal
        math::Matrix vertexTrans = math::Matrix::CreateRotateXMatrix(-90.0f);
        math::Matrix normalTrans = math::Matrix::Inverse(vertexTrans);
        normalTrans.Transpose();

        // Only deal with mesh 0 now
        aiMesh* mesh = scene->mMeshes[0];
        int32_t trianglesNum = scene->mMeshes[0]->mNumFaces;
        float* vertexBuf = new float[trianglesNum * 3 * 3];
        float* texcoordBuf = mesh->HasTextureCoords(0) ? new float[trianglesNum * 3 * 2] : NULL;
        float* lightMapTexCoordBuf = mesh->HasTextureCoords(1) ? new float[trianglesNum * 3 * 2] : NULL;
        float* normalBuf = mesh->HasNormals() ? new float[trianglesNum * 3 * 3] : NULL;
        float* tangentBuf = mesh->HasTangentsAndBitangents() ? new float[trianglesNum * 3 * 3] : NULL;
        float* binormalBuf = mesh->HasTangentsAndBitangents() ? new float[trianglesNum * 3 * 3] : NULL;

        for (int32_t i = 0; i < trianglesNum; i++) {
            aiFace face = scene->mMeshes[0]->mFaces[i];
            if (face.mNumIndices == 3) {
                // Vertex Position
                for (int32_t j = 0; j < 3; j++) {
                    math::Vector pos(mesh->mVertices[face.mIndices[j]].x, mesh->mVertices[face.mIndices[j]].y, mesh->mVertices[face.mIndices[j]].z);
                    pos = vertexTrans * pos;
                    vertexBuf[i * 3 * 3 + j * 3 + 0] = pos.x;
                    vertexBuf[i * 3 * 3 + j * 3 + 1] = pos.y;
                    vertexBuf[i * 3 * 3 + j * 3 + 2] = pos.z;
                }

                // Vertex UV
                if (mesh->HasTextureCoords(0)) {
                    for (int32_t j = 0; j < 3; j++) {
                        texcoordBuf[i * 3 * 2 + j * 2 + 0] = mesh->mTextureCoords[0][face.mIndices[j]].x;
                        texcoordBuf[i * 3 * 2 + j * 2 + 1] = mesh->mTextureCoords[0][face.mIndices[j]].y;
                    }
                }

                // Vertex Second UV
                if (mesh->HasTextureCoords(1)) {
                    for (int32_t j = 0; j < 3; j++) {
                        lightMapTexCoordBuf[i * 3 * 2 + j * 2 + 0] = mesh->mTextureCoords[1][face.mIndices[j]].x;
                        lightMapTexCoordBuf[i * 3 * 2 + j * 2 + 1] = mesh->mTextureCoords[1][face.mIndices[j]].y;
                    }
                }

                // Vertex Normal
                if (mesh->HasNormals()) {
                    for (int32_t j = 0; j < 3; j++) {
                        math::Vector normal(mesh->mNormals[face.mIndices[j]].x, mesh->mNormals[face.mIndices[j]].y, mesh->mNormals[face.mIndices[j]].z);
                        normal = normalTrans * normal;
                        normalBuf[i * 3 * 3 + j * 3 + 0] = normal.x;
                        normalBuf[i * 3 * 3 + j * 3 + 1] = normal.y;
                        normalBuf[i * 3 * 3 + j * 3 + 2] = normal.z;
                    }
                }

                // Vertex Tangent and Binormal
                if (mesh->HasTangentsAndBitangents()) {
                    for (int32_t j = 0; j < 3; j++) {
                        math::Vector tangent(mesh->mTangents[face.mIndices[j]].x, mesh->mTangents[face.mIndices[j]].y, mesh->mTangents[face.mIndices[j]].z);
                        tangent = normalTrans * tangent;
                        tangentBuf[i * 3 * 3 + j * 3 + 0] = tangent.x;
                        tangentBuf[i * 3 * 3 + j * 3 + 1] = tangent.y;
                        tangentBuf[i * 3 * 3 + j * 3 + 2] = tangent.z;

                        math::Vector biTangent(mesh->mBitangents[face.mIndices[j]].x, mesh->mBitangents[face.mIndices[j]].y, mesh->mBitangents[face.mIndices[j]].z);
                        biTangent = normalTrans * biTangent;
                        binormalBuf[i * 3 * 3 + j * 3 + 0] = biTangent.x;
                        binormalBuf[i * 3 * 3 + j * 3 + 1] = biTangent.y;
                        binormalBuf[i * 3 * 3 + j * 3 + 2] = biTangent.z;
                    }
                }
            } else {
                GLB_SAFE_ASSERT(false);  // Must be triangle
            }
        }

        render::mesh::MeshBase* model = render::mesh::TriangleMesh::Create(trianglesNum, vertexBuf, texcoordBuf, lightMapTexCoordBuf, normalBuf, tangentBuf, binormalBuf);
        render::mesh::Mgr::AddMesh(model);

        render::material::Material* material = render::material::Material::Create("res/default.mat");
        render::material::Mgr::AddMaterial(material);

        int32_t modelId = scene::Scene::AddObject(model, material);
        if (modelId != -1) {
            scene::Scene::GetObjectById(modelId)->SetCullFaceEnable(true);
            scene::Scene::GetObjectById(modelId)->SetCullFaceMode(glb::render::CULL_BACK);
            scene::Scene::GetObjectById(modelId)->SetDepthTestEnable(true);
            scene::Scene::GetObjectById(modelId)->SetWorldMatrix(math::Matrix::CreateIdentityMatrix());
            m_SceneMesh = modelId;
            memcpy(m_SceneMeshName, name, strlen(name));
            m_SceneMeshName[strlen(name)] = '\0';
            result = true;
        } else {
            GLB_SAFE_ASSERT(false);  // Something wrong
        }

        delete[] vertexBuf;
        vertexBuf = NULL;
        if (texcoordBuf) {
            delete[] texcoordBuf;
            texcoordBuf = NULL;
        }
        if (normalBuf) {
            delete[] normalBuf;
            normalBuf = NULL;
        }
        if (tangentBuf) {
            delete[] tangentBuf;
            tangentBuf = NULL;
        }
        if (binormalBuf) {
            delete[] binormalBuf;
            binormalBuf = NULL;
        }
    }

    return result;
}

bool ApplicationCore::SaveModel(const char* filePath) {
    Assimp::Importer importer;
    GLB_SAFE_ASSERT(importer.ValidateFlags(aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FixInfacingNormals) == true);
    const aiScene* scene = importer.ReadFile(m_SceneMeshName, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FixInfacingNormals);

    bool result = false;
    if (scene && scene->HasMeshes()) {
        std::ofstream output;
        output.open(filePath);
        if (!output.fail()) {
            // Only deal with mesh 0 now
            aiMesh* mesh = scene->mMeshes[0];
            int32_t trianglesNum = scene->mMeshes[0]->mNumFaces;

            for (int32_t i = 0; i < trianglesNum; i++) {
                aiFace face = scene->mMeshes[0]->mFaces[i];
                if (face.mNumIndices == 3) {
                    std::string faceStr[3];

                    // Vertex Position
                    for (int32_t j = 0; j < 3; j++) {
                        output << "v ";
                        output << mesh->mVertices[face.mIndices[j]].x << " ";
                        output << mesh->mVertices[face.mIndices[j]].y << " ";
                        output << mesh->mVertices[face.mIndices[j]].z << "\n";

                        char temp[64];
                        sprintf_s(temp, "%d", i * 3 + j + 1);
                        faceStr[j] = faceStr[j] + temp;
                    }

                    // Vertex UV
                    if (mesh->HasTextureCoords(0)) {
                        for (int32_t j = 0; j < 3; j++) {
                            output << "vt ";
                            output << mesh->mTextureCoords[0][face.mIndices[j]].x << " ";
                            output << mesh->mTextureCoords[0][face.mIndices[j]].y << "\n";

                            char temp[64];
                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;
                        }
                    }

                    // Vertex Second UV
                    if (mesh->HasTextureCoords(1)) {
                        for (int32_t j = 0; j < 3; j++) {
                            output << "vlightmapt ";
                            output << mesh->mTextureCoords[1][face.mIndices[j]].x << " ";
                            output << mesh->mTextureCoords[1][face.mIndices[j]].y << "\n";

                            char temp[64];
                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;
                        }
                    }

                    // Vertex Normal
                    if (mesh->HasNormals()) {
                        for (int32_t j = 0; j < 3; j++) {
                            output << "vn ";
                            output << mesh->mNormals[face.mIndices[j]].x << " ";
                            output << mesh->mNormals[face.mIndices[j]].y << " ";
                            output << mesh->mNormals[face.mIndices[j]].z << "\n";

                            char temp[64];
                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;
                        }
                    }

                    // Vertex Tangent and Binormal
                    if (mesh->HasTangentsAndBitangents()) {
                        for (int32_t j = 0; j < 3; j++) {
                            output << "vtan ";
                            output << mesh->mTangents[face.mIndices[j]].x << " ";
                            output << mesh->mTangents[face.mIndices[j]].y << " ";
                            output << mesh->mTangents[face.mIndices[j]].z << "\n";

                            char temp[64];
                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;

                            output << "vbi ";
                            output << mesh->mBitangents[face.mIndices[j]].x << " ";
                            output << mesh->mBitangents[face.mIndices[j]].y << " ";
                            output << mesh->mBitangents[face.mIndices[j]].z << "\n";

                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;
                        }
                    }

                    output << "f " << faceStr[0] << " " << faceStr[1] << " " << faceStr[2] << "\n";
                } else {
                    GLB_SAFE_ASSERT(false);  // Must be triangle
                }
            }

            output.close();
            result = true;
        }
    }

    return result;
}

bool ApplicationCore::Preview(const char* name) {
    m_SceneMesh = scene::Scene::AddObject(name, "res/default.mat");
    if (m_SceneMesh != -1) {
        scene::Scene::GetObjectById(m_SceneMesh)->SetDepthTestEnable(true);
        scene::Scene::GetObjectById(m_SceneMesh)->SetWorldMatrix(math::Matrix::CreateIdentityMatrix());

        scene::Scene::GetObjectById(m_SceneMesh)->SetCullFaceEnable(true);
        scene::Scene::GetObjectById(m_SceneMesh)->SetCullFaceMode(glb::render::CULL_BACK);
    }

    return m_SceneMesh != -1;
}

bool ApplicationCore::AddEmptyMaterial(const char* name) {
    bool result = false;

    render::material::Material* mat = new glb::render::material::Material;
    if (mat) {
        mat->SetMaterialName(name);
        m_Material = render::material::Mgr::AddMaterial(mat);
        result = true;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

bool ApplicationCore::AddMaterial(const char* name) {
    bool result = false;

    m_Material = render::material::Mgr::AddMaterial(name);
    if (m_Material >= 0) {
        scene::Scene::GetObjectById(m_SceneMesh)->SetMaterial(render::material::Mgr::GetMaterial(m_Material));
        result = true;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

bool ApplicationCore::SaveMaterial(const char* name) {
    bool result = false;

    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        std::string destDir = util::path_get_dir(name);

        std::ofstream output;
        output.open(name);

        std::vector<glb::render::material::PassMaterial*>& passMaterial = mat->GetAllPassMaterial();

        for (auto& pass : passMaterial) {
            output << "beginpass " << pass->GetPassName() << "\n";

            const char* vertexShaderName = glb::render::shader::Mgr::GetShader(pass->GetShaderID())->GetVertexShaderName();
            const char* fragmentShaderName = glb::render::shader::Mgr::GetShader(pass->GetShaderID())->GetFragmentShaderName();
            output << "shader " << util::path_get_name(vertexShaderName).c_str() << " " << util::path_get_name(fragmentShaderName).c_str() << "\n";

            std::string srcFragmentShaderName = util::path_get_dir(vertexShaderName) + util::path_get_name(fragmentShaderName).substr(4);

            // Copy shader file
            std::string destVertexShaderName = destDir.empty() ? util::path_get_name(vertexShaderName) : (destDir + '\\' + util::path_get_name(vertexShaderName));
            std::string destFragmentShaderName = destDir.empty() ? util::path_get_name(fragmentShaderName) : (destDir + '\\' + util::path_get_name(fragmentShaderName));
            std::string destSrcFragmentShaderName = destDir.empty() ? util::path_get_name(srcFragmentShaderName.c_str()) : (destDir + '\\' + util::path_get_name(srcFragmentShaderName.c_str()));
            CopyFileA(vertexShaderName, destVertexShaderName.c_str(), TRUE);
            CopyFileA(fragmentShaderName, destFragmentShaderName.c_str(), TRUE);
            CopyFileA(srcFragmentShaderName.c_str(), destSrcFragmentShaderName.c_str(), TRUE);

            for (auto& param : pass->GetAllParameters()) {
                if (param.type == glb::render::material::PassMaterial::PARAMETER_TYPE_USER) {
                    switch (param.format) {
                    case glb::render::PARAMETER_FORMAT_INT:
                        output << "passparameter " << param.name << " " << param.intValue << "\n";
                        break;
                    case glb::render::PARAMETER_FORMAT_FLOAT:
                        output << "passparameter " << param.name << " " << param.floatValue << "\n";
                        break;
                    case glb::render::PARAMETER_FORMAT_FLOAT3:
                        output << "passparameter " << param.name << " "
                               << param.vecValue.x << " "
                               << param.vecValue.y << " "
                               << param.vecValue.z << "\n";
                        break;
                    case glb::render::PARAMETER_FORMAT_FLOAT4:
                        output << "passparameter " << param.name << " "
                               << param.vecValue.x << " "
                               << param.vecValue.y << " "
                               << param.vecValue.z << " "
                               << param.vecValue.w << "\n";
                    case glb::render::PARAMETER_FORMAT_TEXTURE_2D:
                    case glb::render::PARAMETER_FORMAT_TEXTURE_CUBE:
                        const char* texPath = glb::render::texture::Mgr::GetTextureById(param.intValue)->GetName();
                        std::string texName = util::path_get_name(texPath);
                        output << "passparameter " << param.name << " " << texName.c_str() << "\n";

                        std::string destTextureName = destDir.empty() ? std::string(texPath) : (destDir + '\\' + texName);
                        CopyFileA(texPath, destTextureName.c_str(), TRUE);
                        break;
                    }
                }
            }

            output << "endpass" << "\n";
        }

        if (mat->IsCastShadowEnable()) {
            output << "materialparameter " << "castshadow " << 1 << "\n";
        } else {
            output << "materialparameter " << "castshadow " << 0 << "\n";
        }

        if (mat->IsReceiveShadowEnable()) {
            output << "materialparameter " << "receiveshadow " << 1;
        } else {
            output << "materialparameter " << "receiveshadow " << 0;
        }

        result = true;
        output.close();
    }

    return result;
}

bool ApplicationCore::TryCompileShader(const char* passName, const char* vertexShaderName, const char* fragmentShaderName) {
    bool result = true;

    // Try compile shader
    glb::render::shader::UberProgram* program = glb::render::shader::UberProgram::Create(vertexShaderName, fragmentShaderName);

    if (program) {
        // Try replace old pass shader
        int32_t shaderID = glb::render::shader::Mgr::ReplaceUberShader(program);

        // If failed, create a new shader
        if (shaderID == -1)
        {
            shaderID = glb::render::shader::Mgr::AddUberShader(program);
        }

        glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
        if (mat) {
            // Try to get pass
            glb::render::material::PassMaterial* passMaterial = mat->GetPassMaterial(passName);

            // If failed, create a new pass
            if (passMaterial == nullptr)
            {
                passMaterial = new glb::render::material::PassMaterial();
                passMaterial->SetPassName(passName);
                mat->AddPassMaterial(passMaterial);
            }

            // Setup shader and parameters
            passMaterial->SetShaderID(shaderID);
            passMaterial->CollectParameter();

            // Setup default texture parameter
            std::vector<render::material::PassMaterial::ParameterEntry>& parameters = passMaterial->GetAllParameters();
            for (auto& param : parameters) {
                if (param.type == render::material::PassMaterial::PARAMETER_TYPE_USER) {
                    switch (param.format) {
                    case render::PARAMETER_FORMAT_TEXTURE_2D:
                        param.intValue = m_DefaultTexture2D;
                        break;
                    case render::PARAMETER_FORMAT_TEXTURE_CUBE:
                        param.intValue = m_DefaultTextureCube;
                        break;
                    }
                }
            }

            // Setup model
            scene::Scene::GetObjectById(m_SceneMesh)->SetMaterial(mat);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        result = false;
    }

    return result;
}

std::vector<glb::render::material::PassMaterial::ParameterEntry>& ApplicationCore::GetPassParameters(const char* passName) {
    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        glb::render::material::PassMaterial* passMaterial = mat->GetPassMaterial(passName);
        if (passMaterial) {
            return passMaterial->GetAllParameters();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    static std::vector<glb::render::material::PassMaterial::ParameterEntry> sNonePassParameters;
    return sNonePassParameters;
}

std::vector<std::vector<glb::render::material::PassMaterial::ParameterEntry>> ApplicationCore::GetAllPassParameters() {
    std::vector<std::vector<glb::render::material::PassMaterial::ParameterEntry>> result;
    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        std::vector<glb::render::material::PassMaterial*>& allPassMaterial = mat->GetAllPassMaterial();
        for (glb::render::material::PassMaterial* pass : allPassMaterial) {
            if (pass) {
                result.push_back(pass->GetAllParameters());
            }
        }
        return result;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    static std::vector<std::vector<glb::render::material::PassMaterial::ParameterEntry>> sNonePassParameters;
    return sNonePassParameters;
}

void ApplicationCore::SetPassParameterInt(const char* passName, const char* parameterName, int32_t value) {
    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        glb::render::material::PassMaterial* passMaterial = mat->GetPassMaterial(passName);
        if (passMaterial) {
            glb::render::material::PassMaterial::ParameterEntry& entry = passMaterial->GetParameterByName(parameterName);
            if (entry.format == glb::render::PARAMETER_FORMAT_INT) {
                entry.intValue = value;
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ApplicationCore::SetPassParameterFloat(const char* passName, const char* parameterName, float value) {
    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        glb::render::material::PassMaterial* passMaterial = mat->GetPassMaterial(passName);
        if (passMaterial) {
            glb::render::material::PassMaterial::ParameterEntry& entry = passMaterial->GetParameterByName(parameterName);
            if (entry.format == glb::render::PARAMETER_FORMAT_FLOAT) {
                entry.floatValue = value;
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ApplicationCore::SetPassParameterVec(const char* passName, const char* parameterName, glb::math::Vector value) {
    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        glb::render::material::PassMaterial* passMaterial = mat->GetPassMaterial(passName);
        if (passMaterial) {
            glb::render::material::PassMaterial::ParameterEntry& entry = passMaterial->GetParameterByName(parameterName);
            if (entry.format == glb::render::PARAMETER_FORMAT_FLOAT3
                || entry.format == glb::render::PARAMETER_FORMAT_FLOAT4) {
                entry.vecValue = value;
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ApplicationCore::SetPassParameterTex(const char* passName, const char* parameterName, int32_t value) {
    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        glb::render::material::PassMaterial* passMaterial = mat->GetPassMaterial(passName);
        if (passMaterial) {
            glb::render::material::PassMaterial::ParameterEntry& entry = passMaterial->GetParameterByName(parameterName);
            if (entry.format == glb::render::PARAMETER_FORMAT_TEXTURE_2D
                || entry.format == glb::render::PARAMETER_FORMAT_TEXTURE_CUBE) {
                entry.intValue = value;
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

bool ApplicationCore::IsCastShadow() {
    bool result = false;

    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        result = mat->IsCastShadowEnable();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void ApplicationCore::SetCastShadow(bool bCastShadow) {
    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        mat->SetCastShadowEnable(bCastShadow);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

bool ApplicationCore::IsReceiveShadow() {
    bool result = false;

    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        result = mat->IsReceiveShadowEnable();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void ApplicationCore::SetRecieveShadow(bool bRecieveShadow) {
    glb::render::material::Material* mat = glb::render::material::Mgr::GetMaterial(m_Material);
    if (mat) {
        mat->SetReceiveShadowEnable(bRecieveShadow);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ApplicationCore::UpdateCamera() {
    if (Input::IsKeyboardButtonPressed(BK_LCONTROL) && Input::IsMouseButtonPressed(BM_LEFT)) {
        static float sRot = 0.0f;
        sRot = sRot + Input::GetMouseMoveX() * 0.1f;
        static float sDist = 0.0f;
        sDist = sDist + Input::GetMouseMoveY() * 0.05f;
        math::Vector pos = math::Vector(0.0f, 10.0f, 10.0) + math::Vector(0.0f, 1.0f, 1.0f) * sDist;
        math::Matrix rotY;
        rotY.MakeRotateYMatrix(sRot);
        pos = rotY * pos;
        scene::Scene::GetCurCamera()->SetPos(pos);
        scene::Scene::GetCurCamera()->Update(1.0f);
    }
}