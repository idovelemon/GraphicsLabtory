//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 11
// Brief: Model manage all the meshes,materials and textures.
//---------------------------------------------------------------------
#include "glbmodel.h"

#include <vector>

#include "glbmacro.h"
#include "glbmaterial.h"
#include "glbmesh.h"
#include "glbtexture.h"

namespace glb {

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
, m_ModefEffectParam()
, m_BoundBoxMax(0.0f, 0.0f, 0.0f)
, m_BoundBoxMin(0.0f, 0.0f, 0.0f) {
    for (int32_t i = 0; i < render::TS_MAX; i++) {
        m_Tex[i] = -1;
    }
}

Model::~Model() {
}

Model* Model::Create(const char* file_name) {
    Model* model = NULL;
    if (file_name != NULL) {
        mesh::TriangleMesh* mesh = NULL;
        int32_t diffuse_tex = -1;
        int32_t alpha_tex = -1;
        int32_t normal_tex = -1;
        int32_t material = -1;

        float* vertex_buf = NULL;
        float* tex_buf = NULL;
        float* normal_buf = NULL;
        float* tangent_buf = NULL;
        float* binormal_buf = NULL;
        ModelEffectParam effect_param;
        ModelMaterialParam material_param;
        int32_t num_triangles = ModelFile::ExtractModelData(
            file_name,
            effect_param,
            material_param,
            &vertex_buf,
            &tex_buf,
            &normal_buf,
            &tangent_buf,
            &binormal_buf
            );
        if (num_triangles > 0) {
            mesh = mesh::TriangleMesh::Create(num_triangles, vertex_buf, tex_buf, normal_buf, tangent_buf, binormal_buf);
            mesh::Mgr::AddMesh(mesh);

            if (effect_param.has_diffuse_tex) {
                diffuse_tex = texture::Mgr::LoadTexture(material_param.diffuse_tex_name);
            }

            if (effect_param.has_alpha_tex) {
                alpha_tex = texture::Mgr::LoadTexture(material_param.alpha_tex_name);
            }

            if (effect_param.has_normal_tex) {
                normal_tex = texture::Mgr::LoadTexture(material_param.normal_tex_name);
            }

            ModelFile::RelaseBuf(&vertex_buf, &tex_buf, &normal_buf, &tangent_buf, &binormal_buf);
        } else {
            GLB_SAFE_ASSERT(false);
        }

        material::Material mat;
        mat.ambient = material_param.ambient;
        mat.diffuse = material_param.diffuse;
        mat.specular = material_param.specular;
        mat.emission = material_param.emission;
        mat.specular_pow = material_param.pow;
        memcpy(mat.mat_name, file_name, strlen(file_name));
        mat.mat_name[strlen(file_name)] = '\0';
        material = material::Mgr::AddMaterial(mat);

        model = new Model;
        model->m_Name = std::string(file_name);
        model->m_Mesh = mesh->GetId();
        if (effect_param.has_diffuse_tex) {
            model->m_Tex[MT_DIFFUSE] = diffuse_tex;
        }
        if (effect_param.has_alpha_tex) {
            model->m_Tex[MT_ALPHA] = alpha_tex;
        }
        if (effect_param.has_normal_tex) {
            model->m_Tex[MT_NORMAL] = normal_tex;
        }
        model->m_Material = material;
        model->m_ModefEffectParam = effect_param;
        model->m_BoundBoxMax = material_param.boundbox_max;
        model->m_BoundBoxMin = material_param.boundbox_min;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return model;
}

std::string Model::GetName() {
    return m_Name;
}

int32_t Model::GetMeshId() {
    return m_Mesh;
}

int32_t Model::GetTexId(int32_t slot) {
    int32_t result = -1;
    if (0 <= slot && slot < render::TS_MAX) {
        result = m_Tex[slot];
    }

    return result;
}

int32_t Model::GetMaterial() {
    return m_Material;
}

Vector Model::GetBoundBoxMax() {
    return m_BoundBoxMax;
}

Vector Model::GetBoundBoxMin() {
    return m_BoundBoxMin;
}

bool Model::HasDiffuseTexture() {
    return m_ModefEffectParam.has_diffuse_tex;
}

bool Model::HasAlphaTexture() {
    return m_ModefEffectParam.has_alpha_tex;
}

bool Model::HasNormalTexture() {
    return m_ModefEffectParam.has_normal_tex;
}

bool Model::HasNormal() {
    return m_ModefEffectParam.has_normal;
}

bool Model::HasTangent() {
    return m_ModefEffectParam.has_tanget;
}

bool Model::HasBinormal() {
    return m_ModefEffectParam.has_binormal;
}

bool Model::IsAcceptLight() {
    return m_ModefEffectParam.accept_light;
}

void Model::SetAcceptLight(bool accept) {
    m_ModefEffectParam.accept_light = accept;
}

bool Model::IsAcceptShadow() {
    return m_ModefEffectParam.accept_shadow;
}

void Model::SetAcceptShadow(bool accept) {
    m_ModefEffectParam.accept_shadow = accept;
}

bool Model::IsCastShadow() {
    return m_ModefEffectParam.cast_shadow;
}

void Model::SetCastShadow(bool cast) {
    m_ModefEffectParam.cast_shadow = cast;
}

bool Model::IsUseAO() {
    return m_ModefEffectParam.use_ao;
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

Model* ModelMgr::GetModelByName(const char* file_name) {
    Model* model = NULL;

    if (s_ModelMgrImp != NULL) {
        model = s_ModelMgrImp->GetModelByName(file_name);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return model;
}
};  // namespace glb