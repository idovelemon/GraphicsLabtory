//----------------------------------------------------
// begin: Build-in parameter uniforms
//----------------------------------------------------

// Camera setting
uniform vec3 glb_unif_EyePos;
uniform vec3 glb_unif_LookAt;

// Global light setting
uniform vec3 glb_unif_GlobalLight_Ambient;

// Parallel light setting
uniform vec3 glb_unif_ParallelLight_Dir;
uniform vec3 glb_unif_ParallelLight;

// Image based light settting
uniform sampler2D glb_unif_BRDFPFTTex;
uniform float glb_unif_SpecularPFCLOD;

// Shadow map setting
uniform sampler2D glb_unif_ShadowTex0;
uniform sampler2D glb_unif_ShadowTex1;
uniform sampler2D glb_unif_ShadowTex2;
uniform sampler2D glb_unif_ShadowTex3;
uniform mat4 glb_unif_ShadowM0;
uniform mat4 glb_unif_ShadowM1;
uniform mat4 glb_unif_ShadowM2;
uniform mat4 glb_unif_ShadowM3;
uniform float glb_unif_ShadowSplit0;
uniform float glb_unif_ShadowSplit1;
uniform float glb_unif_ShadowSplit2;

// Decal map setting
uniform mat4 glb_unif_DecalViewM;
uniform mat4 glb_unif_DecalProjM;
uniform sampler2D glb_unif_DecalTex;

//----------------------------------------------------
// end: Build-in parameter uniforms
//----------------------------------------------------