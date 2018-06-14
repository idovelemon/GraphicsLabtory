//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 11
// Brief: Model manage all the meshes,materials and textures.
//---------------------------------------------------------------------
#include "glbmodel.h"

#include <vector>

#include "render/glbmaterial.h"
#include "render/glbmesh.h"
#include "render/glbtexture.h"

#include "util/glbmacro.h"
#include "util/glbutil.h"

namespace glb {

namespace scene {
//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class ModelMgrImp;
static ModelMgrImp* s_ModelMgrImp = NULL;

//----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------
class ModelMgrImp {
public:
    ModelMgrImp();
    virtual ~ModelMgrImp();

public:
    void Initialize();
    void Destroy();
    Model* AddModel(const char* file_name);
    void AddModel(Model* model);
    Model* GetModelByName(const char* file_name);

private:
    std::vector<Model*> m_ModelDataBase;
};  // class ModelMgrImp

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Model DEFINITION
//-----------------------------------------------------------------------------------
Model::Model()
: m_Name()
, m_Mesh(-1)
, m_Material(-1)
, m_ModelEffectParam()
, m_BoundBoxMax(0.0f, 0.0f, 0.0f)
, m_BoundBoxMin(0.0f, 0.0f, 0.0f) {
    for (int32_t i = 0; i < MT_MAX; i++) {
        m_Tex[i] = -1;
    }
}

Model::~Model() {
}

Model* Model::Create(const char* fileName) {
    Model* model = NULL;
    if (fileName != NULL) {
        render::mesh::TriangleMesh* mesh = NULL;
        int32_t albedoTex = -1;
        int32_t roughnessTex = -1;
        int32_t metallicTex = -1;
        int32_t alphaTex = -1;
        int32_t normalTex = -1;
        int32_t emissionTex = -1;
        int32_t diffusePFCTex = -1;
        int32_t specularPFCTex = -1;
        int32_t material = -1;

        float* vertexBuf = NULL;
        float* texBuf = NULL;
        float* lightMapTexBuf = NULL;
        float* normalBuf = NULL;
        float* tangentBuf = NULL;
        float* binormalBuf = NULL;
        ModelEffectParam effectParam;
        ModelMaterialParam materialParam;
        std::string dir = util::path_get_dir(fileName);
        int32_t num_triangles = ModelFile::ExtractModelData(
            fileName,
            effectParam,
            materialParam,
            &vertexBuf,
            &texBuf,
            &lightMapTexBuf,
            &normalBuf,
            &tangentBuf,
            &binormalBuf
            );
        if (num_triangles > 0) {
            mesh = render::mesh::TriangleMesh::Create(num_triangles, vertexBuf, texBuf, lightMapTexBuf, normalBuf, tangentBuf, binormalBuf);
            render::mesh::Mgr::AddMesh(mesh);

            if (effectParam.hasAlbedoTex) {
                std::string albedoTexPath = dir + materialParam.albedoTexName;
                albedoTex = render::texture::Mgr::LoadTexture(albedoTexPath.c_str());
            }

            if (effectParam.hasRoughnessTex) {
                std::string roughnessTexPath = dir + materialParam.roughnessTexName;
                roughnessTex = render::texture::Mgr::LoadTexture(roughnessTexPath.c_str());
            }

            if (effectParam.hasMetallicTex) {
                std::string metallicTexPath = dir + materialParam.metallicTexName;
                metallicTex = render::texture::Mgr::LoadTexture(metallicTexPath.c_str());
            }

            if (effectParam.hasAlphaTex) {
                std::string alphaTexPath = dir + materialParam.alphaTexName;
                alphaTex = render::texture::Mgr::LoadTexture(alphaTexPath.c_str());
            }

            if (effectParam.hasNormalTex) {
                std::string normalTexPath = dir + materialParam.normalTexName;
                normalTex = render::texture::Mgr::LoadTexture(normalTexPath.c_str());
            }

            if (effectParam.hasEmissionTex) {
                std::string emissionTexPath = dir + materialParam.emissionTexName;
                emissionTex = render::texture::Mgr::LoadTexture(emissionTexPath.c_str());
            }

            if (effectParam.hasDiffusePFCTex) {
                std::string diffusePFCTexPath = dir + materialParam.diffusePFCTexName;
                diffusePFCTex = render::texture::Mgr::LoadPFCTexture(diffusePFCTexPath.c_str());
            }

            if (effectParam.hasSpecularPFCTex) {
                std::string specularPFCTexPath = dir + materialParam.specularPFCTexName;
                specularPFCTex = render::texture::Mgr::LoadPFCTexture(specularPFCTexPath.c_str());
            }

            ModelFile::RelaseBuf(&vertexBuf, &texBuf, &normalBuf, &tangentBuf, &binormalBuf);
        } else {
            GLB_SAFE_ASSERT(false);
        }

        render::material::Material mat;
        mat.ambient = materialParam.ambient;
        mat.diffuse = materialParam.diffuse;
        mat.specular = materialParam.specular;
        mat.emission = materialParam.emission;
        mat.specular_pow = materialParam.pow;
        mat.albedo = materialParam.albedo;
        mat.roughness = materialParam.roughness;
        mat.metallic = materialParam.metallic;
        memcpy(mat.mat_name, fileName, strlen(fileName));
        mat.mat_name[strlen(fileName)] = '\0';
        material = render::material::Mgr::AddMaterial(mat);

        model = new Model;
        model->m_Name = std::string(fileName);
        model->m_Mesh = mesh->GetId();
        if (effectParam.hasAlbedoTex) {
            model->m_Tex[MT_ALBEDO] = albedoTex;
        }
        if (effectParam.hasRoughnessTex) {
            model->m_Tex[MT_ROUGHNESS] = roughnessTex;
        }
        if (effectParam.hasMetallicTex) {
            model->m_Tex[MT_METALLIC] = metallicTex;
        }
        if (effectParam.hasAlphaTex) {
            model->m_Tex[MT_ALPHA] = alphaTex;
        }
        if (effectParam.hasNormalTex) {
            model->m_Tex[MT_NORMAL] = normalTex;
        }
        if (effectParam.hasEmissionTex) {
            model->m_Tex[MT_EMISSION] = emissionTex;
        }
        if (effectParam.hasDiffusePFCTex) {
            model->m_Tex[MT_DIFFUSE_PFC] = diffusePFCTex;
        }
        if (effectParam.hasSpecularPFCTex) {
            model->m_Tex[MT_SPECULAR_PFC] = specularPFCTex;
        }
        model->m_Material = material;
        model->m_ModelEffectParam = effectParam;
        model->m_BoundBoxMax = materialParam.boundboxMax;
        model->m_BoundBoxMin = materialParam.boundboxMin;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return model;
}

Model* Model::Create(int32_t numTriangles, float* vertexBuf, float* texBuf, float* lightMapTexCoordBuf, float* normalBuf, float* tangentBuf, float* binormalBuf) {
    Model* model = NULL;
    render::mesh::TriangleMesh* mesh = NULL;

    if (numTriangles > 0 && vertexBuf) {
        mesh = render::mesh::TriangleMesh::Create(numTriangles, vertexBuf, texBuf, lightMapTexCoordBuf, normalBuf, tangentBuf, binormalBuf);
        render::mesh::Mgr::AddMesh(mesh);

        model = new Model;
        model->m_Name = std::string("DefaultMesh");
        model->m_Mesh = mesh->GetId();

        model->m_ModelEffectParam.hasAlbedoTex = false;
        model->m_ModelEffectParam.hasRoughnessTex = false;
        model->m_ModelEffectParam.hasMetallicTex = false;
        model->m_ModelEffectParam.hasAlphaTex = false;
        model->m_ModelEffectParam.hasNormalTex = false;
        model->m_ModelEffectParam.hasEmissionTex = false;
        model->m_ModelEffectParam.hasDiffusePFCTex = true;
        model->m_ModelEffectParam.hasSpecularPFCTex = true;
        model->m_ModelEffectParam.hasTexcoord = texBuf ? true : false;
        model->m_ModelEffectParam.hasLightMapTexCoord = lightMapTexCoordBuf ? true : false;
        model->m_ModelEffectParam.hasNormal = normalBuf ? true : false;
        model->m_ModelEffectParam.hasTanget = tangentBuf ? true : false;
        model->m_ModelEffectParam.hasBinormal = binormalBuf ? true : false;
        model->m_ModelEffectParam.acceptLight = true;
        model->m_ModelEffectParam.acceptShadow = false;
        model->m_ModelEffectParam.castShadow = false;
        model->m_ModelEffectParam.useAO = false;

        model->m_Tex[MT_DIFFUSE_PFC] = render::texture::Mgr::LoadPFCTexture("..\\glb\\resource\\texture\\diffuse.pfc");
        model->m_Tex[MT_SPECULAR_PFC] = render::texture::Mgr::LoadPFCTexture("..\\glb\\resource\\texture\\specular.pfc");

        render::material::Material material;
        material.albedo = math::Vector(1.0f, 1.0f, 1.0f);
        material.roughness = 0.5f;
        material.metallic = 0.5f;
        material.emission = math::Vector(0.0f, 0.0f, 0.0f);
        model->m_Material = render::material::Mgr::AddMaterial(material);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return model;
}

std::string Model::GetName() const {
    return m_Name;
}

int32_t Model::GetMeshId() const {
    return m_Mesh;
}

int32_t Model::GetTexId(int32_t slot) const {
    int32_t result = -1;
    if (0 <= slot && slot < MT_MAX) {
        result = m_Tex[slot];
    }

    return result;
}

int32_t Model::GetMaterial() const {
    return m_Material;
}

math::Vector Model::GetBoundBoxMax() const {
    return m_BoundBoxMax;
}

math::Vector Model::GetBoundBoxMin() const {
    return m_BoundBoxMin;
}

bool Model::HasAlbedoTexture() const {
    return m_ModelEffectParam.hasAlbedoTex;
}

bool Model::HasRoughnessTexture() const {
    return m_ModelEffectParam.hasRoughnessTex;
}

bool Model::HasMettalicTexture() const {
    return m_ModelEffectParam.hasMetallicTex;
}

bool Model::HasAlphaTexture() const {
    return m_ModelEffectParam.hasAlphaTex;
}

bool Model::HasNormalTexture() const {
    return m_ModelEffectParam.hasNormalTex;
}

bool Model::HasEmissionTexture() const {
    return m_ModelEffectParam.hasEmissionTex;
}

bool Model::HasReflectTexture() const {
    bool result = false;

    if (m_Tex[MT_REFLECT] != -1) {
        result = true;
    }

    return result;
}

bool Model::HasDiffusePFCTexture() const {
    return m_ModelEffectParam.hasDiffusePFCTex;
}

bool Model::HasSpecularPFCTexture() const {
    return m_ModelEffectParam.hasSpecularPFCTex;
}

bool Model::HasLightTexture() const {
    return m_ModelEffectParam.hasLightTex;
}

bool Model::HasLightMapTexCoord() const {
    return m_ModelEffectParam.hasLightMapTexCoord;
}

bool Model::HasTexCoord() const {
    return m_ModelEffectParam.hasTexcoord;
}

bool Model::HasNormal() const {
    return m_ModelEffectParam.hasNormal;
}

bool Model::HasTangent() const {
    return m_ModelEffectParam.hasTanget;
}

bool Model::HasBinormal() const {
    return m_ModelEffectParam.hasBinormal;
}

bool Model::IsAcceptLight() const {
    return m_ModelEffectParam.acceptLight;
}

void Model::SetAcceptLight(bool accept) {
    m_ModelEffectParam.acceptLight = accept;
}

bool Model::IsAcceptShadow() const {
    return m_ModelEffectParam.acceptShadow;
}

void Model::SetAcceptShadow(bool accept) {
    m_ModelEffectParam.acceptShadow = accept;
}

bool Model::IsCastShadow() const {
    return m_ModelEffectParam.castShadow;
}

void Model::SetCastShadow(bool cast) {
    m_ModelEffectParam.castShadow = cast;
}

bool Model::IsUseAO() const {
    return m_ModelEffectParam.useAO;
}

void Model::SetTexWithId(int32_t slot, int32_t tex_id) {
    if (0 <= slot && slot < MT_MAX) {
        m_Tex[slot] = tex_id;

        // Update model effect parameters
        switch (slot) {
        case MT_ALBEDO:
            m_ModelEffectParam.hasAlbedoTex = true;
            break;

        case MT_ROUGHNESS:
            m_ModelEffectParam.hasRoughnessTex = true;
            break;

        case MT_METALLIC:
            m_ModelEffectParam.hasMetallicTex = true;
            break;

        case MT_NORMAL:
            m_ModelEffectParam.hasNormalTex = true;
            break;

        case MT_ALPHA:
            m_ModelEffectParam.hasAlphaTex = true;
            break;

        case MT_EMISSION:
            m_ModelEffectParam.hasEmissionTex = true;
            break;

        case MT_LIGHT0:
        case MT_LIGHT1:
        case MT_LIGHT2:
            if (m_Tex[MT_LIGHT0] != -1 && m_Tex[MT_LIGHT1] != -1 && m_Tex[MT_LIGHT2] != -1) {
                m_ModelEffectParam.hasLightTex = true;
            }
            break;
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}
//-----------------------------------------------------------------------------------
// ModelMgrImp DEFINITION
//-----------------------------------------------------------------------------------
ModelMgrImp::ModelMgrImp() {
    m_ModelDataBase.clear();
}

ModelMgrImp::~ModelMgrImp() {
    Destroy();
}

void ModelMgrImp::Initialize() {
}

void ModelMgrImp::Destroy() {
    for (int32_t i = 0; i < static_cast<int32_t>(m_ModelDataBase.size()); i++) {
        GLB_SAFE_DELETE(m_ModelDataBase[i]);
    }
    m_ModelDataBase.clear();
}

Model* ModelMgrImp::AddModel(const char* file_name) {
    Model* model = NULL;

    if (file_name != NULL) {
        model = Model::Create(file_name);
        m_ModelDataBase.push_back(model);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return model;
}

void ModelMgrImp::AddModel(Model* model) {
    if (model != NULL) {
        m_ModelDataBase.push_back(model);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

Model* ModelMgrImp::GetModelByName(const char* file_name) {
    Model* model = NULL;

    if (file_name != NULL) {
        for (int32_t i = 0; i < static_cast<int32_t>(m_ModelDataBase.size()); i++) {
            if (!strcmp(file_name, m_ModelDataBase[i]->GetName().c_str())) {
                model = m_ModelDataBase[i];
                break;
            }
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return model;
}

//-----------------------------------------------------------------------------------
// ModelMgr DEFINITION
//-----------------------------------------------------------------------------------
void ModelMgr::Initialize() {
    if (s_ModelMgrImp == NULL) {
        s_ModelMgrImp = new ModelMgrImp;
        if (s_ModelMgrImp != NULL) {
            s_ModelMgrImp->Initialize();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void ModelMgr::Destroy() {
    if (s_ModelMgrImp != NULL) {
        s_ModelMgrImp->Destroy();
        GLB_SAFE_DELETE(s_ModelMgrImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

Model* ModelMgr::AddModel(const char* file_name) {
    Model* model = NULL;

    if (s_ModelMgrImp != NULL) {
        model = s_ModelMgrImp->AddModel(file_name);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return model;
}

void ModelMgr::AddModel(Model* model) {
    if (s_ModelMgrImp != NULL) {
        s_ModelMgrImp->AddModel(model);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

Model* ModelMgr::GetModelByName(const char* file_name) {
    Model* model = NULL;

    if (s_ModelMgrImp != NULL) {
        model = s_ModelMgrImp->GetModelByName(file_name);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return model;
}

};  // namespace scene

};  // namespace glb