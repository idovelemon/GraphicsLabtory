//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 14
// Brief: Define a object
//-----------------------------------------------------------------------------------
#include "glbobject.h"

#include <fstream>

#include "glbmodel.h"

#include "util/glbmacro.h"

namespace glb {

namespace scene {

Object::Object()
: m_IsDead(false)
, m_Model(NULL)
, m_Pos(0.0f, 0.0f, 0.0f)
, m_Scale(0.0f, 0.0f, 0.0f)
, m_Rotation(0.0f, 0.0f, 0.0f)
, m_WorldMatrix()
, m_EnableDraw(true)
, m_EnableCullFace(false)
, m_EnableDepthTest(false)
, m_EnableAlphaBlend(false)
, m_CullMode(render::CULL_NONE) {
}

Object::~Object() {
    m_Model = NULL;
}

Object* Object::Create(const char* file_name, math::Vector pos, math::Vector scale, math::Vector rotation) {
    Object* obj = NULL;
    
    if (file_name) {
        Model* model = ModelMgr::GetModelByName(file_name);
        if (model == NULL) {
            model = ModelMgr::AddModel(file_name);
        }

        if (model) {
            obj = new Object();
            if (obj) {
                obj->m_Model = model;
                obj->m_Pos = pos;
                obj->m_Scale = scale;
                obj->m_Rotation = rotation;
                obj->m_WorldMatrix.MakeIdentityMatrix();
                obj->m_ShaderDesc = obj->CalculateShaderDesc();
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

Object* Object::Create(scene::Model* model, math::Vector pos, math::Vector scale, math::Vector rotation) {
    Object* obj = NULL;

    if (model) {
        obj = new Object();
        if (obj) {
            ModelMgr::AddModel(model);
            obj->m_Model = model;
            obj->m_Pos = pos;
            obj->m_Scale = scale;
            obj->m_Rotation = rotation;
            obj->m_WorldMatrix.MakeIdentityMatrix();
            obj->m_ShaderDesc = obj->CalculateShaderDesc();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

void Object::SetDead(bool dead) {
    m_IsDead = dead;
}

bool Object::IsDead() const {
    return m_IsDead;
}

void Object::SetPos(math::Vector pos) {
    m_Pos = pos;
}

math::Vector Object::GetPos() const {
    return m_Pos;
}

void Object::SetScale(math::Vector scale) {
    m_Scale = scale;
}

math::Vector Object::GetScale() const {
    return m_Scale;
}

void Object::SetRotation(math::Vector rotation) {
    m_Rotation = rotation;
}

math::Vector Object::GetRotation() const {
    return m_Rotation;
}

math::Vector Object::GetBoundBoxMax() {
    math::Vector boundbox_max = m_Model->GetBoundBoxMax();
    boundbox_max += m_Pos;
    return boundbox_max;
}

math::Vector Object::GetBoundBoxMin() {
    math::Vector boundbox_min = m_Model->GetBoundBoxMin();
    boundbox_min += m_Pos;
    return boundbox_min;
}

math::Matrix Object::GetWorldMatrix() const {
    return m_WorldMatrix;
}

Model* Object::GetModel() {
    return m_Model;
}

render::shader::Descriptor Object::GetShaderDesc() {
    return m_ShaderDesc;
}

void Object::SetDrawEnable(bool enable) {
    m_EnableDraw = enable;
}

bool Object::IsDrawEnable() {
    return m_EnableDraw;
}

void Object::SetAlphaBlendEnable(bool enable) {
    m_EnableAlphaBlend = enable;
}

bool Object::IsAlphaBlendEnable() {
    return m_EnableAlphaBlend;
}

void Object::SetAlphaBlendFunc(render::AlphaBlendFactor factor, render::AlphaBlendFunc func) {
    if (factor == render::FACTOR_SRC) {
        m_SrcBlendFunc = func;
    } else if (factor == render::FACTOR_DST) {
        m_DstBlendFunc = func;
    }
}

render::AlphaBlendFunc Object::GetAlphaBlendFunc(render::AlphaBlendFactor factor) {
    render::AlphaBlendFunc func = render::FUNC_ZERO;

    if (factor == render::FACTOR_SRC) {
        func = m_SrcBlendFunc;
    } else if (factor == render::FACTOR_DST) {
        func = m_DstBlendFunc;
    }

    return func;
}

void Object::SetCullFaceEnable(bool enable) {
    m_EnableCullFace = enable;
}

bool Object::IsCullFaceEnable() {
    return m_EnableCullFace;
}

void Object::SetDepthTestEnable(bool enable) {
    m_EnableDepthTest = enable;
}

bool Object::IsDepthTestEnable() {
    return m_EnableDepthTest;
}

void Object::SetCullFaceMode(render::CullMode cull_mode) {
    m_CullMode = cull_mode;
}

render::CullMode Object::GetCullFaceMode() {
    return m_CullMode;
}

void Object::SetTexWithId(int32_t slot, int32_t tex_id) {
    bool bTryUsingLightMapping = false;
    if (m_Model) {
        switch (slot) {
        case Model::MT_ALBEDO:
            m_ShaderDesc.SetFlag(render::shader::GLB_ENABLE_ALBEDO_TEX, true);
            break;

        case Model::MT_ROUGHNESS:
            m_ShaderDesc.SetFlag(render::shader::GLB_ENABLE_ROUGHNESS_TEX, true);
            break;

        case Model::MT_METALLIC:
            m_ShaderDesc.SetFlag(render::shader::GLB_ENABLE_METALLIC_TEX, true);
            break;

        case Model::MT_ALPHA:
            m_ShaderDesc.SetFlag(render::shader::GLB_ENABLE_ALPHA_TEX, true);
            break;

        case Model::MT_NORMAL:
            m_ShaderDesc.SetFlag(render::shader::GLB_ENABLE_NORMAL_TEX, true);
            break;

        case Model::MT_EMISSION:
            m_ShaderDesc.SetFlag(render::shader::GLB_ENABLE_EMISSION_TEX, true);
            break;

        case Model::MT_LIGHT0:
        case Model::MT_LIGHT1:
        case Model::MT_LIGHT2:
            bTryUsingLightMapping = true;
            break;

        case Model::MT_REFLECT:
            m_ShaderDesc.SetFlag(render::shader::GLB_ENABLE_REFLECT_TEX, true);
            break;
        }

        m_Model->SetTexWithId(slot, tex_id);

        if (bTryUsingLightMapping && m_Model->GetTexId(Model::MT_LIGHT0) != -1 && m_Model->GetTexId(Model::MT_LIGHT1) != -1 && m_Model->GetTexId(Model::MT_LIGHT2) != -1) {
            m_ShaderDesc.SetFlag(render::shader::GLB_ENABLE_LIGHT_TEX, true);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Object::GetTexId(int32_t slot) {
    int32_t result = -1;

    if (m_Model) {
        result = m_Model->GetTexId(slot);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void Object::Update() {
    // Build world transform matrix
    m_WorldMatrix.MakeIdentityMatrix();
    m_WorldMatrix.Scale(m_Scale.x, m_Scale.y, m_Scale.z);

    // Build rotation matrix in xyz-order
    m_WorldMatrix.RotateXYZ(m_Rotation.x, m_Rotation.y, m_Rotation.z);

    m_WorldMatrix.Translate(m_Pos.x, m_Pos.y, m_Pos.z);
}

render::shader::Descriptor Object::CalculateShaderDesc() {
    render::shader::Descriptor desc;

    if (m_Model->HasTexCoord()) {
        desc.SetFlag(render::shader::GLB_TEXCOORD_IN_VERTEX, true);
    }

    if (m_Model->HasLightMapTexCoord()) {
        desc.SetFlag(render::shader::GLB_LIGHT_TEXCOORD_IN_VERTEX, true);
    }

    if (m_Model->HasAlbedoTexture()) {
        desc.SetFlag(render::shader::GLB_ENABLE_ALBEDO_TEX, true);
    }

    if (m_Model->HasRoughnessTexture()) {
        desc.SetFlag(render::shader::GLB_ENABLE_ROUGHNESS_TEX, true);
    }

    if (m_Model->HasMettalicTexture()) {
        desc.SetFlag(render::shader::GLB_ENABLE_METALLIC_TEX, true);
    }

    if (m_Model->HasAlphaTexture()) {
        desc.SetFlag(render::shader::GLB_ENABLE_ALPHA_TEX, true);
    }

    if (m_Model->HasNormalTexture()) {
        desc.SetFlag(render::shader::GLB_ENABLE_NORMAL_TEX, true);
    }

    if (m_Model->HasEmissionTexture()) {
        desc.SetFlag(render::shader::GLB_ENABLE_EMISSION_TEX, true);
    }

    if (m_Model->HasNormal()) {
        desc.SetFlag(render::shader::GLB_NORMAL_IN_VERTEX, true);
    }

    if (m_Model->HasTangent()) {
        desc.SetFlag(render::shader::GLB_TANGENT_IN_VERTEX, true);
    }

    if (m_Model->HasBinormal()) {
        desc.SetFlag(render::shader::GLB_BINORMAL_IN_VERTEX, true);
    }

    if (m_Model->IsAcceptLight()) {
        desc.SetFlag(render::shader::GLB_ENABLE_LIGHTING, true);

        for (int32_t i = 0; i < scene::kMaxLight; i++) {
            scene::Light lit = scene::Scene::GetLight(i);

            switch(lit.type) {
            case scene::PARALLEL_LIGHT:
                desc.SetFlag(render::shader::GLB_USE_PARALLEL_LIGHT, true);
                break;
            }
        }
    }

    if (m_Model->IsAcceptShadow()) {
        desc.SetFlag(render::shader::GLB_ENABLE_SHADOW, true);
    }

    if (m_Model->IsUseAO()) {
        desc.SetFlag(render::shader::GLB_ENABLE_AO, true);
    }

    return desc;
}

};  // namespace scene

};  // namespace glb
