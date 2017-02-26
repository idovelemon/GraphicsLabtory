//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 24
// Brief: Manage uniforms
//-----------------------------------------------------------
#include "glbuniform.h"

#include "glbcamera.h"
#include "glbmaterial.h"
#include "glbobject.h"
#include "glbrender.h"
#include "glbrenderdevice.h"

namespace glb {

namespace uniform {

// Wrapper
Wrapper::Wrapper()
: m_Int(0)
, m_Float(0.0f)
, m_Sampler(0)
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

void Wrapper::SetSampler(int32_t sampler) {
    m_Sampler = sampler;
}

int32_t Wrapper::GetSampler() {
    return m_Sampler;
}

void Wrapper::SetVector(Vector v) {
    m_Vector = v;
}

Vector& Wrapper::GetVector() {
    return m_Vector;
}

void Wrapper::SetMatrix(Matrix v) {
    m_Matrix = v;
}

Matrix& Wrapper::GetMatrix() {
    return m_Matrix;
}

void Wrapper::SetFormat(int32_t format) {
    m_Format = format;
}

int32_t Wrapper::GetFormat() {
    return m_Format;
}

// Uniform picker
Wrapper uniform_projm_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    int32_t type = glb::render::Render::GetCurPerspectiveType();
    glb::Matrix proj = glb::render::Render::GetPerspective(type);
    wrapper.SetMatrix(proj);

    return wrapper;
}

Wrapper uniform_viewm_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    int32_t type = glb::scene::Scene::GetCurCamera();
    glb::camera::CameraBase* cam = glb::scene::Scene::GetCamera(type);
    if (cam != NULL) {
        wrapper.SetMatrix(cam->GetViewMatrix());
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return wrapper;
}

Wrapper uniform_worldm_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    if (obj != NULL) {
        Matrix m = obj->GetWorldMatrix();
        wrapper.SetMatrix(m);
    } else {
        GLB_SAFE_ASSERT(false);
        Matrix m;
        m.MakeIdentityMatrix();
        wrapper.SetMatrix(m);
    }

    return wrapper;
}

Wrapper uniform_shadowm_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    // Calculate the center of the boundbox
    Vector max = scene::Scene::GetSceneBoundBoxMax();
    Vector min = scene::Scene::GetSceneBoundBoxMin();
    Vector center = (max + min);
    center /= 2.0f;

    // Transform into a fake light space but has correctly orientation
    Matrix fake_light_view;
    fake_light_view.MakeViewMatrix(Vector(0.0f, 0.0f, 0.0f), scene::Scene::GetLight(0).dir);

    Vector view_space_points[8];
    view_space_points[render::Render::NLU] = Vector(min.x, max.y, max.z);
    view_space_points[render::Render::NLD] = Vector(min.x, min.y, max.z);
    view_space_points[render::Render::NRU] = Vector(max.x, max.y, max.z);
    view_space_points[render::Render::NRD] = Vector(max.x, min.y, max.z);
    view_space_points[render::Render::FLU] = Vector(min.x, max.y, min.z);
    view_space_points[render::Render::FLD] = Vector(min.x, min.y, min.z);
    view_space_points[render::Render::FRU] = Vector(max.x, max.y, min.z);
    view_space_points[render::Render::FRD] = Vector(max.x, min.y, min.z);

    for (int32_t i = 0; i < 8; i++) {
        view_space_points[i] = fake_light_view * view_space_points[i];
    }

    // Calculate the size of orthogonal frustum
    float min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX;
    float max_x = 0.0f, max_y = 0.0f, max_z = 0.0f;
    for (int32_t i = 0; i < 8; i++) {
        if (view_space_points[i].x < min_x) {
            min_x = view_space_points[i].x;
        }
        if (view_space_points[i].y < min_y) {
            min_y = view_space_points[i].y;
        }
        if (view_space_points[i].z < min_z) {
            min_z = view_space_points[i].z;
        }
        if (view_space_points[i].x > max_x) {
            max_x = view_space_points[i].x;
        }
        if (view_space_points[i].y > max_y) {
            max_y = view_space_points[i].y;
        }
        if (view_space_points[i].z > max_z) {
            max_z = view_space_points[i].z;
        }
    }
    float width = (max_x - min_x) / 2.0f;
    float height = (max_y - min_y) / 2.0f;
    float depth = (max_z - min_z) / 2.0f;

    // Build shadow matrix
    Matrix orth_matrix;
    orth_matrix.MakeOrthogonalMatrix(-width, width, -height, height, -depth, depth);
    Matrix view_matrix;
    view_matrix.MakeViewMatrix(center, center + glb::scene::Scene::GetLight(0).dir);

    Matrix result;
    result.MakeIdentityMatrix();
    result.Mul(orth_matrix);
    result.Mul(view_matrix);

    wrapper.SetMatrix(result);

    return wrapper;
}

Wrapper uniform_trans_inv_worldm_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_MATRIX);

    if (obj != NULL) {
        Matrix m = obj->GetWorldMatrix();
        m.Inverse();
        m.Transpose();
        wrapper.SetMatrix(m);
    } else {
        GLB_SAFE_ASSERT(false);
        Matrix m;
        m.MakeIdentityMatrix();
        wrapper.SetMatrix(m);
    }

    return wrapper;
}

Wrapper uniform_diffuse_texslot_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);

    if (obj != NULL) {
        wrapper.SetSampler(render::TS_DIFFUSE);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetSampler(0);
    }

    return wrapper;
}

Wrapper uniform_alpha_texslot_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);

    if (obj != NULL) {
        wrapper.SetSampler(render::TS_ALPHA);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetSampler(0);
    }

    return wrapper;
}

Wrapper uniform_shadow_texslot_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);
    wrapper.SetSampler(render::TS_SHADOW);
    return wrapper;
}

Wrapper uniform_material_ambient_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    if (obj != NULL) {
        int32_t mat_id = obj->GetModel()->GetMaterial();
        wrapper.SetVector(material::Mgr::GetMaterial(mat_id).ambient);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(Vector(0.0f, 0.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_material_diffuse_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    if (obj != NULL) {
        int32_t mat_id = obj->GetModel()->GetMaterial();
        wrapper.SetVector(material::Mgr::GetMaterial(mat_id).diffuse);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(Vector(0.0f, 0.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_material_specular_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    if (obj != NULL) {
        int32_t mat_id = obj->GetModel()->GetMaterial();
        wrapper.SetVector(material::Mgr::GetMaterial(mat_id).specular);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(Vector(0.0f, 0.0f, 0.0f));
    }


    return wrapper;
}

Wrapper uniform_material_emission_picker(glb::Object* obj) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    if (obj != NULL) {
        int32_t mat_id = obj->GetModel()->GetMaterial();
        wrapper.SetVector(material::Mgr::GetMaterial(mat_id).emission);
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(Vector(0.0f, 0.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_material_pow_picker(glb::Object* obj) {
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

Wrapper uniform_eye_pos_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    glb::camera::CameraBase* cam = glb::scene::Scene::GetCamera(glb::scene::PRIMIAY_CAM);
    if (cam != NULL) {
        wrapper.SetVector(cam->GetPos());
    } else {
        GLB_SAFE_ASSERT(false);
        wrapper.SetVector(Vector(0.0f, 0.0f, 0.0f));
    }

    return wrapper;
}

Wrapper uniform_parallel_light_dir_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    glb::light::Light lit = glb::scene::Scene::GetLight(0);
    wrapper.SetVector(lit.dir);

    return wrapper;
}

Wrapper uniform_parallel_light_ambient_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    glb::light::Light lit = glb::scene::Scene::GetLight(0);
    wrapper.SetVector(lit.ambient);

    return wrapper;
}

Wrapper uniform_parallel_light_diffuse_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    glb::light::Light lit = glb::scene::Scene::GetLight(0);
    wrapper.SetVector(lit.diffuse);

    return wrapper;
}

Wrapper uniform_parallel_light_specular_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT3);

    // Only support one parallel light now
    glb::light::Light lit = glb::scene::Scene::GetLight(0);
    wrapper.SetVector(lit.specular);

    return wrapper;
}

Wrapper uniform_hdr_average_lum_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(glb::render::Render::GetHDRAverageLum());
    return wrapper;
}

Wrapper uniform_hdr_scene_tex_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);
    wrapper.SetSampler(render::TS_HDRSCENE);
    return wrapper;
}

Wrapper uniform_log_lum_tex_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);
    wrapper.SetSampler(render::TS_LOG_LUM);
    return wrapper;
}

Wrapper uniform_bloom_tex_width_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(glb::render::Render::GetBloomWidth());
    return wrapper;
}

Wrapper uniform_bloom_tex_height_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(glb::render::Render::GetBloomHeight());
    return wrapper;
}

Wrapper uniform_hdr_bloom_tex_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);
    wrapper.SetSampler(render::TS_HDR_BLOOM);
    return wrapper;
}

Wrapper uniform_far_clip_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetFarClip());
    return wrapper;
}

Wrapper uniform_screen_width_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetScreenWidth());
    return wrapper;
}

Wrapper uniform_screen_height_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetScreenHeight());
    return wrapper;
}

Wrapper uniform_depth_tex_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);
    wrapper.SetSampler(render::TS_DEPTH);
    return wrapper;
}

Wrapper uniform_random_rotate_tex_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);
    wrapper.SetSampler(render::TS_RANDOM_ROTATE);
    return wrapper;
}

Wrapper uniform_ao_tex_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);
    wrapper.SetSampler(render::TS_AO_MAP);
    return wrapper;
}

Wrapper uniform_biblur_tex_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_SAMPLER);
    wrapper.SetSampler(render::TS_BI_BLUR_MAP);
    return wrapper;
}

Wrapper uniform_exposure_level_picker(glb::Object*) {
    Wrapper wrapper;
    wrapper.SetFormat(Wrapper::FMT_FLOAT);
    wrapper.SetFloat(render::Render::GetExposureLevel());
    return wrapper;
}
};  // namespace uniform

};  // namespace glb