//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 24
// Brief: Manage uniforms
//-----------------------------------------------------------
#ifndef GLB_GLBUNIFORM_H_
#define GLB_GLBUNIFORM_H_

#include <stdint.h>

#include "math/glbmatrix.h"
#include "math/glbvector.h"

#include "scene/glbscene.h"

namespace glb {

class Object;

namespace render {

namespace uniform {

// Uniform enum
enum {
    GLB_PROJM = 0,
    GLB_VIEWM,
    GLB_WORLDM,
    GLB_SHADOWM0,
    GLB_SHADOWM1,
    GLB_SHADOWM2,
    GLB_SHADOWM3,
    GLB_SHADOWSPLIT0,
    GLB_SHADOWSPLIT1,
    GLB_SHADOWSPLIT2,
    GLB_SHADOWMINDEX,
    GLB_TRANS_INV_WORLDM,
    GLB_ALBEDOTEX,
    GLB_ROUGHNESSTEX,
    GLB_METALLICTEX,
    GLB_ALPHATEX,
    GLB_NORMALTEX,
    GLB_EMISSIONTEX,
    GLB_REFLECT_TEX,
    GLB_SHADOWTEX0,
    GLB_SHADOWTEX1,
    GLB_SHADOWTEX2,
    GLB_SHADOWTEX3,
    GLB_DIFFUSEPFCTEX,
    GLB_SPECULARPFCTEX,
    GLB_BRDFPFTTEX,
    GLB_MATERIAL_AMBIENT,
    GLB_MATERIAL_DIFFUSE,
    GLB_MATERIAL_SPECULAR,
    GLB_MATERIAL_EMISSION,
    GLB_MATERIAL_POW,
    GLB_MATERIAL_ALBEDO,
    GLB_MATERIAL_ROUGHNESS,
    GLB_MATERIAL_METALLIC,
    GLB_EYEPOS,
    GLB_LOOKAT,
    GLB_GLOBALLIGHT_AMBIENT,
    GLB_PARALLELLIGHT_DIR,
    GLB_PARALLELLIGHT,
    GLB_FAR_CLIP,
    GLB_SCREEN_WIDTH,
    GLB_SCREEN_HEIGHT,
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
    {"glb_ShadowM0",                        GLB_SHADOWM0,                       1},
    {"glb_ShadowM1",                        GLB_SHADOWM1,                       1},
    {"glb_ShadowM2",                        GLB_SHADOWM2,                       1},
    {"glb_ShadowM3",                        GLB_SHADOWM3,                       1},
    {"glb_ShadowSplit0",                    GLB_SHADOWSPLIT0,                   1},
    {"glb_ShadowSplit1",                    GLB_SHADOWSPLIT1,                   1},
    {"glb_ShadowSplit2",                    GLB_SHADOWSPLIT2,                   1},
    {"glb_ShadowMIndex",                    GLB_SHADOWMINDEX,                   1},
    {"glb_Trans_Inv_WorldM",                GLB_TRANS_INV_WORLDM,               0},
    {"glb_AlbedoTex",                       GLB_ALBEDOTEX,                      0},
    {"glb_RoughnessTex",                    GLB_ROUGHNESSTEX,                   0},
    {"glb_MetallicTex",                     GLB_METALLICTEX,                    0},
    {"glb_AlphaTex",                        GLB_ALPHATEX,                       0},
    {"glb_NormalTex",                       GLB_NORMALTEX,                      0},
    {"glb_EmissionTex",                     GLB_EMISSIONTEX,                    0},
    {"glb_ReflectTex",                      GLB_REFLECT_TEX,                    0},
    {"glb_ShadowTex0",                      GLB_SHADOWTEX0,                     0},
    {"glb_ShadowTex1",                      GLB_SHADOWTEX1,                     0},
    {"glb_ShadowTex2",                      GLB_SHADOWTEX2,                     0},
    {"glb_ShadowTex3",                      GLB_SHADOWTEX3,                     0},
    {"glb_DiffusePFCTex",                   GLB_DIFFUSEPFCTEX,                  0},
    {"glb_SpecularPFCTex",                  GLB_SPECULARPFCTEX,                 0},
    {"glb_BRDFPFTTex",                      GLB_BRDFPFTTEX,                     1},
    {"glb_Material_Ambient",                GLB_MATERIAL_AMBIENT,               0},
    {"glb_Material_Diffuse",                GLB_MATERIAL_DIFFUSE,               0},
    {"glb_Material_Specular",               GLB_MATERIAL_SPECULAR,              0},
    {"glb_Material_Emission",               GLB_MATERIAL_EMISSION,              0},
    {"glb_Material_Pow",                    GLB_MATERIAL_POW,                   0},
    {"glb_Material_Albedo",                 GLB_MATERIAL_ALBEDO   ,             0},
    {"glb_Material_Roughness",              GLB_MATERIAL_ROUGHNESS,             0},
    {"glb_Material_Metallic",               GLB_MATERIAL_METALLIC,              0},
    {"glb_EyePos",                          GLB_EYEPOS,                         1},
    {"glb_LookAt",                          GLB_LOOKAT,                         1},
    {"glb_GlobalLight_Ambient",             GLB_GLOBALLIGHT_AMBIENT,            1},
    {"glb_ParallelLight_Dir",               GLB_PARALLELLIGHT_DIR,              1},
    {"glb_ParallelLight",                   GLB_PARALLELLIGHT,                  1},
    {"glb_FarClip",                         GLB_FAR_CLIP,                       1},
    {"glb_ScreenWidth",                     GLB_SCREEN_WIDTH,                   1},
    {"glb_ScreenHeight",                    GLB_SCREEN_HEIGHT,                  1},
};

// Uniform wrapper
class Wrapper {
public:
    enum {
        FMT_INT,
        FMT_FLOAT,
        FMT_FLOAT3,
        FMT_MATRIX,
        FMT_SAMPLER2D,
        FMT_SAMPLERCUBE,
    };

public:
    Wrapper();
    virtual ~Wrapper();

public:
    void SetFloat(float v);
    float GetFloat();

    void SetVector(math::Vector v);
    math::Vector& GetVector();

    void SetInt(int v);
    int GetInt();

    void SetSampler2D(int32_t sampler);
    int32_t GetSampler2D();

    void SetSamplerCube(int32_t sampler);
    int32_t GetSamplerCube();

    void SetMatrix(math::Matrix m);
    math::Matrix& GetMatrix();

    void SetFormat(int32_t format);
    int32_t GetFormat();

private:
    int32_t                 m_Int;
    int32_t                 m_Sampler2D;
    int32_t                 m_SamplerCube;
    float                   m_Float;
    math::Vector            m_Vector;
    math::Matrix            m_Matrix;
    int32_t                 m_Format;
};


// Uniform picker
typedef Wrapper (*Uniform_Picker)(scene::Object*);

Wrapper uniform_projm_picker(scene::Object*);                     // Pick glb_ProjM
Wrapper uniform_viewm_picker(scene::Object*);                     // Pick glb_ViewM
Wrapper uniform_worldm_picker(scene::Object*);                    // Pick glb_WorldM
Wrapper uniform_shadowm0_picker(scene::Object*);                  // Pick glb_ShadowM0
Wrapper uniform_shadowm1_picker(scene::Object*);                  // Pick glb_ShadowM1
Wrapper uniform_shadowm2_picker(scene::Object*);                  // Pick glb_ShadowM2
Wrapper uniform_shadowm3_picker(scene::Object*);                  // Pick glb_ShadowM3
Wrapper uniform_shadowsplit0_picker(scene::Object*);              // Pick glb_ShadowSplit0
Wrapper uniform_shadowsplit1_picker(scene::Object*);              // Pick glb_ShadowSplit1
Wrapper uniform_shadowsplit2_picker(scene::Object*);              // Pick glb_ShadowSplit2
Wrapper uniform_shadowmindex_picker(scene::Object*);              // Pick glb_ShadowMIndex
Wrapper uniform_trans_inv_worldm_picker(scene::Object*);          // Pick glb_Trans_Inv_WorldM
Wrapper uniform_albedo_texslot_picker(scene::Object*);            // Pick glb_AlbedoTex
Wrapper uniform_roughness_texslot_picker(scene::Object*);         // Pick glb_RoughnessTex
Wrapper uniform_metallic_texslot_picker(scene::Object*);          // Pick glb_MetallicTex
Wrapper uniform_alpha_texslot_picker(scene::Object*);             // Pick glb_AlphaTex
Wrapper uniform_normal_texslot_picker(scene::Object*);            // Pick glb_NormalTex
Wrapper uniform_emission_texslot_picker(scene::Object*);          // Pick glb_EmissionTex
Wrapper uniform_reflect_texslot_picker(scene::Object*);           // Pick glb_ReflectTex
Wrapper uniform_shadow0_texslot_picker(scene::Object*);           // Pick glb_ShadowTex0
Wrapper uniform_shadow1_texslot_picker(scene::Object*);           // Pick glb_ShadowTex1
Wrapper uniform_shadow2_texslot_picker(scene::Object*);           // Pick glb_ShadowTex2
Wrapper uniform_shadow3_texslot_picker(scene::Object*);           // Pick glb_ShadowTex3
Wrapper uniform_diffuse_pfc_texslot_picker(scene::Object* obj);   // Pick glb_DiffusePFC
Wrapper uniform_specular_pfc_texslot_picker(scene::Object* obj);  // Pick glb_SpecularPFC
Wrapper uniform_brdf_pft_texslot_picker(scene::Object* obj);      // Pick glb_BRDFPFT
Wrapper uniform_material_ambient_picker(scene::Object*);          // Pick glb_Material_Ambient
Wrapper uniform_material_diffuse_picker(scene::Object*);          // Pick glb_Material_Diffuse
Wrapper uniform_material_specular_picker(scene::Object*);         // Pick glb_Material_Specular
Wrapper uniform_material_emission_picker(scene::Object*);         // Pick glb_Material_Emission
Wrapper uniform_material_pow_picker(scene::Object*);              // Pick glb_Material_Pow
Wrapper uniform_material_albedo_picker(scene::Object*);           // Pick glb_Material_Albedo
Wrapper uniform_material_roughness_picker(scene::Object*);        // Pick glb_Material_Roughness
Wrapper uniform_material_metallic_picker(scene::Object*);         // Pick glb_Material_Metallic
Wrapper uniform_eye_pos_picker(scene::Object*);                   // Pick glb_EyePos
Wrapper uniform_look_at_picker(scene::Object*);                   // Pick glb_LookAt
Wrapper uniform_global_light_ambient_picker(scene::Object*);      // Pick glb_GlobalLight_Ambient
Wrapper uniform_parallel_light_dir_picker(scene::Object*);        // Pick glb_ParallelLight_Dir
Wrapper uniform_parallel_light_picker(scene::Object*);            // Pick glb_ParallelLight
Wrapper uniform_far_clip_picker(scene::Object*);                  // Pick glb_FarClip
Wrapper uniform_screen_width_picker(scene::Object*);              // Pick glb_ScreenWidth
Wrapper uniform_screen_height_picker(scene::Object*);             // Pick glb_ScreenHeight

// Uniform picker table
static const struct {
    Uniform_Picker      picker;
    int                 id;
} kUniformPickers[] = {
    {uniform_projm_picker,                      GLB_PROJM},
    {uniform_viewm_picker,                      GLB_VIEWM},
    {uniform_worldm_picker,                     GLB_WORLDM},
    {uniform_shadowm0_picker,                   GLB_SHADOWM0},
    {uniform_shadowm1_picker,                   GLB_SHADOWM1},
    {uniform_shadowm2_picker,                   GLB_SHADOWM2},
    {uniform_shadowm3_picker,                   GLB_SHADOWM3},
    {uniform_shadowsplit0_picker,               GLB_SHADOWSPLIT0},
    {uniform_shadowsplit1_picker,               GLB_SHADOWSPLIT1},
    {uniform_shadowsplit2_picker,               GLB_SHADOWSPLIT2},
    {uniform_shadowmindex_picker,               GLB_SHADOWMINDEX},
    {uniform_trans_inv_worldm_picker,           GLB_TRANS_INV_WORLDM},
    {uniform_albedo_texslot_picker,             GLB_ALBEDOTEX},
    {uniform_roughness_texslot_picker,          GLB_ROUGHNESSTEX},
    {uniform_metallic_texslot_picker,           GLB_METALLICTEX},
    {uniform_alpha_texslot_picker,              GLB_ALPHATEX},
    {uniform_normal_texslot_picker,             GLB_NORMALTEX},
    {uniform_emission_texslot_picker,           GLB_EMISSIONTEX},
    {uniform_reflect_texslot_picker,            GLB_REFLECT_TEX},
    {uniform_shadow0_texslot_picker,            GLB_SHADOWTEX0},
    {uniform_shadow1_texslot_picker,            GLB_SHADOWTEX1},
    {uniform_shadow2_texslot_picker,            GLB_SHADOWTEX2},
    {uniform_shadow3_texslot_picker,            GLB_SHADOWTEX3},
    {uniform_diffuse_pfc_texslot_picker,        GLB_DIFFUSEPFCTEX},
    {uniform_specular_pfc_texslot_picker,       GLB_SPECULARPFCTEX},
    {uniform_brdf_pft_texslot_picker,           GLB_BRDFPFTTEX},
    {uniform_material_ambient_picker,           GLB_MATERIAL_AMBIENT},
    {uniform_material_diffuse_picker,           GLB_MATERIAL_DIFFUSE},
    {uniform_material_specular_picker,          GLB_MATERIAL_SPECULAR},
    {uniform_material_emission_picker,          GLB_MATERIAL_EMISSION},
    {uniform_material_pow_picker,               GLB_MATERIAL_POW},
    {uniform_material_albedo_picker,            GLB_MATERIAL_ALBEDO},
    {uniform_material_roughness_picker,         GLB_MATERIAL_ROUGHNESS},
    {uniform_material_metallic_picker,          GLB_MATERIAL_METALLIC},
    {uniform_eye_pos_picker,                    GLB_EYEPOS},
    {uniform_look_at_picker,                    GLB_LOOKAT},
    {uniform_global_light_ambient_picker,       GLB_GLOBALLIGHT_AMBIENT},
    {uniform_parallel_light_dir_picker,         GLB_PARALLELLIGHT_DIR},
    {uniform_parallel_light_picker,             GLB_PARALLELLIGHT},
    {uniform_far_clip_picker,                   GLB_FAR_CLIP},
    {uniform_screen_width_picker,               GLB_SCREEN_WIDTH},
    {uniform_screen_height_picker,              GLB_SCREEN_HEIGHT},
};

// Uniform entry
struct UniformEntry {
    uint32_t            location;
    int                 id;
    int32_t             flag;  // 0:Scene Uniforms 1:Object Uniforms
};

};  // namespace uniform

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBUNIFORM_H_
