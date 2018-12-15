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

//----------------------------------------------------
// end: Build-in parameter uniforms
//----------------------------------------------------