//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 24
// Brief: Manage uniforms
//-----------------------------------------------------------
#ifndef GLB_GLBUNIFORM_H_
#define GLB_GLBUNIFORM_H_

#include <stdint.h>

#include "glbmatrix.h"
#include "glbscene.h"
#include "glbvector.h"

namespace glb {

class Object;

namespace uniform {

// Uniform enum
enum {
    GLB_PROJM = 0,
    GLB_VIEWM,
    GLB_WORLDM,
    GLB_SHADOWM,
    GLB_TRANS_INV_WORLDM,
    GLB_DIFFUSETEX,
    GLB_ALPHATEX,
    GLB_NORMALTEX,
    GLB_SHADOWTEX,
    GLB_MATERIAL_AMBIENT,
    GLB_MATERIAL_DIFFUSE,
    GLB_MATERIAL_SPECULAR,
    GLB_MATERIAL_EMISSION,
    GLB_MATERIAL_POW,
    GLB_EYEPOS,
    GLB_PARALLELLIGHT_DIR,
    GLB_PARALLELLIGHT_AMBIENT,
    GLB_PARALLELLIGHT_DIFFUSE,
    GLB_PARALLELLIGHT_SPECULAR,
    GLB_HDR_AVERAGE_LUM,
    GLB_HDR_SCENE_TEX,
    GLB_LOG_LUM_TEX,
    GLB_BLOOM_TEX_WIDTH,
    GLB_BLOOM_TEX_HEIGHT,
    GLB_HDR_BLOOM_TEX,
    GLB_FAR_CLIP,
    GLB_SCREEN_WIDTH,
    GLB_SCREEN_HEIGHT,
    GLB_DEPTH_TEX,
    GLB_RANDOM_ROTATE_TEX,
    GLB_AO_TEX,
    GLB_BIBLUR_TEX,
    GLB_EXPOSURE_LEVEL,
};

// Uniform table
static const struct {
    char        name[64];
    int32_t     id;
    int32_t     flag; // 1: Scene uniform 0: Object uniform
} kEngineUniforms[] = {
    {"glb_ProjM",                           GLB_PROJM,                          1},
    {"glb_ViewM",                           GLB_VIEWM,                          1},
    {"glb_WorldM",                          GLB_WORLDM,                         0},
    {"glb_ShadowM",                         GLB_SHADOWM,                        1},
    {"glb_Trans_Inv_WorldM",                GLB_TRANS_INV_WORLDM,               0},
    {"glb_DiffuseTex",                      GLB_DIFFUSETEX,                     0},
    {"glb_AlphaTex",                        GLB_ALPHATEX,                       0},
    {"glb_NormalTex",                       GLB_NORMALTEX,                      0},
    {"glb_ShadowTex",                       GLB_SHADOWTEX,                      0},
    {"glb_Material_Ambient",                GLB_MATERIAL_AMBIENT,               0},
    {"glb_Material_Diffuse",                GLB_MATERIAL_DIFFUSE,               0},
    {"glb_Material_Specular",               GLB_MATERIAL_SPECULAR,              0},
    {"glb_Material_Emission",               GLB_MATERIAL_EMISSION,              0},
    {"glb_Material_Pow",                    GLB_MATERIAL_POW,                   0},
    {"glb_EyePos",                          GLB_EYEPOS,                         1},
    {"glb_ParallelLight_Dir",               GLB_PARALLELLIGHT_DIR,              1},
    {"glb_ParallelLight_Ambient",           GLB_PARALLELLIGHT_AMBIENT,          1},
    {"glb_ParallelLight_Diffuse",           GLB_PARALLELLIGHT_DIFFUSE,          1},
    {"glb_ParallelLight_Specular",          GLB_PARALLELLIGHT_SPECULAR,         1},
    {"glb_AverageLum",                      GLB_HDR_AVERAGE_LUM,                1},
    {"glb_HDRSceneTex",                     GLB_HDR_SCENE_TEX,                  1},
    {"glb_LogLumTex",                       GLB_LOG_LUM_TEX,                    1},
    {"glb_BloomTexWidth",                   GLB_BLOOM_TEX_WIDTH,                1},
    {"glb_BloomTexHeight",                  GLB_BLOOM_TEX_HEIGHT,               1},
    {"glb_Bloom",                           GLB_HDR_BLOOM_TEX,                  1},
    {"glb_FarClip",                         GLB_FAR_CLIP,                       1},
    {"glb_ScreenWidth",                     GLB_SCREEN_WIDTH,                   1},
    {"glb_ScreenHeight",                    GLB_SCREEN_HEIGHT,                  1},
    {"glb_DepthMap",                        GLB_DEPTH_TEX,                      1},
    {"glb_RandRotateMap",                   GLB_RANDOM_ROTATE_TEX,              1},
    {"glb_AOMap",                           GLB_AO_TEX,                         1},
    {"glb_BiBlurMap",                       GLB_BIBLUR_TEX,                     1},
    {"glb_ExposureLevel",                   GLB_EXPOSURE_LEVEL,                 1},
};

// Uniform wrapper
class Wrapper {
public:
    enum {
        FMT_INT,
        FMT_FLOAT,
        FMT_FLOAT3,
        FMT_MATRIX,
        FMT_SAMPLER,
    };

public:
    Wrapper();
    virtual ~Wrapper();

public:
    void SetFloat(float v);
    float GetFloat();

    void SetVector(Vector v);
    Vector& GetVector();

    void SetInt(int v);
    int GetInt();

    void SetSampler(int32_t sampler);
    int32_t GetSampler();

    void SetMatrix(Matrix m);
    Matrix& GetMatrix();

    void SetFormat(int32_t format);
    int32_t GetFormat();

private:
    int32_t                 m_Int;
    int32_t                 m_Sampler;
    float                   m_Float;
    Vector                  m_Vector;
    Matrix                  m_Matrix;
    int32_t                 m_Format;
};


// Uniform picker
typedef Wrapper (*Uniform_Picker)(Object*);

Wrapper uniform_projm_picker(Object*);                     // Pick glb_ProjM
Wrapper uniform_viewm_picker(Object*);                     // Pick glb_ViewM
Wrapper uniform_worldm_picker(Object*);                    // Pick glb_WorldM
Wrapper uniform_shadowm_picker(Object*);                   // Pick glb_ShadowM
Wrapper uniform_trans_inv_worldm_picker(Object*);          // Pick glb_Trans_Inv_WorldM
Wrapper uniform_diffuse_texslot_picker(Object*);           // Pick glb_DiffuseTex
Wrapper uniform_alpha_texslot_picker(Object*);             // Pick glb_AlphaTex
Wrapper uniform_normal_texslot_picker(Object*);            // Pick glb_NormalTex
Wrapper uniform_shadow_texslot_picker(Object*);            // Pick glb_ShadowTex
Wrapper uniform_material_ambient_picker(Object*);          // Pick glb_Material_Ambient
Wrapper uniform_material_diffuse_picker(Object*);          // Pick glb_Material_Diffuse
Wrapper uniform_material_specular_picker(Object*);         // Pick glb_Material_Specular
Wrapper uniform_material_emission_picker(Object*);         // Pick glb_Material_Emission
Wrapper uniform_material_pow_picker(Object*);              // Pick glb_Material_Pow
Wrapper uniform_eye_pos_picker(Object*);                   // Pick glb_EyePos
Wrapper uniform_parallel_light_dir_picker(Object*);        // Pick glb_ParallelLight_Dir
Wrapper uniform_parallel_light_ambient_picker(Object*);    // Pick glb_ParallelLight_Ambient
Wrapper uniform_parallel_light_diffuse_picker(Object*);    // Pick glb_ParallelLight_Diffuse
Wrapper uniform_parallel_light_specular_picker(Object*);   // Pick glb_ParallelLight_Specular
Wrapper uniform_hdr_average_lum_picker(Object*);           // Pick glb_AverageLum
Wrapper uniform_hdr_scene_tex_picker(Object*);             // Pick glb_HDRSceneTex
Wrapper uniform_log_lum_tex_picker(Object*);               // Pick glb_LogLumTex
Wrapper uniform_bloom_tex_width_picker(Object*);           // Pick glb_BloomTexWidth
Wrapper uniform_bloom_tex_height_picker(Object*);          // Pick glb_BloomTexHeight
Wrapper uniform_hdr_bloom_tex_picker(Object*);             // Pick glb_Bloom
Wrapper uniform_far_clip_picker(Object*);                  // Pick glb_FarClip
Wrapper uniform_screen_width_picker(Object*);              // Pick glb_ScreenWidth
Wrapper uniform_screen_height_picker(Object*);             // Pick glb_ScreenHeight
Wrapper uniform_depth_tex_picker(Object*);                 // Pick glb_DepthMap
Wrapper uniform_random_rotate_tex_picker(Object*);         // Pick glb_RandRotateMap
Wrapper uniform_ao_tex_picker(Object*);                    // Pick glb_AOMap
Wrapper uniform_biblur_tex_picker(Object*);                // Pick glb_BiBlurMap
Wrapper uniform_exposure_level_picker(Object*);            // Pick glb_ExposureLevel

// Uniform picker table
static const struct {
    Uniform_Picker      picker;
    int                 id;
} kUniformPickers[] = {
    {uniform_projm_picker,                      GLB_PROJM},
    {uniform_viewm_picker,                      GLB_VIEWM},
    {uniform_worldm_picker,                     GLB_WORLDM},
    {uniform_shadowm_picker,                    GLB_SHADOWM},
    {uniform_trans_inv_worldm_picker,           GLB_TRANS_INV_WORLDM},
    {uniform_diffuse_texslot_picker,            GLB_DIFFUSETEX},
    {uniform_alpha_texslot_picker,              GLB_ALPHATEX},
    {uniform_normal_texslot_picker,             GLB_NORMALTEX},
    {uniform_shadow_texslot_picker,             GLB_SHADOWTEX},
    {uniform_material_ambient_picker,           GLB_MATERIAL_AMBIENT},
    {uniform_material_diffuse_picker,           GLB_MATERIAL_DIFFUSE},
    {uniform_material_specular_picker,          GLB_MATERIAL_SPECULAR},
    {uniform_material_emission_picker,          GLB_MATERIAL_EMISSION},
    {uniform_material_pow_picker,               GLB_MATERIAL_POW},
    {uniform_eye_pos_picker,                    GLB_EYEPOS},
    {uniform_parallel_light_dir_picker,         GLB_PARALLELLIGHT_DIR},
    {uniform_parallel_light_ambient_picker,     GLB_PARALLELLIGHT_AMBIENT},
    {uniform_parallel_light_diffuse_picker,     GLB_PARALLELLIGHT_DIFFUSE},
    {uniform_parallel_light_specular_picker,    GLB_PARALLELLIGHT_SPECULAR},
    {uniform_hdr_average_lum_picker,            GLB_HDR_AVERAGE_LUM},
    {uniform_hdr_scene_tex_picker,              GLB_HDR_SCENE_TEX},
    {uniform_log_lum_tex_picker,                GLB_LOG_LUM_TEX},
    {uniform_bloom_tex_width_picker,            GLB_BLOOM_TEX_WIDTH},
    {uniform_bloom_tex_height_picker,           GLB_BLOOM_TEX_HEIGHT},
    {uniform_hdr_bloom_tex_picker,              GLB_HDR_BLOOM_TEX},
    {uniform_far_clip_picker,                   GLB_FAR_CLIP},
    {uniform_screen_width_picker,               GLB_SCREEN_WIDTH},
    {uniform_screen_height_picker,              GLB_SCREEN_HEIGHT},
    {uniform_depth_tex_picker,                  GLB_DEPTH_TEX},
    {uniform_random_rotate_tex_picker,          GLB_RANDOM_ROTATE_TEX},
    {uniform_ao_tex_picker,                     GLB_AO_TEX},
    {uniform_biblur_tex_picker,                 GLB_BIBLUR_TEX},
    {uniform_exposure_level_picker,             GLB_EXPOSURE_LEVEL},
};

// Uniform entry
struct UniformEntry {
    uint32_t            location;
    int                 id;
    int32_t             flag;  // 0:Scene Uniforms 1:Object Uniforms
};

};  // namespace uniform

};  // namespace glb

#endif  // GLB_GLBUNIFORM_H_
