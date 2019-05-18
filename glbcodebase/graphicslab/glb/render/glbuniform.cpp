//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 24
// Brief: Manage uniforms
//-----------------------------------------------------------
#include "glbuniform.h"

#include "glbapplication.h"
#include "glbmaterial.h"
#include "glbrender.h"
#include "glbrenderdevice.h"

#include "scene/glbcamera.h"
#include "scene/glbobject.h"

namespace glb {

namespace render {

namespace uniform {

// Wrapper
Wrapper::Wrapper()
: m_Int(0)
, m_Float(0.0f)
, m_Sampler2D(-1)
, m_SamplerCube(-1)
, m_Vector(0.0f, 0.0f, 0.0f)
, m_Matrix()
, m_Format(0) {
}

Wrapper::~Wrapper() {
}

void Wrapper::SetInt(int v) {
    m_Int = v;
}

int Wrapper::GetInt() {
    return m_Int;
}

void Wrapper::SetFloat(float v) {
    m_Float = v;
}

float Wrapper::GetFloat() {
    return m_Float;
}

void Wrapper::SetSampler2D(int32_t sampler) {
    m_Sampler2D = sampler;
}

int32_t Wrapper::GetSampler2D() {
    return m_Sampler2D;
}

void Wrapper::SetSampler3D(int32_t sampler) {
    m_Sampler3D = sampler;
}

int32_t Wrapper::GetSampler3D() {
    return m_Sampler3D;
}

void Wrapper::SetSamplerCube(int32_t sampler) {
    m_SamplerCube = sampler;
}

int32_t Wrapper::GetSamplerCube() {
    return m_SamplerCube;
}

void Wrapper::SetVector(math::Vector v) {
    m_Vector = v;
}

math::Vector& Wrapper::GetVector() {
    return m_Vector;
}

void Wrapper::SetMatrix(math::Matrix v) {
    m_Matrix = v;
}

math::Matrix& Wrapper::GetMatrix() {
    return m_Matrix;
}

void Wrapper::SetFormat(int32_t format) {
    m_Format = format;
}

int32_t Wrapper::GetFormat() {
    return m_Format;
}

// Uniform picker
Wrapper uniform_projm_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    int32_t type = render::Render::GetCurPerspectiveType();
    math::Matrix proj = render::Render::GetPerspective(type);
    wrapper.SetMatrix(proj);

    return wrapper;
}

Wrapper uniform_viewm_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    int32_t type = scene::Scene::GetCurCameraType();
    scene::CameraBase* cam = scene::Scene::GetCamera(type);
    if (cam != nullptr) {
        wrapper.SetMatrix(cam->GetViewMatrix());
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return wrapper;
}

Wrapper uniform_worldm_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    if (obj != nullptr) {
        math::Matrix m = obj->GetWorldMatrix();
        wrapper.SetMatrix(m);
    } else {
        GLB_SAFE_ASSERT(false);
        math::Matrix m;
        m.MakeIdentityMatrix();
        wrapper.SetMatrix(m);
    }

    return wrapper;
}

Wrapper uniform_shadowm0_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);
    wrapper.SetMatrix(render::Render::GetShadowMapMatrix(0));
    return wrapper;
}

Wrapper uniform_shadowm1_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);
    wrapper.SetMatrix(render::Render::GetShadowMapMatrix(1));
    return wrapper;
}

Wrapper uniform_shadowm2_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);
    wrapper.SetMatrix(render::Render::GetShadowMapMatrix(2));
    return wrapper;
}

Wrapper uniform_shadowm3_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);
    wrapper.SetMatrix(render::Render::GetShadowMapMatrix(3));
    return wrapper;
}

Wrapper uniform_shadowsplit0_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetShadowSplitValue(0));
    return wrapper;
}

Wrapper uniform_shadowsplit1_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetShadowSplitValue(1));
    return wrapper;
}

Wrapper uniform_shadowsplit2_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetShadowSplitValue(2));
    return wrapper;
}

Wrapper uniform_look_at_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    math::Vector pos = scene::Scene::GetCamera(scene::PRIMIAY_CAM)->GetPos();
    math::Vector target = scene::Scene::GetCamera(scene::PRIMIAY_CAM)->GetTarget();
    math::Vector look_at = pos - target;
    look_at.Normalize();
    wrapper.SetVector(look_at);

    return wrapper;
}

Wrapper uniform_shadowmindex_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_INT);
    wrapper.SetInt(render::Render::GetCurShadowMapIndex());
    return wrapper;
}

Wrapper uniform_shadowmap_width_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_INT);
    wrapper.SetInt(app::Application::GetShadowMapWidth());
    return wrapper;
}

Wrapper uniform_shadowmap_height_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_INT);
    wrapper.SetInt(app::Application::GetShadowMapHeight());
    return wrapper;
}

Wrapper uniform_trans_inv_worldm_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    if (obj != nullptr) {
        math::Matrix m = obj->GetWorldMatrix();
        m.Inverse();
        m.Transpose();
        wrapper.SetMatrix(m);
    } else {
        GLB_SAFE_ASSERT(false);
        math::Matrix m;
        m.MakeIdentityMatrix();
        wrapper.SetMatrix(m);
    }

    return wrapper;
}

Wrapper uniform_decal_viewm_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);
    wrapper.SetMatrix(render::Render::GetDecalViewMatrix());
    return wrapper;
}

Wrapper uniform_decal_projm_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);
    wrapper.SetMatrix(render::Render::GetDecalProjMatrix());
    return wrapper;
}

Wrapper uniform_shadow0_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::Render::GetShadowMap(0));
    return wrapper;
}

Wrapper uniform_shadow1_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::Render::GetShadowMap(1));
    return wrapper;
}

Wrapper uniform_shadow2_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::Render::GetShadowMap(2));
    return wrapper;
}

Wrapper uniform_shadow3_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::Render::GetShadowMap(3));
    return wrapper;
}

Wrapper uniform_brdf_pft_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::Render::GetBRDFMap());
    return wrapper;
}

Wrapper uniform_light0_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_LIGHT0);
    return wrapper;
}

Wrapper uniform_light1_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_LIGHT1);
    return wrapper;
}

Wrapper uniform_light2_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_LIGHT2);
    return wrapper;
}

Wrapper uniform_decal_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::Render::GetDecalTexture());
    return wrapper;
}

Wrapper uniform_eye_pos_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    scene::CameraBase* cam = scene::Scene::GetCamera(scene::PRIMIAY_CAM);
    if (cam != nullptr) {
        wrapper.SetVector(cam->GetPos());
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(0.0f, 0.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_global_light_ambient_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one global ambient light now
    bool bFound = false;
    for (int32_t i = 0; i < scene::kMaxLight; i++) {
        scene::Light lit = scene::Scene::GetLight(i);
        if (lit.type == scene::AMBIENT_LIGHT) {
            wrapper.SetVector(lit.color);
            bFound = true;
            break;
        }
    }

    if (!bFound) {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(0.5f, 0.5f, 0.5f));
    }

    return wrapper;
}

Wrapper uniform_parallel_light_dir_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    bool bFound = false;
    for (int32_t i = 0; i < scene::kMaxLight; i++) {
        scene::Light lit = scene::Scene::GetLight(i);
        if (lit.type == scene::PARALLEL_LIGHT) {
            wrapper.SetVector(lit.dir);
            bFound = true;
            break;
        }
    }

    if (!bFound) {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(0.0f, 1.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_parallel_light_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    bool bFound = false;
    for (int32_t i = 0; i < scene::kMaxLight; i++) {
        scene::Light lit = scene::Scene::GetLight(i);
        if (lit.type == scene::PARALLEL_LIGHT) {
            wrapper.SetVector(lit.color);
            bFound = true;
            break;
        }
    }

    if (!bFound) {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(0.5f, 0.5f, 0.5f));
    }

    return wrapper;
}

Wrapper uniform_skylight_diffuse_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLERCUBE);

    wrapper.SetSamplerCube(render::Render::GetDiffuseSkyLightCubeMap());

    return wrapper;
}

Wrapper uniform_skylight_specular_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLERCUBE);

    wrapper.SetSamplerCube(render::Render::GetSpecularSkyLightCubeMap());

    return wrapper;
}

Wrapper uniform_skylight_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one sky light now
    bool bFound = false;
    for (int32_t i = 0; i < scene::kMaxLight; i++) {
        scene::Light lit = scene::Scene::GetLight(i);
        if (lit.type == scene::SKY_LIGHT) {
            wrapper.SetVector(lit.color);
            bFound = true;
            break;
        }
    }

    if (!bFound) {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(1.0f, 1.0f, 1.0f));
    }

    return wrapper;
}

Wrapper uniform_skylight_lod_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);

    wrapper.SetFloat(render::Render::GetSpecularSkyPFCLOD());

    return wrapper;
}

Wrapper uniform_far_clip_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetFarClip());
    return wrapper;
}

Wrapper uniform_screen_width_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(static_cast<float>(render::Render::GetScreenWidth()));
    return wrapper;
}

Wrapper uniform_screen_height_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(static_cast<float>(render::Render::GetScreenHeight()));
    return wrapper;
}

//---------------------------------------------------------------------

bool IsInternalParameter(const char* name) {
    bool result = false;

    for (auto& pick : kEngineUniforms) {
        if (!strcmp(pick.name, name)) {
            result = true;
            break;
        }
    }

    return result;
}

};  // namespace uniform

}; // namespace render

};  // namespace glb