//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 24
// Brief: Manage uniforms
//-----------------------------------------------------------
#include "glbuniform.h"

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
    if (cam != NULL) {
        wrapper.SetMatrix(cam->GetViewMatrix());
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return wrapper;
}

Wrapper uniform_worldm_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    if (obj != NULL) {
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

Wrapper uniform_trans_inv_worldm_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    if (obj != NULL) {
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

Wrapper uniform_diffuse_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);

    if (obj != NULL) {
        wrapper.SetSampler2D(render::TS_DIFFUSE);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetSampler2D(0);
    }

    return wrapper;
}

Wrapper uniform_alpha_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);

    if (obj != NULL) {
        wrapper.SetSampler2D(render::TS_ALPHA);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetSampler2D(0);
    }

    return wrapper;
}

Wrapper uniform_normal_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);

    if (obj != NULL) {
        wrapper.SetSampler2D(render::TS_NORMAL);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetSampler2D(0);
    }

    return wrapper;
}

Wrapper uniform_reflect_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLERCUBE);
    wrapper.SetSamplerCube(render::TS_REFLECT);
    return wrapper;
}

Wrapper uniform_shadow0_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_SHADOW0);
    return wrapper;
}

Wrapper uniform_shadow1_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_SHADOW1);
    return wrapper;
}

Wrapper uniform_shadow2_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_SHADOW2);
    return wrapper;
}

Wrapper uniform_shadow3_texslot_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_SHADOW3);
    return wrapper;
}

Wrapper uniform_material_ambient_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    if (obj != NULL) {
        int32_t mat_id = obj->GetModel()->GetMaterial();
        wrapper.SetVector(material::Mgr::GetMaterial(mat_id).ambient);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(0.0f, 0.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_material_diffuse_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    if (obj != NULL) {
        int32_t mat_id = obj->GetModel()->GetMaterial();
        wrapper.SetVector(material::Mgr::GetMaterial(mat_id).diffuse);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(0.0f, 0.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_material_specular_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    if (obj != NULL) {
        int32_t mat_id = obj->GetModel()->GetMaterial();
        wrapper.SetVector(material::Mgr::GetMaterial(mat_id).specular);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(0.0f, 0.0f, 0.0f));
    }


    return wrapper;
}

Wrapper uniform_material_emission_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    if (obj != NULL) {
        int32_t mat_id = obj->GetModel()->GetMaterial();
        wrapper.SetVector(material::Mgr::GetMaterial(mat_id).emission);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(0.0f, 0.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_material_pow_picker(scene::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);

    if (obj != NULL) {
        int32_t mat_id = obj->GetModel()->GetMaterial();
        wrapper.SetFloat(material::Mgr::GetMaterial(mat_id).specular_pow);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetFloat(0.0f);
    }

    return wrapper;
}

Wrapper uniform_eye_pos_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    scene::CameraBase* cam = scene::Scene::GetCamera(scene::PRIMIAY_CAM);
    if (cam != NULL) {
        wrapper.SetVector(cam->GetPos());
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(math::Vector(0.0f, 0.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_parallel_light_dir_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    scene::Light lit = scene::Scene::GetLight(0);
    wrapper.SetVector(lit.dir);

    return wrapper;
}

Wrapper uniform_parallel_light_ambient_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    scene::Light lit = scene::Scene::GetLight(0);
    wrapper.SetVector(lit.ambient);

    return wrapper;
}

Wrapper uniform_parallel_light_diffuse_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    scene::Light lit = scene::Scene::GetLight(0);
    wrapper.SetVector(lit.diffuse);

    return wrapper;
}

Wrapper uniform_parallel_light_specular_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    scene::Light lit = scene::Scene::GetLight(0);
    wrapper.SetVector(lit.specular);

    return wrapper;
}

Wrapper uniform_hdr_average_lum_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetHDRAverageLum());
    return wrapper;
}

Wrapper uniform_hdr_scene_tex_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_HDRSCENE);
    return wrapper;
}

Wrapper uniform_log_lum_tex_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_LOG_LUM);
    return wrapper;
}

Wrapper uniform_bloom_tex_width_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetBloomWidth());
    return wrapper;
}

Wrapper uniform_bloom_tex_height_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetBloomHeight());
    return wrapper;
}

Wrapper uniform_hdr_bloom_tex_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_HDR_BLOOM);
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

Wrapper uniform_depth_tex_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_DEPTH);
    return wrapper;
}

Wrapper uniform_random_rotate_tex_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_RANDOM_ROTATE);
    return wrapper;
}

Wrapper uniform_ao_tex_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_AO_MAP);
    return wrapper;
}

Wrapper uniform_biblur_tex_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER2D);
    wrapper.SetSampler2D(render::TS_BI_BLUR_MAP);
    return wrapper;
}

Wrapper uniform_exposure_level_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetExposureLevel());
    return wrapper;
}

Wrapper uniform_high_light_base_picker(scene::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetHighLightBase());
    return wrapper;
}
};  // namespace uniform

}; // namespace render

};  // namespace glb